#ifndef LEVEL2_H
#define LEVEL2_H

#include "Scene.h"

constexpr int LEVEL_2_WIDTH  = 40;
constexpr int LEVEL_2_HEIGHT = 8;
constexpr int HORSE_COUNT    = 16;

struct HorseData
{
    float x, y;
    float speed;
    float animTimer;
    int   frameIndex;
    int   lane;
};

class Level2 : public Scene
{
private:
    unsigned int mLevelData[LEVEL_2_WIDTH * LEVEL_2_HEIGHT] = {
        43,43,43,43,43,43,43,43,43,43, 43,43,43,43,43,43,43,43,43,43, 43,43,43,43,43,43,43,43,43,43, 43,43,43,43,43,43,43,43,43,43,
        43, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43,43,43,43,43,43,43,43,43,43, 43,43,43,43,43,43,43,43,43,43, 43,43,43,43,43,43,43,43,43,43,
        43, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0,43,
        43, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0,43,
        43, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0,43,
        43, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0,43,
        43, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43,43,43,43,43,43,43,43,43,43,
        43,43,43,43,43,43,43,43,43,43, 43,43,43,43,43,43,43,43,43,43, 43,43,43,43,43,43,43,43,43,43, 43,43,43,43,43,43,43,43,43,43,
    };

    Texture2D mBgTexture           = {};
    Texture2D mIdleFrames[6]       = {};
    Texture2D mMoveFrames[6]       = {};
    Texture2D mHorseSpriteSheet    = {};

    HorseData mHorses[HORSE_COUNT] = {};

    float mPlayerAnimTimer         = 0.0f;
    int   mPlayerFrameIndex        = 0;
    bool  mPlayerMoving            = false;
    bool  mPlayerFacingRight       = true;

    float mInvincibleTimer         = 0.0f;
    int   mLives                   = 3;
    bool  mLevelDone               = false;
    bool  mLevelWon                = false;
    float mEndTimer                = 0.0f;

    Sound mGallopSound             = {};
    Sound mHitSound                = {};

public:
    static constexpr float TILE_DIMENSION  = 64.0f;
    static constexpr float PLAYER_SCALE    = 72.0f;
    static constexpr float PLAYER_SPEED    = 220.0f;
    static constexpr float PLAYER_ANIM_SPD = 0.10f;
    static constexpr float HORSE_DISPLAY_W = 84.0f;
    static constexpr float HORSE_DISPLAY_H = 60.0f;
    static constexpr float HORSE_FRAME_W   = 172.0f;
    static constexpr float HORSE_FRAME_H   = 145.0f;
    static constexpr float HORSE_ANIM_SPD  = 0.10f;
    static constexpr float LANE_1_Y        = 205.0f;
    static constexpr float LANE_2_Y        = 300.0f;
    static constexpr float LANE_3_Y        = 390.0f;
    static constexpr float LANE_4_Y        = 460.0f;
    static constexpr float LANE_1_SPEED    = 130.0f;
    static constexpr float LANE_2_SPEED    = 90.0f;
    static constexpr float LANE_3_SPEED    = 155.0f;
    static constexpr float LANE_4_SPEED    = 110.0f;
    static constexpr float INVINCIBLE_TIME = 1.2f;
    static constexpr float END_DELAY       = 3.0f;
    static constexpr float EXIT_MARGIN     = 2.0f;

    Level2();
    Level2(Vector2 origin, const char *bgHexCode);
    ~Level2();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif