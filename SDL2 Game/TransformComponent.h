#pragma once
#include "ECS.h"
#include "Vector2D.h"
#include "Constants.h"

class TransformComponent : public Component
{
public:
	Vector2D position;
	Vector2D velocity;
	Vector2D lastPosition;

	int height = 32;
	int width = 32;
	int scale = 1;

	double speed = 3;

	TransformComponent() {
		position.Zero();
	}
	TransformComponent(int sc) {
		position.x = 400;
		position.y = 600;
		scale = sc;
	}
	TransformComponent(double x, double y) {
		position.x = x;
		position.y = y;
	}
	TransformComponent(double x, double y, int h, int w, int sc) {
		position.x = x;
		position.y = y;
		width = w;
		height = h;
		scale = sc;
	}

	void init() override {
		velocity.Zero();
		lastPosition = position;
	}

	void update() override {
		lastPosition = position;
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}
};

