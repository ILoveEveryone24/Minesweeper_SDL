#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "globals.h"
#include "RenderWindow.h"

class MineBlock{
	public:
		SDL_Rect border;
		bool isMine;
		bool isFlagged;
		bool isRevealed;
		bool isPressed;
		Uint8 number;
		
		MineBlock();
		MineBlock(SDL_Rect b, bool isM, bool isF, bool isR, bool isP, Uint8 num);
		MineBlock(const MineBlock &other);

		const SDL_Rect& getMine() const;

		void setTexture(SDL_Texture *oneTexture, 
				SDL_Texture *twoTexture, 
				SDL_Texture *threeTexture, 
				SDL_Texture *fourTexture, 
				SDL_Texture *fiveTexture, 
				SDL_Texture *sixTexture, 
				SDL_Texture *sevenTexture, 
				SDL_Texture *eightTexture, 
				SDL_Texture *flagTexture, 
				SDL_Texture *mineTexture);
		void draw(RenderWindow &window);

		void revealBlock(MineBlock blocks[], int pos, bool isPressed);
	private:	
		int offset = 3;
		bool isDetonated = false;
		SDL_Rect mine = {};
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
};
