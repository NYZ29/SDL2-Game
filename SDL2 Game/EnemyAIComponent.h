#pragma once
#include "Components.h"

enum class EnemyBehavior {
    Chase,
    Patrol,
    Shooter
};

class EnemyAIComponent : public Component {
public:
    TransformComponent* transform = nullptr;
    TransformComponent* playerTransform = nullptr;

    EnemyBehavior behavior;
    double speed;
    float patrolLeft;   
    float patrolRight;
    int dir = 1;

    EnemyAIComponent(TransformComponent* playerTr,
        EnemyBehavior bh = EnemyBehavior::Chase,
        double spd = 1.5,
        double left = 0,
        double right = 0)
        : playerTransform(playerTr),
        behavior(bh),
        speed(spd),
        patrolLeft(left),
        patrolRight(right) {
    }

    void init() override {
        transform = &entity->getComponent<TransformComponent>();
    }

    void update() override {
        Vector2D dirVec(0, 0);

        switch (behavior) {
        case EnemyBehavior::Chase:
            if (playerTransform->position.x > transform->position.x) dirVec.x = 1;
            else if (playerTransform->position.x < transform->position.x) dirVec.x = -1;

            if (playerTransform->position.y > transform->position.y) dirVec.y = 1;
            else if (playerTransform->position.y < transform->position.y) dirVec.y = -1;
            break;

        case EnemyBehavior::Patrol:
            dirVec.x = dir;
            if (transform->position.x < patrolLeft)  dir = 1;
            if (transform->position.x > patrolRight) dir = -1;
            break;

        case EnemyBehavior::Shooter:
        {
            double dx = playerTransform->position.x - transform->position.x;
            double dy = playerTransform->position.y - transform->position.y;
            double dist2 = dx * dx + dy * dy;

            const double minDist2 = 200.0 * 200.0;
            const double maxDist2 = 350.0 * 350.0;

            if (dist2 > maxDist2) {
                if (dx > 0) dirVec.x = 1;
                else if (dx < 0) dirVec.x = -1;
                if (dy > 0) dirVec.y = 1;
                else if (dy < 0) dirVec.y = -1;
            }
            else if (dist2 < minDist2) {
                if (dx > 0) dirVec.x = -1;
                else if (dx < 0) dirVec.x = 1;
                if (dy > 0) dirVec.y = -1;
                else if (dy < 0) dirVec.y = 1;
            }
        }
        break;
        }

        transform->velocity = dirVec;
        transform->speed = speed;
    }
};

