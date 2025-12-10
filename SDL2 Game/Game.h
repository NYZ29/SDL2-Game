#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include "AssetManager.h"

class ColliderComponent;
class AssetManager;

class Game
{
private:
	double timeSeconds = 0.0;
	int cnt = 0;
	SDL_Window* window;
public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	bool running() { return isRunning; }
	void handleEvents();
	void update();
	void render();
	void clean();

	static SDL_Renderer* renderer;
	static SDL_Event event;
	static bool isRunning;
	bool gameOver = false;
	static SDL_Rect camera;
	static AssetManager* assets;
	enum groupLabels : std::size_t {
		groupMap,
		groupPlayers,
		groupEnemies,
		groupColliders,
		groupProjectiles
	};
};

