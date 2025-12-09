#include "GameObject.h"
#include "TextureManager.h"

GameObject::GameObject(const char* texturesheet, int x, int y) {
	objTexture = TextureManager::LoadTexture(texturesheet);

	xpos = x;
	ypos = y;
}

GameObject::~GameObject() {

}

void GameObject::Update() {
	xpos++;
	ypos++;

	srcRect = { 0,0,32,32 };
	destRect = { xpos,ypos,srcRect.w * 2,srcRect.h * 2 };
}

void GameObject::Render() {
	SDL_RenderCopy(Game::renderer, objTexture, &srcRect, &destRect);
}

