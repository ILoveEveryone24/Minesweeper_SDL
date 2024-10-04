#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "../include/RenderWindow.h"

RenderWindow::RenderWindow(const char* title, int w, int h)
:window(NULL), renderer(NULL){
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, 0);

	if (window == NULL)
		std::cout << "Error initializing window: " << SDL_GetError() << std::endl;

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == NULL)
		std::cout << "Error initializing renderer: " << SDL_GetError() << std::endl;
}

SDL_Texture* RenderWindow::loadTexture(const char* filePath){
	SDL_Texture *texture = NULL;
	texture = IMG_LoadTexture(renderer, filePath);
	if(texture == NULL)
		std::cout << "Failed to load texture" << SDL_GetError() << std::endl;

	return texture;
}

SDL_Texture* RenderWindow::createTextureFromSurface(SDL_Surface* surface){
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

	return texture;
}

void RenderWindow::cleanUp(){
	SDL_DestroyWindow(window);
}

void RenderWindow::clear(){
	SDL_RenderClear(renderer);
}

void RenderWindow::render(SDL_Texture *tex, SDL_Rect *rect){
	SDL_RenderCopy(renderer, tex, NULL, rect);
}

void RenderWindow::display(){
	SDL_RenderPresent(renderer);
}

void RenderWindow::drawFilledRect(const SDL_Rect *rect){
	SDL_RenderFillRect(renderer, rect);
}

void RenderWindow::drawRect(const SDL_Rect *rect){
	SDL_RenderDrawRect(renderer, rect);
}

void RenderWindow::setRenderDrawColor(int r, int g, int b, int a){
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}
