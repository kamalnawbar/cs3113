#ifndef LEVEL1_H
#define LEVEL1_H

#include "Scene.h"

constexpr int LEVEL_1_WIDTH        = 38,
              LEVEL_1_HEIGHT       = 8,
              LEVEL_1_ENEMY_COUNT  = 5; 

class Level1 : public Scene
{
private:
    unsigned int mLevelData[LEVEL_1_WIDTH * LEVEL_1_HEIGHT] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 2, 0, 0, 3, 3, 3, 0, 0, 2, 2, 0, 0, 0, 0, 0, 2, 0, 0, 3, 3, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        2, 2, 2, 2, 2, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2,
        
        3, 3, 3, 3, 3, 0, 0, 0, 3, 3, 3, 3, 3, 3, 0, 0, 3, 3, 3, 3, 0, 0, 0, 3, 3, 3, 3, 3, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
    };

    PlayerState mPlayerState = IDLE_STATE;
    Vector2     mStartPosition;
    Vector2     mEnemyStartPositions[LEVEL_1_ENEMY_COUNT];
    float       mHitCooldown = 0.0f;

    void updatePlayerAnimation();
    void checkEnemyCollisions();

public:
    static constexpr float TILE_DIMENSION          = 75.0f,
                           ACCELERATION_OF_GRAVITY = 1100.0f,
                           HIT_COOLDOWN_DURATION   = 1.5f;

    Level1();
    Level1(Vector2 origin, const char *bgHexCode);
    ~Level1();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // LEVEL1_H