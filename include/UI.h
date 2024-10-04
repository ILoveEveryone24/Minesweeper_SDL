#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "globals.h"
#include "RenderWindow.h"

class UI{
	public:
		SDL_Rect timerBar;
		SDL_Rect mineBar;
		SDL_Rect resetButton;

		UI();
		UI(SDL_Rect tB, SDL_Rect mB, SDL_Rect rB);

		void setTexture(SDL_Texture *winScreenTexture,
				SDL_Texture *mineBarTexture,
				SDL_Texture *timerBarTexture,
				SDL_Texture *winButtonTexture,
				SDL_Texture *lostButtonTexture,
				SDL_Texture *smileButtonTexture);

		SDL_Rect& getResetButton();
		void setResetButtonPressed(bool isPressed);
		void setTimerTexture(SDL_Texture *timerBarTexture);
		void setMineBarTexture(SDL_Texture *mineBarTexture);

		void draw(RenderWindow &window);

	private:
		int offset = 3;
		bool resetButtonPressed = false;
		SDL_Texture *winScreenTexture;
		SDL_Texture *mineBarTexture;
		SDL_Texture *timerBarTexture;
		SDL_Texture *winButtonTexture;
		SDL_Texture *lostButtonTexture;
		SDL_Texture *smileButtonTexture;
};
