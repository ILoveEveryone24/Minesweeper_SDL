#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../include/globals.h"
#include "../include/RenderWindow.h"
#include "../include/UI.h"

UI::UI()
		: timerBar({0,0,0,0}), mineBar({0,0,0,0}), resetButton({0,0,0,0}) {}

UI::UI(SDL_Rect tB, SDL_Rect mB, SDL_Rect rB)
		: timerBar(tB), mineBar(mB), resetButton(rB) {}

void UI::setTexture(SDL_Texture *winScreenTexture,
		SDL_Texture *mineBarTexture,
		SDL_Texture *timerBarTexture,
		SDL_Texture *winButtonTexture,
		SDL_Texture *lostButtonTexture,
		SDL_Texture *smileButtonTexture){

	this->winScreenTexture = winScreenTexture;
	this->mineBarTexture = mineBarTexture;
	this->timerBarTexture = timerBarTexture;
	this->winButtonTexture = winButtonTexture;
	this->lostButtonTexture = lostButtonTexture;
	this->smileButtonTexture = smileButtonTexture;
}

SDL_Rect& UI::getResetButton(){
	return this->resetButton;
}

void UI::setResetButtonPressed(bool isPressed){
	this->resetButtonPressed = isPressed;
}

void UI::setTimerTexture(SDL_Texture *timerBarTexture){
	this->timerBarTexture = timerBarTexture;	
}

void UI::setMineBarTexture(SDL_Texture *mineBarTexture){
	this->mineBarTexture = mineBarTexture;
}

void UI::draw(RenderWindow &window){

	SDL_Rect topBar = {0 + offset*2, 0 + offset*2, WIDTH - offset*4, 100-offset*4};
	SDL_Rect topBarBorder = {0, 0, WIDTH, 100};

	window.setRenderDrawColor(144, 144, 144, 255);
	window.drawFilledRect(&topBarBorder);
	window.setRenderDrawColor(111, 111, 111, 255);
	window.drawFilledRect(&topBar);

	if(resetButtonPressed)	
		window.setRenderDrawColor(111, 111, 111, 255);
	else
		window.setRenderDrawColor(144, 144, 144, 255);
	window.drawFilledRect(&this->resetButton);
	if(gameWon)
		window.render(this->winButtonTexture, &this->resetButton);
	else if(gameLost)
		window.render(this->lostButtonTexture, &this->resetButton);
	else
		window.render(this->smileButtonTexture, &this->resetButton);
	window.setRenderDrawColor(0, 0, 0, 255);
	window.drawFilledRect(&this->timerBar);
	window.drawFilledRect(&this->mineBar);
	window.render(this->mineBarTexture, &this->mineBar);
	window.render(this->timerBarTexture, &this->timerBar);

	if(gameWon){
		SDL_Rect winScreenBlock = {0, HEIGHT/2 - (HEIGHT/5)/2, WIDTH, HEIGHT/5};
		window.setRenderDrawColor(0, 0, 0, 255);
		window.drawFilledRect(&winScreenBlock);
		window.render(this->winScreenTexture, &winScreenBlock);
	}
}
