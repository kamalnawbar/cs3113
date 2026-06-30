#ifndef LEVEL3_H
#define LEVEL3_H

#include "Scene.h"

constexpr int L3_PROTECTED_COUNT = 6;
constexpr int L3_MAX_WOLVES      = 15;
constexpr int L3_MAX_BEARS       = 12;
constexpr int L3_MAX_BIRDS       = 6;
constexpr int L3_MAX_BULLETS     = 4;

struct Bullet3
{
    Vector2 pos, vel;
    bool    active;
};

class Level3 : public Scene
{
private:
    Texture2D mBgTexture             = {};
    Texture2D mIdleFrames[6]         = {};
    Texture2D mAttackFrames[4]       = {};
    Texture2D mReloadFrames[4]       = {};
    Texture2D mBulletTexture         = {};

    Entity *mProtected[L3_PROTECTED_COUNT] = {};
    Entity *mWolves = nullptr;
    Entity *mBears  = nullptr;
    Entity *mBirds  = nullptr;

    Bullet3 mBullets[L3_MAX_BULLETS] = {};

    float mPlayerAnimTimer           = 0.0f;
    int   mPlayerFrameIndex          = 0;
    int   mPlayerState               = 0;
    float mPlayerStateTimer          = 0.0f;
    int   mAmmo                      = 6;
    float mReloadTimer               = 0.0f;

    int   mCurrentWave               = 0;
    int   mWolvesSpawned             = 0;
    int   mBearsSpawned              = 0;
    int   mBirdsSpawned              = 0;
    int   mPacksSpawned              = 0;
    float mSpawnTimer                = 0.0f;
    bool  mWaveTransitioning         = false;
    float mWaveTransitionTimer       = 0.0f;

    bool  mLevelDone                 = false;
    bool  mLevelWon                  = false;
    float mEndTimer                  = 0.0f;

    Sound mGunshotSound              = {};
    Sound mReloadSound               = {};
    Sound mHitSound                  = {};
    Sound mAnimalHitSound            = {};
    Sound mLevelCompleteSound        = {};
    Sound mLevelFailedSound          = {};

    int   mPlayerLives               = 3;
    float mPlayerInvincibleTimer     = 0.0f;
    float mW3WolfTimer               = 0.0f;
    float mW3BearTimer               = 0.0f;
    float mW3BirdTimer               = 0.0f;

    int     countActive(Entity *arr, int count);
    void    spawnWolfPack();
    void    spawnBear();
    void    spawnBird();
    bool    hitsPlayer(Entity &e, float hw, float hh);
    void    hitPlayer();
    void    setupProtected(int idx, const char *tex, int cols, int frames,
                           float scaleW, float scaleH, float x, float y);
    void    setupEnemy(Entity &e, const char *tex, int cols, int frames,
                       float scaleW, float scaleH, int speed, AIType ai,
                       float amplitude = 0.0f);

public:
    static constexpr float PLAYER_SPEED       = 200.0f;
    static constexpr float PLAYER_SCALE       = 80.0f;
    static constexpr float PLAYER_ANIM_SPD    = 0.10f;
    static constexpr float BULLET_SPEED       = 680.0f;
    static constexpr float RELOAD_TIME        = 1.5f;
    static constexpr float ATTACK_FLASH       = 0.15f;
    static constexpr int   MAX_AMMO           = 6;
    static constexpr float END_DELAY          = 3.0f;
    static constexpr float FENCE_X            = 160.0f;
    static constexpr float GROUND_Y           = 470.0f;
    static constexpr float BEAR_GROUND_Y      = 481.0f;
    static constexpr int   MAX_PLAYER_LIVES   = 3;
    static constexpr float PLAYER_INVINCIBLE  = 1.4f;

    static constexpr float WOLF_SCALE_W       = 90.0f;
    static constexpr float WOLF_SCALE_H       = 65.0f;
    static constexpr float BEAR_SCALE_W       = 110.0f;
    static constexpr float BEAR_SCALE_H       = 58.0f;
    static constexpr float BIRD_SCALE_W       = 100.0f;
    static constexpr float BIRD_SCALE_H       = 36.0f;
    static constexpr float COW_SCALE_W        = 65.0f;
    static constexpr float COW_SCALE_H        = 65.0f;
    static constexpr float SHEEP_SCALE_W      = 50.0f;
    static constexpr float SHEEP_SCALE_H      = 42.0f;
    static constexpr float CHICKEN_SCALE_W    = 40.0f;
    static constexpr float CHICKEN_SCALE_H    = 44.0f;

    static constexpr int   W1_PACKS           = 4;
    static constexpr int   W1_PACK_SIZE       = 5;
    static constexpr int   W2_TOTAL_BEARS     = 8;
    static constexpr int   W3_WOLF_PACKS      = 2;
    static constexpr int   W3_BEARS           = 4;
    static constexpr int   W3_BIRDS           = 3;

    Level3();
    Level3(Vector2 origin, const char *bgHexCode);
    ~Level3();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif