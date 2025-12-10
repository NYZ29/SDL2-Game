#pragma once
#include <SDL.h>

class ColliderComponent;

class Collision
{
public:
	static bool AABB(const SDL_Rect& recA, const SDL_Rect& recB);
	static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB);
	static bool AABBwithGap(const SDL_Rect& a, const SDL_Rect& b, int gap);
};

