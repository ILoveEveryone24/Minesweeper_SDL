#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class RenderWindow{
	public:
		RenderWindow(const char* title, int w, int h);
		SDL_Texture *loadTexture(const char* filePath);
		SDL_Texture *createTextureFromSurface(SDL_Surface *surface);
		void cleanUp();
		void clear();
		void render(SDL_Texture *tex, SDL_Rect *rect);
		void display();

		void drawFilledRect(const SDL_Rect *rect);
		void drawRect(const SDL_Rect *rect);
		void setRenderDrawColor(int r, int g, int b, int a);
	private:
		SDL_Window *window;
		SDL_Renderer *renderer;
};
