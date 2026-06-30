#ifndef LEVEL1_H
#define LEVEL1_H

#include "Scene.h"
#include "ShaderProgram.h"

constexpr int LEVEL_1_WIDTH        = 14,
              LEVEL_1_HEIGHT       = 8,
              LEVEL_1_TARGET_COUNT = 3,
              LEVEL_1_MAX_BOUNCES  = 4,
              LEVEL_1_MAX_BULLETS  = 5;

class Level1 : public Scene
{
private:
    unsigned int mLevelData[LEVEL_1_WIDTH * LEVEL_1_HEIGHT] = {
        43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43,
        43,  0,  0,  0,  0,  0,  0,  0,  0, 43,  0,  0,  0, 43,
        43,  0, 43, 43,  0,  0,  0,  0,  0, 43,  0,  0,  0, 43,
        43,  0,  0,  0,  0,  0, 43,  0,  0,  0,  0,  0,  0, 43,
        43,  0,  0,  0,  0,  0, 43,  0,  0,  0,  0,  0,  0, 43,
        43,  0,  0,  0,  0,  0, 43,  0,  0,  0,  0,  0,  0, 43,
        43,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 43,
        43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43,
    };

    Texture2D mBgTexture       = {};
    Texture2D mBulletTexture   = {};

    Vector2 mBulletPos         = {};
    Vector2 mBulletVel         = {};
    bool    mBulletActive      = false;
    int     mBounceCount       = 0;
    float   mShootTimer        = 0.0f;

    int   mBulletsRemaining    = 0;
    float mMovingTargetDir     = 1.0f;

    ShaderProgram    mLightShader;
    RenderTexture2D  mRenderTarget = {};

    Sound mGunshotSound        = {};
    Sound mBounceSound         = {};
    Sound mTargetHitSound      = {};
    Sound mLevelCompleteSound  = {};
    Sound mLevelFailedSound    = {};

    float mEndTimer  = 0.0f;
    bool  mLevelDone = false;
    bool  mLevelWon  = false;

public:
    static constexpr float TILE_DIMENSION       = 64.0f,
                           PLAYER_SCALE         = 96.0f,
                           TARGET_SCALE         = 72.0f,
                           BULLET_SPEED         = 700.0f,
                           SHOOT_FLASH          = 0.12f,
                           END_DELAY            = 3.0f,
                           MOVING_TARGET_SPEED  = 90.0f;

    Level1();
    Level1(Vector2 origin, const char *bgHexCode);
    ~Level1();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif