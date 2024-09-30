#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <list>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <cmath>
#include <string>
#include "../include/RenderWindow.hpp"

const int MINE_SIZE = 40;

const int GRID_W = 10;
const int GRID_H = 15;

const int WIDTH = MINE_SIZE * GRID_W;
const int HEIGHT = MINE_SIZE * GRID_H;

bool gameLost = false;
bool gameWon = false;
bool gameStarted = false;

int FPS = 0;

int timer = 0;
int mineAmount = GRID_W * GRID_H / 5;

struct mineBlock{
	SDL_Rect border;
	bool isMine;
	bool isFlagged;
	bool isRevealed;
	bool isPressed;
	Uint8 number;
	
private:	
	bool isDetonated = false;
	SDL_Rect mine = {};
	int offset = 3;
	SDL_Texture *oneTexture;
	SDL_Texture *twoTexture;
	SDL_Texture *threeTexture;
	SDL_Texture *fourTexture;
	SDL_Texture *fiveTexture;
	SDL_Texture *sixTexture;
	SDL_Texture *sevenTexture;
	SDL_Texture *eightTexture;
	SDL_Texture *flagTexture;
	SDL_Texture *mineTexture;
	SDL_Texture *winScreen;
	SDL_Texture *mineBar;
	SDL_Texture *timerBar;
	SDL_Texture *winButton;
	SDL_Texture *lostButton;
	SDL_Texture *smileButton;

public:
	mineBlock()
		: border({0,0,0,0}), isMine(false), isFlagged(false), isRevealed(false), isPressed(false), number(0){}

	mineBlock(SDL_Rect b, bool isM, bool isF, bool isR, bool isP, Uint8 num)
		: border(b), isMine(isM), isFlagged(isF), isRevealed(isR), isPressed(isP), number(num) {}

	mineBlock(const mineBlock &other)
		: border(other.border), isMine(other.isMine), isFlagged(other.isFlagged), isRevealed(other.isRevealed), isPressed(other.isPressed), number(other.number) {}

	const SDL_Rect& getMine() const{
		return mine;	
	}

	void setTexture(SDL_Texture *oneTexture, 
			SDL_Texture *twoTexture, 
			SDL_Texture *threeTexture, 
			SDL_Texture *fourTexture, 
			SDL_Texture *fiveTexture, 
			SDL_Texture *sixTexture, 
			SDL_Texture *sevenTexture, 
			SDL_Texture *eightTexture, 
			SDL_Texture *flagTexture, 
			SDL_Texture *mineTexture,
			SDL_Texture *winScreen,
			SDL_Texture *mineBar,
			SDL_Texture *timerBar,
			SDL_Texture *winButton,
			SDL_Texture *lostButton,
			SDL_Texture *smileButton){

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
		this->winScreen = winScreen;
		this->mineBar = mineBar;
		this->timerBar = timerBar;
		this->winButton = winButton;
		this->lostButton = lostButton;
		this->smileButton = smileButton;
			
	}

	void setTimerTexture(SDL_Texture *timerBar){
		this->timerBar = timerBar;	
	}

	void setMineBarTexture(SDL_Texture *mineBar){
		this->mineBar = mineBar;	
	}


