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
#include "../include/globals.h"
#include "../include/RenderWindow.h"
#include "../include/MineBlock.h"
#include "../include/UI.h"

bool mouseIntersect(SDL_Rect rect, Sint32 &x, Sint32 &y){
	if(x >= rect.x && x < (rect.x + rect.w) && y >= rect.y && y < (rect.y + rect.h))
		return true;
	else
		return false;
}

void init(std::list<int> &mines, MineBlock blocks[], UI &gameUI, TTF_Font &textFont, RenderWindow &window){
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

	while(static_cast<int>(mines.size()) < mineAmount){
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
			MineBlock block (SDL_Rect{i*WIDTH/GRID_W, x*WIDTH/GRID_W + 100, MINE_SIZE, MINE_SIZE}, isMine, false, false, false, 0);

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
			mineTexture);

			blocks[i * GRID_H + x] = block;
		}
	}

	SDL_Rect timerBar = {WIDTH - 80 - 50/2, 50 - 50/2, 80, 50};
	SDL_Rect mineBar = {50/2, 50 - 50/2, 80, 50};
	SDL_Rect resetButton = {WIDTH/2 - 50/2, 50 - 50/2, 50, 50};

	gameUI.timerBar =  timerBar;
	gameUI.mineBar = mineBar;
	gameUI.resetButton = resetButton;

	gameUI.setTexture(winTexture,
			  mineCountTexture,
			  timerTexture,
			  winButtonTexture,
			  lostButtonTexture,
			  smileButtonTexture);
}

void gameReset(std::list<int> &mines, MineBlock blocks[], UI &gameUI, TTF_Font &textFont, RenderWindow &window){

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

	init(mines, blocks, gameUI, textFont, window);
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
	MineBlock blocks[GRID_W * GRID_H];

	UI gameUI;

	init(mines, blocks, gameUI, *textFont, window);



	bool gameRunning = true;

	SDL_Event event;

	time_t currentTime;
	time_t startTime;
	time(&startTime);
	int deltaT = 0;

	int flagCount = 0;
	int mineCountDiff = 0;

	SDL_Surface *timerText = TTF_RenderText_Solid(textFont, "000", {255,0,0});
	SDL_Surface *mineCountText = TTF_RenderText_Solid(textFont, "000", {255,0,0});

	SDL_Texture *timerTexture = window.createTextureFromSurface(timerText);
	SDL_Texture *mineCountTexture = window.createTextureFromSurface(mineCountText);
	while(gameRunning){

		time(&currentTime);
		deltaT = currentTime - startTime;
		if(deltaT < 1)
			FPS++;
		else{
			startTime = currentTime;
			std::cout << FPS << std::endl;
			FPS = 0;
		}
		
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
				
				gameUI.setTimerTexture(timerTexture);
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

			gameUI.setMineBarTexture(mineCountTexture);
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
							gameReset(mines, blocks, gameUI, *textFont, window);
							break;
					}
				case SDL_MOUSEBUTTONUP:
					if(event.button.button == SDL_BUTTON_LEFT && mouseIntersect(gameUI.getResetButton(), event.button.x, event.button.y))
						gameReset(mines, blocks, gameUI, *textFont, window);

					if(!gameLost && !gameWon){
						for(size_t i = 0; i < sizeof(blocks)/sizeof(blocks[0]); i++){
							blocks[i].isPressed = false;
							if(event.button.button == SDL_BUTTON_LEFT && mouseIntersect(blocks[i].getMine(), event.button.x, event.button.y)){
								blocks[i].revealBlock(blocks, i, true);
							}
							if(event.button.button == SDL_BUTTON_RIGHT && mouseIntersect(blocks[i].getMine(), event.button.x, event.button.y))
								if(!blocks[i].isRevealed)
									blocks[i].isFlagged = !blocks[i].isFlagged;
						}
					}
					break;
				default:	
					break;
			}
		}
		int x, y;
		Uint32 mouseState = SDL_GetMouseState(&x, &y);
		if(mouseState & SDL_BUTTON(SDL_BUTTON_LEFT) && mouseIntersect(gameUI.getResetButton(), x, y))
			gameUI.setResetButtonPressed(true);
		else
			gameUI.setResetButtonPressed(false);

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
			blocks[i].draw(window);

		gameUI.draw(window);

		window.display();
	}

	window.cleanUp();
	TTF_CloseFont(textFont);
	SDL_Quit();

	return 0;
}
