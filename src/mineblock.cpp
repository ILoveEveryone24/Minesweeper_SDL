#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../include/globals.h"
#include "../include/RenderWindow.h"
#include "../include/MineBlock.h"

MineBlock::MineBlock()
		: border({0,0,0,0}), isMine(false), isFlagged(false), isRevealed(false), isPressed(false), number(0){}

MineBlock::MineBlock(SDL_Rect b, bool isM, bool isF, bool isR, bool isP, Uint8 num)
		: border(b), isMine(isM), isFlagged(isF), isRevealed(isR), isPressed(isP), number(num) {}

MineBlock::MineBlock(const MineBlock &other)
		: border(other.border), isMine(other.isMine), isFlagged(other.isFlagged), isRevealed(other.isRevealed), isPressed(other.isPressed), number(other.number) {}

const SDL_Rect& MineBlock::getMine() const{
		return mine;	
}

void MineBlock::setTexture(SDL_Texture *oneTexture, 
		SDL_Texture *twoTexture, 
		SDL_Texture *threeTexture, 
		SDL_Texture *fourTexture, 
		SDL_Texture *fiveTexture, 
		SDL_Texture *sixTexture, 
		SDL_Texture *sevenTexture, 
		SDL_Texture *eightTexture, 
		SDL_Texture *flagTexture, 
		SDL_Texture *mineTexture){

	this->oneTexture = oneTexture;
	this->twoTexture = twoTexture;
	this->threeTexture = threeTexture;
	this->fourTexture = fourTexture;
	this->fiveTexture = fiveTexture;
	this->sixTexture = sixTexture;
	this->sevenTexture = sevenTexture;
	this->eightTexture = eightTexture;
	this->flagTexture = flagTexture;
	this->mineTexture = mineTexture;
		
}



void MineBlock::draw(RenderWindow &window){
	this->mine = {this->border.x + offset, this->border.y + offset, this->border.w - offset*2, this->border.h - offset*2};
	SDL_Rect secondBorder = {this->border.x + offset, this->border.y + offset, this->border.w - offset, this->border.h - offset};



	window.setRenderDrawColor(144, 144, 144, 255);
	window.drawFilledRect(&this->border);

	window.setRenderDrawColor(111, 111, 111, 255);
	window.drawFilledRect(&secondBorder);
	

	if(this->isRevealed)
		if(this->isMine){
			if(this->isDetonated){
				window.setRenderDrawColor(255, 10, 10, 255);
				window.drawFilledRect(&this->mine);
			}
			window.render(this->mineTexture, &this->mine);
		}
		else
			switch(this->number){
				case 1:	
					window.render(this->oneTexture, &this->mine);
					break;
				case 2:
					window.render(this->twoTexture, &this->mine);
					break;
				case 3:
					window.render(this->threeTexture, &this->mine);
					break;
				case 4:
					window.render(this->fourTexture, &this->mine);
					break;
				case 5:
					window.render(this->fiveTexture, &this->mine);
					break;
				case 6:
					window.render(this->sixTexture, &this->mine);
					break;
				case 7:
					window.render(this->sevenTexture, &this->mine);
					break;
				case 8:
					window.render(this->eightTexture, &this->mine);
					break;
				default:
					window.setRenderDrawColor(111, 111, 111, 255);
					break;
			}
	else if(this->isPressed)
		window.setRenderDrawColor(111, 111, 111, 255);
	else
		window.setRenderDrawColor(133, 133, 133, 255);

	if(this->isFlagged){
		window.setRenderDrawColor(133, 133, 133, 255);
		window.drawFilledRect(&this->mine);
		window.render(this->flagTexture, &this->mine);
	}


	
	if(!this->isRevealed && !this->isFlagged)	
		window.drawFilledRect(&this->mine);

	if(gameWon){
		gameStarted = false;
		if(!this->isRevealed)
			this->isFlagged = true;
	}
}
void MineBlock::revealBlock(MineBlock blocks[], int pos, bool isPressed){
	int adjacentBlocks[9] = {-GRID_H - 1, -GRID_H, -GRID_H + 1, -1, 0, +1, GRID_H - 1, GRID_H, GRID_H + 1};

	int mineCnt = 0;

	int edgeCase = (pos - GRID_H);
	if (edgeCase < 0)
		edgeCase = -1;
	else
		edgeCase /= GRID_H;
	if(!blocks[pos].isFlagged){
		if(blocks[pos].isMine && isPressed){
			gameLost = true;
			gameStarted = false;
			blocks[pos].isDetonated = true;
			for(int i = 0; i < GRID_H*GRID_W; i++)
				if(blocks[i].isMine)
					blocks[i].isRevealed = true;
		}
		else{
			blocks[pos].isRevealed = true;
			for(int i = 0; i < 9; i++){
				if(i > 0 && i % 3 == 0)
					edgeCase++;
				if(pos + adjacentBlocks[i] >= 0 && pos + adjacentBlocks[i] < GRID_W * GRID_H && ((pos + adjacentBlocks[i]) / GRID_H) == edgeCase)
					if(blocks[pos + adjacentBlocks[i]].isMine){
						mineCnt++;
					}
			}
			blocks[pos].number = mineCnt;

			edgeCase = (pos - GRID_H);
			if (edgeCase < 0)
				edgeCase = -1;
			else
				edgeCase /= GRID_H;

			if(blocks[pos].number == 0){
				for(int i = 0; i < 9; i++){
					if(i > 0 && i % 3 == 0)
						edgeCase++;
					if(pos + adjacentBlocks[i] >= 0 && pos + adjacentBlocks[i] < GRID_W * GRID_H && ((pos + adjacentBlocks[i]) / GRID_H) == edgeCase)
						if(!blocks[pos + adjacentBlocks[i]].isMine && !blocks[pos + adjacentBlocks[i]].isRevealed)
							blocks[pos + adjacentBlocks[i]].revealBlock(blocks, pos + adjacentBlocks[i], false);
				}
			}
		}
	}
}