	void drawMine(RenderWindow &window){
		this->mine = {this->border.x + offset, this->border.y + offset, this->border.w - offset*2, this->border.h - offset*2};
		SDL_Rect secondBorder = {this->border.x + offset, this->border.y + offset, this->border.w - offset, this->border.h - offset};

		SDL_Rect topBar = {0 + offset*2, 0 + offset*2, WIDTH - offset*4, 100-offset*4};
		SDL_Rect topBarBorder = {0, 0, WIDTH, 100};
		SDL_Rect timerBar = {WIDTH - 80 - 50/2, 50 - 50/2, 80, 50};
		SDL_Rect mineBar = {50/2, 50 - 50/2, 80, 50};
		SDL_Rect resetButton = {WIDTH/2 - 50/2, 50 - 50/2, 50, 50};

		window.setRenderDrawColor(144, 144, 144, 255);
		window.drawFilledRect(&topBarBorder);
		window.setRenderDrawColor(111, 111, 111, 255);
		window.drawFilledRect(&topBar);

		window.setRenderDrawColor(144, 144, 144, 255);
		window.drawFilledRect(&resetButton);
		if(gameWon)
			window.render(this->winButton, &resetButton);
		else if(gameLost)
			window.render(this->lostButton, &resetButton);
		else
			window.render(this->smileButton, &resetButton);

		window.setRenderDrawColor(0, 0, 0, 255);
		window.drawFilledRect(&timerBar);
		window.drawFilledRect(&mineBar);
		window.render(this->mineBar, &mineBar);
		window.render(this->timerBar, &timerBar);

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
			SDL_Rect winScreenBlock = {0, HEIGHT/2 - (HEIGHT/5)/2, WIDTH, HEIGHT/5};
			window.setRenderDrawColor(0, 0, 0, 255);
			window.drawFilledRect(&winScreenBlock);
			window.render(this->winScreen, &winScreenBlock);
		}
	}

	void revealBlock(mineBlock blocks[], int pos, bool isPressed){
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
};

bool mouseIntersect(SDL_Rect rect, Sint32 &x, Sint32 &y){
	if(x >= rect.x && x < (rect.x + rect.w) && y >= rect.y && y < (rect.y + rect.h))
		return true;
	else
		return false;
}

void init(std::list<int> &mines, mineBlock blocks[], TTF_Font &textFont, RenderWindow &window){
	srand (time(NULL));

	bool addMine = true;
	
	SDL_Surface *winText = TTF_RenderText_Solid(&textFont, "YOU WIN!", {255,255,255});
	SDL_Surface *mineCountText;
	SDL_Surface *timerText = TTF_RenderText_Solid(&textFont, "000", {255,0,0});
	if(mineAmount < -9)
		mineCountText = TTF_RenderText_Solid(&textFont, std::to_string(mineAmount).c_str(), {255,0,0});
	else if(mineAmount < 0)
		mineCountText = TTF_RenderText_Solid(&textFont, (std::string("0")+std::to_string(mineAmount)).c_str(), {255,0,0});
	else if(mineAmount < 10)
		mineCountText = TTF_RenderText_Solid(&textFont, (std::string("00")+std::to_string(mineAmount)).c_str(), {255,0,0});
	else if(mineAmount < 100)
		mineCountText = TTF_RenderText_Solid(&textFont, (std::string("0")+std::to_string(mineAmount)).c_str(), {255,0,0});
	else
		mineCountText = TTF_RenderText_Solid(&textFont, std::to_string(mineAmount).c_str(), {255,0,0});

	//Loading textures
	SDL_Texture *winTexture = window.createTextureFromSurface(winText);
	SDL_Texture *mineCountTexture = window.createTextureFromSurface(mineCountText);
	SDL_Texture *timerTexture = window.createTextureFromSurface(timerText);
	SDL_Texture *oneTexture = window.loadTexture("./images/one.png");
	SDL_Texture *twoTexture = window.loadTexture("./images/two.png");
	SDL_Texture *threeTexture = window.loadTexture("./images/three.png");
	SDL_Texture *fourTexture = window.loadTexture("./images/four.png");
	SDL_Texture *fiveTexture = window.loadTexture("./images/five.png");
	SDL_Texture *sixTexture = window.loadTexture("./images/six.png");
	SDL_Texture *sevenTexture = window.loadTexture("./images/seven.png");
	SDL_Texture *eightTexture = window.loadTexture("./images/eight.png");
	SDL_Texture *flagTexture = window.loadTexture("./images/flag.png");
	SDL_Texture *mineTexture = window.loadTexture("./images/mine.png");
	SDL_Texture *winButtonTexture = window.loadTexture("./images/win.png");
	SDL_Texture *lostButtonTexture = window.loadTexture("./images/dead.png");
	SDL_Texture *smileButtonTexture = window.loadTexture("./images/smile.png");

	while(mines.size() < mineAmount){
		int mine = rand() % (GRID_W * GRID_H);	

		for(std::list<int>::iterator i = mines.begin(); i != mines.end(); i++){
			if(mine == *i){
				addMine = false;
				break;
			}
		}
		if(addMine)
			mines.push_back(mine);

		addMine = true;
	}
	mines.sort();
	int tmp = 0;
	
	bool isMine = false;
	for(int i = 0; i < GRID_W; i++){
		for(int x = 0; x < GRID_H; x++){
			isMine = false;	
			if(std::find(mines.begin(), mines.end(), i * GRID_H + x) != mines.end()){
				isMine = true;	
				tmp++;	
			}
			mineBlock block (SDL_Rect{i*WIDTH/GRID_W, x*WIDTH/GRID_W + 100, MINE_SIZE, MINE_SIZE}, isMine, false, false, false, 0);

			block.setTexture(
			oneTexture, 
			twoTexture, 
			threeTexture, 
			fourTexture, 
			fiveTexture, 
			sixTexture, 
			sevenTexture, 
			eightTexture, 
			flagTexture, 
			mineTexture,
			winTexture,
			mineCountTexture,
			timerTexture,
			winButtonTexture,
			lostButtonTexture,
			smileButtonTexture);

			blocks[i * GRID_H + x] = block;
		}
	}
	std::cout << tmp << " " << mines.size() << std::endl;
}

