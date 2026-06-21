#ifndef LEVEL3_H
#define LEVEL3_H

#include "Scene.h"

constexpr int LEVEL_3_WIDTH       = 50,
              LEVEL_3_HEIGHT      = 8,
              LEVEL_3_ENEMY_COUNT = 10,
              LEVEL_3_BALL_COUNT  = 3;

class Level3 : public Scene
{
private:
    unsigned int mLevelData[LEVEL_3_WIDTH * LEVEL_3_HEIGHT] = {
        
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       
        0, 0, 0, 0, 0, 2, 0, 0, 2, 2, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 2, 0, 2, 2, 2, 0, 0, 0, 2, 0, 0, 0, 2, 2, 2, 0, 2, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 3, 0, 0, 3, 3, 3, 0, 3, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 3, 0, 3, 3, 3, 0, 0, 0, 3, 0, 0, 0, 3, 3, 3, 0, 3, 0, 0, 0, 0, 0,
        2, 2, 2, 2, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 2,
        3, 3, 3, 3, 0, 0, 0, 3, 3, 3, 3, 3, 0, 0, 3, 3, 3, 3, 3, 0, 0, 0, 3, 3, 3, 3, 3, 0, 0, 3, 3, 3, 3, 3, 3, 0, 0, 0, 3, 3, 3, 3, 3, 0, 0, 3, 3, 3, 3, 3,
    };

    PlayerState mPlayerState = IDLE_STATE;
    Vector2     mStartPosition;
    Vector2     mEnemyStartPositions[LEVEL_3_ENEMY_COUNT];
    float       mBallBaseX[LEVEL_3_BALL_COUNT]; 
    float       mBallBaseY[LEVEL_3_BALL_COUNT]; 
    float       mHitCooldown = 0.0f;

    void updatePlayerAnimation();
    void checkEnemyCollisions();

public:
    static constexpr float TILE_DIMENSION          = 75.0f,
                           ACCELERATION_OF_GRAVITY = 1100.0f,
                           HIT_COOLDOWN_DURATION   = 1.5f,
                           BALL_AMPLITUDE          = 110.0f; 

    Level3();
    Level3(Vector2 origin, const char *bgHexCode);
    ~Level3();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // LEVEL3_H