#include "Level3.h"

extern Music gBgm;

Level3::Level3()                                      : Scene{ {0.0f}, nullptr   } {}
Level3::Level3(Vector2 origin, const char *bgHexCode) : Scene{ origin, bgHexCode } {}
Level3::~Level3() { shutdown(); }

void Level3::setupProtected(int idx, const char *tex, int cols, int frames,
                             float scaleW, float scaleH, float x, float y)
{
    std::vector<int> indices;
    for (int i = 0; i < frames; i++) indices.push_back(i);
    std::map<Direction, std::vector<int>> atlas;
    atlas[LEFT] = atlas[RIGHT] = atlas[UP] = atlas[DOWN] = indices;

    mProtected[idx] = new Entity();
    mProtected[idx]->setTexture(tex);
    mProtected[idx]->setTextureType(ATLAS);
    mProtected[idx]->setSpriteSheetDimensions({ 1.0f, (float)cols });
    mProtected[idx]->setAnimationAtlas(atlas);
    mProtected[idx]->setScale({ scaleW, scaleH });
    mProtected[idx]->setPosition({ x, y });
    mProtected[idx]->setEntityType(BLOCK);
    mProtected[idx]->setFrameSpeed(6);
    mProtected[idx]->setAutoAnimate(true);
    mProtected[idx]->setDirection(RIGHT);
}

void Level3::setupEnemy(Entity &e, const char *tex, int cols, int frames,
                        float scaleW, float scaleH, int speed, AIType ai,
                        float amplitude)
{
    std::vector<int> indices;
    for (int i = 0; i < frames; i++) indices.push_back(i);
    std::map<Direction, std::vector<int>> atlas;
    atlas[LEFT] = atlas[RIGHT] = atlas[UP] = atlas[DOWN] = indices;

    e.setTexture(tex);
    e.setTextureType(ATLAS);
    e.setSpriteSheetDimensions({ 1.0f, (float)cols });
    e.setAnimationAtlas(atlas);
    e.setScale({ scaleW, scaleH });
    e.setColliderDimensions({ scaleW * 0.5f, scaleH * 0.5f });
    e.setEntityType(NPC);
    e.setAIType(ai);
    e.setAIState(IDLE);
    e.setSpeed(speed);
    e.setFrameSpeed(8);
    e.setFlipX(true);
    e.setAcceleration({ 0.0f, 0.0f });
    e.setFlightAmplitude(amplitude);
    e.deactivate();
    e.setDirection(LEFT);
}

void Level3::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
    mGameState.player      = nullptr;
    mGameState.enemies     = nullptr;
    mGameState.enemyCount  = 0;
    mGameState.map         = nullptr;
    mGameState.lives       = 3;

    mPlayerAnimTimer      = 0.0f;
    mPlayerFrameIndex     = 0;
    mPlayerState          = 0;
    mPlayerStateTimer     = 0.0f;
    mAmmo                 = MAX_AMMO;
    mReloadTimer          = 0.0f;
    mPlayerLives          = 3;
    mPlayerInvincibleTimer = 0.0f;
    mCurrentWave          = 0;
    mWolvesSpawned        = 0;
    mBearsSpawned         = 0;
    mBirdsSpawned         = 0;
    mPacksSpawned         = 0;
    mSpawnTimer           = 0.0f;
    mW3WolfTimer          = 0.0f;
    mW3BearTimer          = 2.5f;
    mW3BirdTimer          = 4.0f;
    mWaveTransitioning    = true;
    mWaveTransitionTimer  = 2.5f;
    mLevelDone            = false;
    mLevelWon             = false;
    mEndTimer             = 0.0f;

    for (int i = 0; i < L3_MAX_BULLETS; i++) mBullets[i].active = false;

    SetMusicVolume(gBgm, 0.13f);

    mGunshotSound       = LoadSound("assets/audio/gunshot.wav");
    mReloadSound        = LoadSound("assets/audio/reload.wav");
    mHitSound           = LoadSound("assets/audio/hit.wav");
    mAnimalHitSound     = LoadSound("assets/audio/target_hit.wav");
    mLevelCompleteSound = LoadSound("assets/audio/level_completed.wav");
    mLevelFailedSound   = LoadSound("assets/audio/level_failed.wav");

    mBgTexture       = LoadTexture("assets/level3_bg.png");
    mBulletTexture   = LoadTexture("assets/protagonist/bullet.png");

    mIdleFrames[0]   = LoadTexture("assets/protagonist/idle1.png");
    mIdleFrames[1]   = LoadTexture("assets/protagonist/idle2.png");
    mIdleFrames[2]   = LoadTexture("assets/protagonist/idle3.png");
    mIdleFrames[3]   = LoadTexture("assets/protagonist/idle4.png");
    mIdleFrames[4]   = LoadTexture("assets/protagonist/idle5.png");
    mIdleFrames[5]   = LoadTexture("assets/protagonist/idle6.png");
    mAttackFrames[0] = LoadTexture("assets/protagonist/attack1_1.png");
    mAttackFrames[1] = LoadTexture("assets/protagonist/attack1_2.png");
    mAttackFrames[2] = LoadTexture("assets/protagonist/attack1_3.png");
    mAttackFrames[3] = LoadTexture("assets/protagonist/attack1_4.png");
    mReloadFrames[0] = LoadTexture("assets/protagonist/reload1_1.png");
    mReloadFrames[1] = LoadTexture("assets/protagonist/reload1_2.png");
    mReloadFrames[2] = LoadTexture("assets/protagonist/reload1_3.png");
    mReloadFrames[3] = LoadTexture("assets/protagonist/reload1_4.png");

    mGameState.player = new Entity();
    mGameState.player->setPosition({ 500.0f, GROUND_Y });
    mGameState.player->setScale({ PLAYER_SCALE, PLAYER_SCALE });
    mGameState.player->setEntityType(PLAYER);
    mGameState.player->setAcceleration({ 0.0f, 0.0f });

    setupProtected(0, "assets/Cow.png",     7, 7, COW_SCALE_W,     COW_SCALE_H,      55.0f, 130.0f);
    setupProtected(1, "assets/Cow.png",     7, 7, COW_SCALE_W,     COW_SCALE_H,     110.0f, 460.0f);
    setupProtected(2, "assets/sheep.png",   6, 6, SHEEP_SCALE_W,   SHEEP_SCALE_H,    50.0f, 270.0f);
    setupProtected(3, "assets/sheep.png",   6, 6, SHEEP_SCALE_W,   SHEEP_SCALE_H,   115.0f, 370.0f);
    setupProtected(4, "assets/Chicken.png", 4, 4, CHICKEN_SCALE_W, CHICKEN_SCALE_H,  80.0f, 200.0f);
    setupProtected(5, "assets/Chicken.png", 4, 4, CHICKEN_SCALE_W, CHICKEN_SCALE_H,  80.0f, 520.0f);

    mWolves = new Entity[L3_MAX_WOLVES];
    mBears  = new Entity[L3_MAX_BEARS];
    mBirds  = new Entity[L3_MAX_BIRDS];

    for (int i = 0; i < L3_MAX_WOLVES; i++)
    {
        setupEnemy(mWolves[i], "assets/wolf.png", 3, 3,
                   WOLF_SCALE_W, WOLF_SCALE_H, 125, WANDERER);
        mWolves[i].setFlipX(false);
    }

    for (int i = 0; i < L3_MAX_BEARS; i++)
        setupEnemy(mBears[i], "assets/bear.png", 4, 4,
                   BEAR_SCALE_W, BEAR_SCALE_H, 58, WANDERER);

    for (int i = 0; i < L3_MAX_BIRDS; i++)
        setupEnemy(mBirds[i], "assets/bird.png", 4, 4,
                   BIRD_SCALE_W, BIRD_SCALE_H, 140, FLYER, 70.0f);
}

int Level3::countActive(Entity *arr, int count)
{
    int n = 0;
    for (int i = 0; i < count; i++)
        if (arr[i].isActive()) n++;
    return n;
}

bool Level3::hitsPlayer(Entity &e, float hw, float hh)
{
    if (mPlayerInvincibleTimer > 0.0f) return false;
    Vector2 pp = mGameState.player->getPosition();
    float   pw = PLAYER_SCALE * 0.30f;
    float   ph = PLAYER_SCALE * 0.36f;
    return fabsf(e.getPosition().x - pp.x) < hw + pw &&
           fabsf(e.getPosition().y - pp.y) < hh + ph;
}

void Level3::spawnWolfPack()
{
    float zoneMin[4] = { 80.0f, 340.0f, 180.0f, 420.0f };
    float zoneMax[4] = { 160.0f, 420.0f, 270.0f, 510.0f };
    int   zone       = mPacksSpawned % 4;
    float packY      = zoneMin[zone] + (float)(rand() % (int)(zoneMax[zone] - zoneMin[zone]));

    int spawned = 0;
    for (int i = 0; i < L3_MAX_WOLVES && spawned < W1_PACK_SIZE; i++)
    {
        if (mWolves[i].isActive()) continue;
        float yOff = (spawned - W1_PACK_SIZE / 2) * 18.0f;
        float xOff = spawned * 32.0f;
        float y    = fmaxf(80.0f, fminf(520.0f, packY + yOff));
        mWolves[i].activate();
        mWolves[i].setPosition({ 1060.0f + xOff, y });
        mWolves[i].setHealth(1);
        mWolves[i].setAIState(WALKING);
        spawned++;
        mWolvesSpawned++;
    }
    mPacksSpawned++;
}

void Level3::spawnBear()
{
    for (int i = 0; i < L3_MAX_BEARS; i++)
    {
        if (mBears[i].isActive()) continue;
        float xOff = (float)(rand() % 100);
        mBears[i].activate();
        mBears[i].setPosition({ 1060.0f + xOff, BEAR_GROUND_Y });
        mBears[i].setHealth(2);
        mBears[i].setAIState(WALKING);
        mBears[i].setSpeed(58);
        mBearsSpawned++;
        return;
    }
}

void Level3::spawnBird()
{
    for (int i = 0; i < L3_MAX_BIRDS; i++)
    {
        if (mBirds[i].isActive()) continue;
        float y = 160.0f + (float)(rand() % 280);
        mBirds[i].activate();
        mBirds[i].setPosition({ 1060.0f, y });
        mBirds[i].setBaseY(y);
        mBirds[i].setFlightTimer(0.0f);
        mBirds[i].setHealth(1);
        mBirdsSpawned++;
        return;
    }
}

void Level3::hitPlayer()
{
    mPlayerLives--;
    mPlayerInvincibleTimer = PLAYER_INVINCIBLE;
    PlaySound(mHitSound);
    if (mPlayerLives <= 0 && !mLevelDone)
    {
        mLevelDone = true;
        mLevelWon  = false;
        mEndTimer  = END_DELAY;
        PlaySound(mLevelFailedSound);
    }
}

void Level3::update(float deltaTime)
{
    if (mLevelDone)
    {
        mEndTimer -= deltaTime;
        if (mEndTimer <= 0.0f)
            mGameState.nextSceneID = mLevelWon ? WIN_SCREEN : GAME_OVER_SCREEN;
        return;
    }

    if (mPlayerInvincibleTimer > 0.0f) mPlayerInvincibleTimer -= deltaTime;

    if (mWaveTransitioning)
    {
        mWaveTransitionTimer -= deltaTime;
        if (mWaveTransitionTimer <= 0.0f)
        {
            mWaveTransitioning  = false;
            mWolvesSpawned      = 0;
            mBearsSpawned       = 0;
            mBirdsSpawned       = 0;
            mPacksSpawned       = 0;
            mSpawnTimer         = 0.0f;
            mW3WolfTimer        = 0.0f;
            mW3BearTimer        = 2.5f;
            mW3BirdTimer        = 4.0f;
        }
        return;
    }

    int wolvesAlive = countActive(mWolves, L3_MAX_WOLVES);
    int bearsAlive  = countActive(mBears,  L3_MAX_BEARS);
    int birdsAlive  = countActive(mBirds,  L3_MAX_BIRDS);

    mSpawnTimer -= deltaTime;

    if (mCurrentWave == 0)
    {
        if (mSpawnTimer <= 0.0f && mPacksSpawned < W1_PACKS && wolvesAlive <= 2)
        {
            spawnWolfPack();
            mSpawnTimer = 0.8f;
        }
        if (mPacksSpawned >= W1_PACKS && wolvesAlive == 0)
        {
            mCurrentWave         = 1;
            mWaveTransitioning   = true;
            mWaveTransitionTimer = 3.0f;
        }
    }
    else if (mCurrentWave == 1)
    {
        if (mSpawnTimer <= 0.0f && mBearsSpawned < W2_TOTAL_BEARS)
        {
            spawnBear();
            mSpawnTimer = 4.0f;
        }
        if (mBearsSpawned >= W2_TOTAL_BEARS && bearsAlive == 0)
        {
            mCurrentWave         = 2;
            mWaveTransitioning   = true;
            mWaveTransitionTimer = 3.0f;
        }
    }
    else if (mCurrentWave == 2)
    {
        mW3WolfTimer -= deltaTime;
        mW3BearTimer -= deltaTime;
        mW3BirdTimer -= deltaTime;

        if (mW3WolfTimer <= 0.0f && mPacksSpawned < W3_WOLF_PACKS)
        {
            spawnWolfPack();
            mW3WolfTimer = 5.5f;
        }
        if (mW3BearTimer <= 0.0f && mBearsSpawned < W3_BEARS)
        {
            spawnBear();
            mW3BearTimer = 5.0f;
        }
        if (mW3BirdTimer <= 0.0f && mBirdsSpawned < W3_BIRDS)
        {
            spawnBird();
            mW3BirdTimer = 4.0f;
        }

        if (mPacksSpawned >= W3_WOLF_PACKS && mBearsSpawned >= W3_BEARS &&
            mBirdsSpawned >= W3_BIRDS &&
            wolvesAlive == 0 && bearsAlive == 0 && birdsAlive == 0)
        {
            mLevelDone = true;
            mLevelWon  = true;
            mEndTimer  = END_DELAY;
            PlaySound(mLevelCompleteSound);
            return;
        }
    }

    Vector2 pos = mGameState.player->getPosition();
    if (IsKeyDown(KEY_W)) pos.y -= PLAYER_SPEED * deltaTime;
    if (IsKeyDown(KEY_S)) pos.y += PLAYER_SPEED * deltaTime;
    if (IsKeyDown(KEY_D)) pos.x += PLAYER_SPEED * deltaTime;
    if (IsKeyDown(KEY_A)) pos.x -= PLAYER_SPEED * deltaTime;
    pos.x = fmaxf(FENCE_X + 20.0f, fminf(920.0f, pos.x));
    pos.y = fmaxf(60.0f,            fminf(GROUND_Y, pos.y));
    mGameState.player->setPosition(pos);

    if (mReloadTimer > 0.0f)
    {
        mReloadTimer -= deltaTime;
        mPlayerState  = 2;
        if (mReloadTimer <= 0.0f) { mAmmo = MAX_AMMO; mPlayerState = 0; }
    }
    else if (mPlayerStateTimer > 0.0f)
    {
        mPlayerStateTimer -= deltaTime;
        if (mPlayerStateTimer <= 0.0f) mPlayerState = 0;
    }

    if (IsKeyPressed(KEY_SPACE) && mReloadTimer <= 0.0f && mAmmo > 0)
    {
        for (int i = 0; i < L3_MAX_BULLETS; i++)
        {
            if (mBullets[i].active) continue;
            Vector2 mouse = GetMousePosition();
            float   dx    = mouse.x - pos.x;
            float   dy    = mouse.y - pos.y;
            float   len   = sqrtf(dx * dx + dy * dy);
            if (len > 1.0f)
            {
                mBullets[i].pos    = pos;
                mBullets[i].vel    = { dx / len * BULLET_SPEED, dy / len * BULLET_SPEED };
                mBullets[i].active = true;
                mAmmo--;
                mPlayerState      = 1;
                mPlayerStateTimer = ATTACK_FLASH;
                PlaySound(mGunshotSound);
            }
            break;
        }
    }

    if (mAmmo == 0 && mReloadTimer <= 0.0f)
    {
        mReloadTimer = RELOAD_TIME;
        mPlayerState = 2;
        PlaySound(mReloadSound);
    }

    mPlayerAnimTimer += deltaTime;
    if (mPlayerAnimTimer >= PLAYER_ANIM_SPD)
    {
        mPlayerAnimTimer  = 0.0f;
        int frameCount    = (mPlayerState == 0) ? 6 : 4;
        mPlayerFrameIndex = (mPlayerFrameIndex + 1) % frameCount;
    }

    for (int i = 0; i < L3_MAX_BULLETS; i++)
    {
        if (!mBullets[i].active) continue;
        mBullets[i].pos.x += mBullets[i].vel.x * deltaTime;
        mBullets[i].pos.y += mBullets[i].vel.y * deltaTime;
        if (mBullets[i].pos.x > 1060 || mBullets[i].pos.x < -50 ||
            mBullets[i].pos.y < -50  || mBullets[i].pos.y > 660)
            mBullets[i].active = false;
    }

    for (int i = 0; i < L3_MAX_WOLVES; i++)
    {
        if (!mWolves[i].isActive()) continue;
        mWolves[i].update(deltaTime, nullptr, nullptr, nullptr, 0);

        for (int j = 0; j < L3_MAX_BULLETS; j++)
        {
            if (!mBullets[j].active) continue;
            Vector2 wp = mWolves[i].getPosition();
            if (fabsf(mBullets[j].pos.x - wp.x) < WOLF_SCALE_W * 0.32f &&
                fabsf(mBullets[j].pos.y - wp.y) < WOLF_SCALE_H * 0.32f)
            {
                mBullets[j].active = false;
                mWolves[i].takeDamage();
            }
        }

        if (mWolves[i].isActive() && hitsPlayer(mWolves[i], WOLF_SCALE_W * 0.30f, WOLF_SCALE_H * 0.30f))
        {
            mWolves[i].deactivate();
            hitPlayer();
        }

        if (mWolves[i].isActive() && mWolves[i].getPosition().x < FENCE_X)
        {
            mWolves[i].deactivate();
            hitPlayer();
        }
    }

    for (int i = 0; i < L3_MAX_BEARS; i++)
    {
        if (!mBears[i].isActive()) continue;
        mBears[i].update(deltaTime, nullptr, nullptr, nullptr, 0);

        for (int j = 0; j < L3_MAX_BULLETS; j++)
        {
            if (!mBullets[j].active) continue;
            Vector2 bp = mBears[i].getPosition();
            if (fabsf(mBullets[j].pos.x - bp.x) < BEAR_SCALE_W * 0.32f &&
                fabsf(mBullets[j].pos.y - bp.y) < BEAR_SCALE_H * 0.32f)
            {
                mBullets[j].active = false;
                mBears[i].takeDamage();
            }
        }

        if (mBears[i].isActive() && hitsPlayer(mBears[i], BEAR_SCALE_W * 0.30f, BEAR_SCALE_H * 0.30f))
        {
            mBears[i].deactivate();
            hitPlayer();
        }

        if (mBears[i].isActive() && mBears[i].getPosition().x < FENCE_X)
        {
            mBears[i].deactivate();
            hitPlayer();
        }
    }

    for (int i = 0; i < L3_MAX_BIRDS; i++)
    {
        if (!mBirds[i].isActive()) continue;
        mBirds[i].update(deltaTime, nullptr, nullptr, nullptr, 0);

        for (int j = 0; j < L3_MAX_BULLETS; j++)
        {
            if (!mBullets[j].active) continue;
            Vector2 bp = mBirds[i].getPosition();
            if (fabsf(mBullets[j].pos.x - bp.x) < BIRD_SCALE_W * 0.40f &&
                fabsf(mBullets[j].pos.y - bp.y) < BIRD_SCALE_H * 0.40f)
            {
                mBullets[j].active = false;
                mBirds[i].takeDamage();
            }
        }

        if (mBirds[i].isActive() && hitsPlayer(mBirds[i], BIRD_SCALE_W * 0.35f, BIRD_SCALE_H * 0.35f))
        {
            mBirds[i].deactivate();
            hitPlayer();
        }

        if (mBirds[i].isActive() && mBirds[i].getPosition().x < -80.0f)
            mBirds[i].deactivate();
    }

    for (int i = 0; i < L3_PROTECTED_COUNT; i++)
        mProtected[i]->animate(deltaTime);
}

void Level3::render()
{
    ClearBackground({ 60, 40, 20, 255 });

    DrawTexturePro(
        mBgTexture,
        { 0, 0, (float)mBgTexture.width, (float)mBgTexture.height },
        { 0, 0, 1000, 600 },
        { 0, 0 }, 0.0f, WHITE
    );

    int groundTopY = (int)(GROUND_Y + PLAYER_SCALE / 2.0f);
    DrawRectangle(0, groundTopY, 1000, 600 - groundTopY, { 110, 72, 30, 255 });
    DrawRectangle(0, groundTopY, 1000, 3, { 70, 44, 12, 255 });
    for (int tx = 1; tx < 16; tx++)
        DrawLine(tx * 64, groundTopY, tx * 64, 600, { 70, 44, 12, 180 });

    DrawRectangle(0, 0, (int)FENCE_X, 600, { 20, 10, 5, 120 });
    DrawLineEx({ FENCE_X, 0 }, { FENCE_X, 600 }, 4.0f, { 139, 90, 43, 255 });

    for (int i = 0; i < L3_PROTECTED_COUNT; i++)
        mProtected[i]->render();

    for (int i = 0; i < L3_MAX_WOLVES; i++)
        if (mWolves[i].isActive()) mWolves[i].render();

    for (int i = 0; i < L3_MAX_BEARS; i++)
        if (mBears[i].isActive()) mBears[i].render();

    for (int i = 0; i < L3_MAX_BIRDS; i++)
        if (mBirds[i].isActive()) mBirds[i].render();

    for (int i = 0; i < L3_MAX_BULLETS; i++)
    {
        if (!mBullets[i].active) continue;
        DrawTextureEx(mBulletTexture,
            { mBullets[i].pos.x - 4.0f, mBullets[i].pos.y - 4.0f },
            0.0f, 2.0f, WHITE);
    }

    Vector2 pos     = mGameState.player->getPosition();
    bool invisible  = mPlayerInvincibleTimer > 0.0f && ((int)(mPlayerInvincibleTimer * 10) % 2 == 0);

    if (!invisible)
    {
        Texture2D *frames = (mPlayerState == 1) ? mAttackFrames :
                            (mPlayerState == 2) ? mReloadFrames : mIdleFrames;
        DrawTexturePro(
            frames[mPlayerFrameIndex],
            { 0, 0, (float)frames[mPlayerFrameIndex].width,
                    (float)frames[mPlayerFrameIndex].height },
            { pos.x, pos.y, PLAYER_SCALE, PLAYER_SCALE },
            { PLAYER_SCALE / 2.0f, PLAYER_SCALE / 2.0f },
            0.0f, WHITE
        );
    }

    DrawText("AMMO", 20, 16, 18, LIGHTGRAY);
    for (int i = 0; i < MAX_AMMO; i++)
    {
        Color c = (i < mAmmo) ? GOLD : Fade(WHITE, 0.25f);
        DrawCircle(26 + i * 22, 48, 8, c);
    }
    if (mReloadTimer > 0.0f)
        DrawText("RELOADING...", 20, 64, 18, ORANGE);

    DrawText("LIVES", 20, 86, 18, LIGHTGRAY);
    for (int i = 0; i < MAX_PLAYER_LIVES; i++)
    {
        Color c = (i < mPlayerLives) ? RED : Fade(WHITE, 0.25f);
        DrawCircle(28 + i * 28, 114, 11, c);
    }

    const char *waveLabel = (mCurrentWave == 0) ? "WAVE 1 — WOLF PACKS" :
                            (mCurrentWave == 1) ? "WAVE 2 — STRONG BEARS" :
                                                  "WAVE 3 — GO CRAZY!";
    DrawText(waveLabel,
        (int)mOrigin.x - MeasureText(waveLabel, 22) / 2,
        16, 22, GOLD);

    if (mWaveTransitioning)
    {
        const char *wNum  = (mCurrentWave == 0) ? "WAVE  1" :
                            (mCurrentWave == 1) ? "WAVE  2" : "WAVE  3";
        const char *wName = (mCurrentWave == 0) ? "WOLF PACKS" :
                            (mCurrentWave == 1) ? "STRONG BEARS" : "GO CRAZY!";
        DrawRectangle(0, (int)mOrigin.y - 110, 1000, 220, { 0, 0, 0, 210 });
        DrawText(wNum,
            (int)mOrigin.x - MeasureText(wNum, 64) / 2,
            (int)mOrigin.y - 100, 64, RED);
        DrawText(wName,
            (int)mOrigin.x - MeasureText(wName, 44) / 2,
            (int)mOrigin.y - 20, 44, GOLD);
    }

    if (mLevelDone)
    {
        const char *msg  = mLevelWon ? "RANCH DEFENDED!" : "THEY BROKE THROUGH!";
        Color        col = mLevelWon ? GOLD : RED;
        int          sz  = 52;
        DrawRectangle(0, (int)mOrigin.y - 50, 1000, 100, { 0, 0, 0, 190 });
        DrawText(msg,
            (int)mOrigin.x - MeasureText(msg, sz) / 2,
            (int)mOrigin.y - sz / 2, sz, col);
    }
}

void Level3::shutdown()
{
    delete mGameState.player;
    mGameState.player = nullptr;

    for (int i = 0; i < L3_PROTECTED_COUNT; i++)
    {
        delete mProtected[i];
        mProtected[i] = nullptr;
    }

    delete[] mWolves;  mWolves = nullptr;
    delete[] mBears;   mBears  = nullptr;
    delete[] mBirds;   mBirds  = nullptr;

    UnloadTexture(mBgTexture);       mBgTexture       = {};
    UnloadTexture(mBulletTexture);   mBulletTexture   = {};
    for (int i = 0; i < 6; i++) { UnloadTexture(mIdleFrames[i]);   mIdleFrames[i]   = {}; }
    for (int i = 0; i < 4; i++) { UnloadTexture(mAttackFrames[i]); mAttackFrames[i] = {}; }
    for (int i = 0; i < 4; i++) { UnloadTexture(mReloadFrames[i]); mReloadFrames[i] = {}; }

    UnloadSound(mGunshotSound);       mGunshotSound       = {};
    UnloadSound(mReloadSound);        mReloadSound        = {};
    UnloadSound(mHitSound);           mHitSound           = {};
    UnloadSound(mAnimalHitSound);     mAnimalHitSound     = {};
    UnloadSound(mLevelCompleteSound); mLevelCompleteSound = {};
    UnloadSound(mLevelFailedSound);   mLevelFailedSound   = {};
}