#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "Constants.h"
#include <sstream>
#include <iomanip>

Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
SDL_Rect Game::camera = { 0,0,ScreenWidth,ScreenHeight };

AssetManager* Game::assets = new AssetManager(&manager);

bool Game::isRunning = false;

auto& player = manager.addEntity();
auto& label = manager.addEntity();
auto& enemy1 = manager.addEntity();
auto& enemy2 = manager.addEntity();
auto& enemy3 = manager.addEntity();

auto& timeLabel = manager.addEntity();

Game::Game() {}

Game::~Game() {}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}
	
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}
		isRunning = true;
	}
	else {
		isRunning = false;
	}

	if (TTF_Init() == -1) {
		std::cout << "Error : SDL_TTF" << std::endl;
	}

	assets->AddTexture("terrain", "images/terrain_ss.png");
	assets->AddTexture("player", "images/player_anims.png");
	assets->AddTexture("enemy", "images/evil_anims.png");
	assets->AddTexture("projectile", "images/proj.png");

	assets->AddFont("arial", "fonts/arial.ttf", 32);

	map = new Map("terrain", 3, 32);

	map->LoadMap("images/map.map", 25, 20);

	player.addComponent<TransformComponent>(4);
	player.addComponent<SpriteComponent>("player", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(groupPlayers);

	enemy1.addComponent<TransformComponent>(600, 1000, 32, 32, 4);
	enemy1.addComponent<SpriteComponent>("enemy", true);
	enemy1.addComponent<ColliderComponent>("enemy");
	enemy1.addComponent<EnemyAIComponent>(
		&player.getComponent<TransformComponent>(),
		EnemyBehavior::Chase, 1.5);
	enemy1.addGroup(groupEnemies);

	enemy2.addComponent<TransformComponent>(1200, 300, 32, 32, 4);
	enemy2.addComponent<SpriteComponent>("enemy", true);
	enemy2.addComponent<ColliderComponent>("enemy");
	enemy2.addComponent<EnemyAIComponent>(
		&player.getComponent<TransformComponent>(),
		EnemyBehavior::Patrol, 1, 1100.0, 1300.0);
	enemy2.addGroup(groupEnemies);

	enemy3.addComponent<TransformComponent>(200, 1500, 32, 32, 4);
	enemy3.addComponent<SpriteComponent>("enemy", true);
	enemy3.addComponent<ColliderComponent>("enemy");
	enemy3.addComponent<EnemyAIComponent>(
		&player.getComponent<TransformComponent>(), EnemyBehavior::Shooter, 1);
	enemy3.addGroup(groupEnemies);

	SDL_Color white = { 255,255,255,255 };
	label.addComponent<UILabel>(10, 10, "Test String", "arial", white);

	timeLabel.addComponent<UILabel>(10, 40, "Time: 0.0", "arial", white);

	assets->CreateProjectile(Vector2D(600, 600), Vector2D(2, 0), 200, 2, "projectile");
	assets->CreateProjectile(Vector2D(600, 620), Vector2D(2, 0), 200, 2, "projectile");
	assets->CreateProjectile(Vector2D(400, 600), Vector2D(2, 1), 200, 2, "projectile");
	assets->CreateProjectile(Vector2D(600, 600), Vector2D(2, -1), 200, 2, "projectile");
}

auto& tiles = manager.getGroup(Game::groupMap);
auto& players = manager.getGroup(Game::groupPlayers);
auto& enemies = manager.getGroup(Game::groupEnemies);
auto& colliders = manager.getGroup(Game::groupColliders);
auto& projectiles = manager.getGroup(Game::groupProjectiles);

void Game::handleEvents() {
	SDL_PollEvent(&event);

	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;

	default:

		break;
	}
}

void Game::update() {
	if (gameOver) {
		player.getComponent<TransformComponent>().velocity.Zero();
		label.getComponent<UILabel>().SetLabelText("GAME OVER", "arial", 48);
		manager.refresh();
		return;
	}

	timeSeconds += 1.0 / FPS;
	manager.refresh();
	manager.update();

	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	Vector2D playerPos = player.getComponent<TransformComponent>().position;

	std::stringstream ss;
	ss << "Player position: " << playerPos;
	label.getComponent<UILabel>().SetLabelText(ss.str(), "arial", 24);
	std::stringstream ssTime;
	ssTime << "Time: " << std::fixed << std::setprecision(1) << timeSeconds;
	timeLabel.getComponent<UILabel>().SetLabelText(ssTime.str(), "arial", 24);

	for (auto& c : colliders) {
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, playerCol)) {
			auto& tr = player.getComponent<TransformComponent>();
			tr.position = tr.lastPosition;
			tr.velocity.Zero();
			playerCol = player.getComponent<ColliderComponent>().collider;
		}
	}


	for (auto& e : enemies) {
		SDL_Rect enemyCol = e->getComponent<ColliderComponent>().collider;
		if (Collision::AABBwithGap(playerCol, enemyCol, 48)) {
			gameOver = true;
			break;
		}
	}

	for (auto& p : projectiles) {
		if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
			p->getComponent<ColliderComponent>().collider)) {
			std::cout << "Hit player!" << std::endl;
			p->destroy();
		}
	}

	camera.x = static_cast<int>(player.getComponent<TransformComponent>().position.x - ScreenWidth / 2);
	camera.y = static_cast<int>(player.getComponent<TransformComponent>().position.y - ScreenHeight / 2);

	if (camera.x < 0) camera.x = 0;
	if (camera.y < 0) camera.y = 0;
}

void Game::render() {
	SDL_RenderClear(renderer);
	for (auto& t : tiles) {
		t->draw();
	}
	for (auto& c : colliders) {
		c->draw();
	}
	for (auto& p : players) {
		p->draw();
	}
	for (auto& e : enemies) {
		e->draw();
	}
	for (auto& p : projectiles) {
		p->draw();
	}

	label.draw();
	timeLabel.draw();

	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	IMG_Quit();
	SDL_Quit();
}