void gameReset(std::list<int> &mines, mineBlock blocks[], TTF_Font &textFont, RenderWindow &window){

	mineAmount = GRID_W * GRID_H / 5;
	gameLost = false;
	gameWon = false;
	gameStarted = false;
	timer = 0;
	mines.clear();
	for(size_t i = 0; i < sizeof(*blocks)/sizeof(blocks[0]); i++){
		blocks[i].isMine = false;
		blocks[i].isFlagged = false;
		blocks[i].isRevealed = false;
		blocks[i].isPressed = false;
		blocks[i].number = 0;
	}

	init(mines, blocks, textFont, window);
}

int main(int argc, char* args[]){

	if (SDL_Init(SDL_INIT_VIDEO) > 0){
		std::cout << "SDL_Init ERROR: " << SDL_GetError()  << std::endl;
		return -1;
	}

	if (!(IMG_Init(IMG_INIT_PNG))){
		std::cout << "IMG_Init ERROR: " << SDL_GetError() << std::endl;
		return -1;	
	}

	if(TTF_Init() == -1){
		std::cout << "TTF_Init ERROR: " << SDL_GetError() << std::endl;
		return -1;	
	}

	TTF_Font *textFont = TTF_OpenFont("./font.ttf", 128);

	if(textFont == nullptr){
		std::cout << "TTF_Font ERROR: " << SDL_GetError() << std::endl;
		return -1;	
	}

	RenderWindow window("Minesweeper", WIDTH, HEIGHT+100);
	std::list<int> mines;
	mineBlock blocks[GRID_W * GRID_H];

	init(mines, blocks, *textFont, window);

	bool gameRunning = true;

	SDL_Event event;

	time_t currentTime;
	time_t startTime;
	time(&startTime);
	int deltaT;

	int flagCount = 0;
	int mineCountDiff = 0;

	SDL_Surface *timerText = TTF_RenderText_Solid(textFont, "000", {255,0,0});
	SDL_Surface *mineCountText = TTF_RenderText_Solid(textFont, "000", {255,0,0});

	SDL_Texture *timerTexture = window.createTextureFromSurface(timerText);
	SDL_Texture *mineCountTexture = window.createTextureFromSurface(mineCountText);
	while(gameRunning){
		//FPS
		time(&currentTime);
		deltaT = currentTime - startTime;
		if(deltaT < 1)
			FPS++;
		else{
			startTime = currentTime;
			std::cout << FPS << std::endl;
			FPS = 0;
		}
		//FPS
		if(gameStarted)
			if(deltaT >= 1){
				timer++;
				if(timer > 999)
					timer = 999;
			
				if(timer < 10)
					timerText = TTF_RenderText_Solid(textFont, (std::string("00")+std::to_string(timer)).c_str(), {255,0,0});
				else if(timer < 100)
					timerText = TTF_RenderText_Solid(textFont, (std::string("0")+std::to_string(timer)).c_str(), {255,0,0});
				else
					timerText = TTF_RenderText_Solid(textFont, std::to_string(timer).c_str(), {255,0,0});
				timerTexture = window.createTextureFromSurface(timerText);
				
				for(size_t i = 0; i < sizeof(blocks)/sizeof(blocks[0]); i++)
					blocks[i].setTimerTexture(timerTexture);
			}
		
		
		if(mineCountDiff != mineAmount - flagCount){
			mineCountDiff = mineAmount - flagCount;

			if(mineCountDiff < -99)
				mineCountDiff = -99;
			if(mineCountDiff < -9)
				mineCountText = TTF_RenderText_Solid(textFont, std::to_string(mineCountDiff).c_str(), {255,0,0});
			else if(mineCountDiff < 0)
				mineCountText = TTF_RenderText_Solid(textFont, (std::string("0")+std::to_string(mineCountDiff)).c_str(), {255,0,0});
			else if(mineCountDiff < 10)
				mineCountText = TTF_RenderText_Solid(textFont, (std::string("00")+std::to_string(mineCountDiff)).c_str(), {255,0,0});
			else if(mineCountDiff < 100)
				mineCountText = TTF_RenderText_Solid(textFont, (std::string("0")+std::to_string(mineCountDiff)).c_str(), {255,0,0});
			else
				mineCountText = TTF_RenderText_Solid(textFont, std::to_string(mineCountDiff).c_str(), {255,0,0});
			mineCountTexture = window.createTextureFromSurface(mineCountText);
			for(size_t i = 0; i < sizeof(blocks)/sizeof(blocks[0]); i++)
					blocks[i].setMineBarTexture(mineCountTexture);
			
		}


		window.clear();

		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_QUIT:
					gameRunning = false;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym){
						case SDLK_r:
							gameReset(mines, blocks, *textFont, window);
							break;
					}
				case SDL_MOUSEBUTTONUP:
					if(!gameLost && !gameWon)
						for(size_t i = 0; i < sizeof(blocks)/sizeof(blocks[0]); i++){
							blocks[i].isPressed = false;
							if(event.button.button == SDL_BUTTON_LEFT && mouseIntersect(blocks[i].getMine(), event.button.x, event.button.y)){
								blocks[i].revealBlock(blocks, i, true);
							}
							if(event.button.button == SDL_BUTTON_RIGHT && mouseIntersect(blocks[i].getMine(), event.button.x, event.button.y))
								if(!blocks[i].isRevealed)
									blocks[i].isFlagged = !blocks[i].isFlagged;
						}
					break;
				default:	
					break;
			}
		}
		int x, y;
		Uint32 mouseState = SDL_GetMouseState(&x, &y);

		for(size_t i = 0; i < sizeof(blocks)/sizeof(blocks[0]); i++)
			if(!gameWon && !gameLost && mouseState & SDL_BUTTON(SDL_BUTTON_LEFT) && mouseIntersect(blocks[i].getMine(), x, y))
				blocks[i].isPressed = true;
			else
				blocks[i].isPressed = false;

		for(size_t i = 0; i < sizeof(blocks)/sizeof(blocks[0]); i++)
			if((blocks[i].isFlagged || blocks[i].isRevealed) && !blocks[i].isMine && !gameWon && !gameLost)
				gameStarted = true;
			

		gameWon = true;
		for(size_t i = 0; i < sizeof(blocks)/sizeof(blocks[0]); i++)
			if(!blocks[i].isRevealed && !blocks[i].isMine)
				gameWon = false;	

		flagCount = 0;
		for(size_t i = 0; i < sizeof(blocks)/sizeof(blocks[0]); i++)
			if(blocks[i].isFlagged)
				flagCount++;

		for(size_t i = 0; i < sizeof(blocks)/sizeof(blocks[0]); i++)
			blocks[i].drawMine(window);

		window.display();

	}

	window.cleanUp();
	TTF_CloseFont(textFont);
	SDL_Quit();

	return 0;
}
