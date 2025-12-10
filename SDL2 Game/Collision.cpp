#include "Collision.h"
#include "ColliderComponent.h"

bool Collision::AABB(const SDL_Rect& recA, const SDL_Rect& recB) {
	if (recA.x + recA.w >= recB.x &&
		recB.x + recB.w >= recA.x &&
		recA.y + recA.h >= recB.y &&
		recB.y + recB.h >= recA.y) {
		return true;
	}
	return false;
}

bool Collision::AABB(const ColliderComponent& colA, const ColliderComponent& colB) {
	if (AABB(colA.collider, colB.collider)) {
		return true;
	}
	else { 
		return false; 
	}
}

bool Collision::AABBwithGap(const SDL_Rect& a, const SDL_Rect& b, int gap) {
	SDL_Rect a2 = { a.x + gap, a.y + gap, a.w - 2 * gap, a.h - 2 * gap };
	SDL_Rect b2 = { b.x + gap, b.y + gap, b.w - 2 * gap, b.h - 2 * gap };
	return Collision::AABB(a2, b2);
}

