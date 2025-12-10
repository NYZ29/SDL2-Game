#pragma once
#include "ECS.h"
#include "AssetManager.h"
#include "TextureManager.h"
#include "Game.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class UILabel : public Component
{
private:
	SDL_Rect position;
	std::string labelText;
	std::string labelFont;
	SDL_Color textColour;
	SDL_Texture* labelTexture;
public:
	UILabel(int xpos, int ypos, std::string text, std::string font, SDL_Color& colour) :
	labelText(text), labelFont(font), textColour(colour){
		position.x = xpos;
		position.y = ypos;
		SetLabelText(labelText,labelFont);
	}
	~UILabel(){}

	void SetLabelText(std::string text,std::string font) {
		SDL_Surface* surf = TTF_RenderText_Blended(Game::assets->GetFont(font),
			text.c_str(), textColour);
		labelTexture = SDL_CreateTextureFromSurface(Game::renderer, surf);
		SDL_FreeSurface(surf);

		SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h);
	}

	void SetLabelText(std::string text, std::string font, int size) {
		TTF_Font* fontPtr = TTF_OpenFont("fonts/arial.ttf", size);
		if (!fontPtr) {
			std::cout << "TTF_OpenFont error: " << TTF_GetError() << std::endl;
			return;
		}

		SDL_Surface* surf = TTF_RenderText_Blended(fontPtr, text.c_str(), textColour);
		TTF_CloseFont(fontPtr);

		if (!surf) {
			std::cout << "TTF_RenderText error: " << TTF_GetError() << std::endl;
			return;
		}

		labelTexture = SDL_CreateTextureFromSurface(Game::renderer, surf);
		SDL_FreeSurface(surf);

		SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h);
	}


	void draw() override {
		SDL_RenderCopy(Game::renderer, labelTexture, nullptr, &position);
	}
};

