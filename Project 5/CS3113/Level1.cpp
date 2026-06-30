#include "Level1.h"

extern Music gBgm;

Level1::Level1()                                      : Scene{ {0.0f}, nullptr   } {}
Level1::Level1(Vector2 origin, const char *bgHexCode) : Scene{ origin, bgHexCode } {}
Level1::~Level1() { shutdown(); }

void Level1::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
    mGameState.lives       = 3;

    mBulletActive     = false;
    mBulletPos        = { 0.0f, 0.0f };
    mBulletVel        = { 0.0f, 0.0f };
    mBounceCount      = 0;
    mShootTimer       = 0.0f;
    mBulletsRemaining = LEVEL_1_MAX_BULLETS;
    mMovingTargetDir  = 1.0f;
    mEndTimer         = 0.0f;
    mLevelDone        = false;
    mLevelWon         = false;

    SetMusicVolume(gBgm, 0.10f);

    mGunshotSound       = LoadSound("assets/audio/gunshot.wav");
    mBounceSound        = LoadSound("assets/audio/bullet_hit_wall.wav");
    mTargetHitSound     = LoadSound("assets/audio/target_hit.wav");
    mLevelCompleteSound = LoadSound("assets/audio/level_completed.wav");
    mLevelFailedSound   = LoadSound("assets/audio/level_failed.wav");

    mBgTexture     = LoadTexture("assets/level1_bg.png");
    mBulletTexture = LoadTexture("assets/protagonist/bullet.png");

    mLightShader.load("shaders/vertex.glsl", "shaders/fragment.glsl");
    mRenderTarget = LoadRenderTexture(1000, 600);

    mGameState.map = new Map(
        LEVEL_1_WIDTH, LEVEL_1_HEIGHT,
        (unsigned int *) mLevelData,
        "assets/DesertTilemap16x16.png",
        TILE_DIMENSION,
        14, 15,
        mOrigin
    );

    float left = mGameState.map->getLeftBoundary();
    float top  = mGameState.map->getTopBoundary();

    mGameState.player = new Entity(
        { left + 2.5f * TILE_DIMENSION,
          top  + 2.0f * TILE_DIMENSION - PLAYER_SCALE / 2.0f },
        { PLAYER_SCALE, PLAYER_SCALE },
        "assets/protagonist/attack1_1.png",
        PLAYER
    );
    mGameState.player->setDirection(RIGHT);

    mGameState.enemies    = new Entity[LEVEL_1_TARGET_COUNT];
    mGameState.enemyCount = LEVEL_1_TARGET_COUNT;

    for (int i = 0; i < LEVEL_1_TARGET_COUNT; i++)
    {
        mGameState.enemies[i].setScale({ TARGET_SCALE, TARGET_SCALE });
        mGameState.enemies[i].setColliderDimensions({ TARGET_SCALE, TARGET_SCALE });
        mGameState.enemies[i].setTexture("assets/target.png");
        mGameState.enemies[i].setEntityType(NPC);
    }

    mGameState.enemies[0].setPosition({ left + 12.5f * TILE_DIMENSION, top + 4.5f * TILE_DIMENSION });
    mGameState.enemies[1].setPosition({ left + 11.5f * TILE_DIMENSION, top + 6.5f * TILE_DIMENSION });
    mGameState.enemies[2].setPosition({ left +  3.5f * TILE_DIMENSION, top + 4.5f * TILE_DIMENSION });
}

void Level1::update(float deltaTime)
{
    if (mLevelDone)
    {
        mEndTimer -= deltaTime;
        if (mEndTimer <= 0.0f)
            mGameState.nextSceneID = mLevelWon ? LEVEL_2_INTRO : GAME_OVER_SCREEN;
        return;
    }

    if (mShootTimer > 0.0f)
    {
        mShootTimer -= deltaTime;
        if (mShootTimer <= 0.0f)
            mGameState.player->setTexture("assets/protagonist/attack1_1.png");
    }

    if (IsKeyPressed(KEY_SPACE) && mBulletsRemaining > 0)
    {
        Vector2 playerPos = mGameState.player->getPosition();
        Vector2 mousePos  = GetMousePosition();
        float   dx        = mousePos.x - playerPos.x;
        float   dy        = mousePos.y - playerPos.y;
        float   len       = sqrtf(dx * dx + dy * dy);

        if (len > 1.0f)
        {
            mBulletVel    = { dx / len * BULLET_SPEED, dy / len * BULLET_SPEED };
            mBulletPos    = playerPos;
            mBulletActive = true;
            mBounceCount  = 0;
            mBulletsRemaining--;
            mShootTimer   = SHOOT_FLASH;
            PlaySound(mGunshotSound);
            mGameState.player->setTexture("assets/protagonist/attack1_2.png");
        }
    }

    if (mBulletActive)
    {
        float xOlap   = 0.0f, yOlap = 0.0f;
        bool  bounced = false;

        float newX = mBulletPos.x + mBulletVel.x * deltaTime;
        if (mGameState.map->isSolidTileAt({ newX, mBulletPos.y }, &xOlap, &yOlap))
        {
            mBulletVel.x = -mBulletVel.x;
            newX         = mBulletPos.x;
            bounced      = true;
        }
        mBulletPos.x = newX;

        float newY = mBulletPos.y + mBulletVel.y * deltaTime;
        if (mGameState.map->isSolidTileAt({ mBulletPos.x, newY }, &xOlap, &yOlap))
        {
            mBulletVel.y = -mBulletVel.y;
            newY         = mBulletPos.y;
            bounced      = true;
        }
        mBulletPos.y = newY;

        if (bounced)
        {
            PlaySound(mBounceSound);
            mBounceCount++;
            if (mBounceCount > LEVEL_1_MAX_BOUNCES)
            {
                mBulletActive = false;
            }
        }

        float halfTarget = TARGET_SCALE * 0.45f;
        for (int i = 0; i < LEVEL_1_TARGET_COUNT && mBulletActive; i++)
        {
            if (!mGameState.enemies[i].isActive()) continue;

            Vector2 tp = mGameState.enemies[i].getPosition();
            if (mBulletPos.x > tp.x - halfTarget && mBulletPos.x < tp.x + halfTarget &&
                mBulletPos.y > tp.y - halfTarget && mBulletPos.y < tp.y + halfTarget)
            {
                mGameState.enemies[i].deactivate();
                mBulletActive = false;
                PlaySound(mTargetHitSound);
            }
        }
    }

    if (mGameState.enemies[2].isActive())
    {
        float   left = mGameState.map->getLeftBoundary();
        Vector2 pos  = mGameState.enemies[2].getPosition();
        pos.x       += mMovingTargetDir * MOVING_TARGET_SPEED * deltaTime;

        float leftStop  = left + TILE_DIMENSION        + TARGET_SCALE / 2.0f;
        float rightStop = left + 6.0f * TILE_DIMENSION - TARGET_SCALE / 2.0f;

        if (pos.x <= leftStop)  { mMovingTargetDir =  1.0f; pos.x = leftStop;  }
        if (pos.x >= rightStop) { mMovingTargetDir = -1.0f; pos.x = rightStop; }

        mGameState.enemies[2].setPosition(pos);
    }

    bool allHit = true;
    for (int i = 0; i < LEVEL_1_TARGET_COUNT; i++)
        if (mGameState.enemies[i].isActive()) { allHit = false; break; }

    if (allHit)
    {
        mLevelDone = true;
        mLevelWon  = true;
        mEndTimer  = END_DELAY;
        PlaySound(mLevelCompleteSound);
        return;
    }

    if (mBulletsRemaining == 0 && !mBulletActive)
    {
        mLevelDone = true;
        mLevelWon  = false;
        mEndTimer  = END_DELAY;
        PlaySound(mLevelFailedSound);
    }
}

void Level1::render()
{
    BeginTextureMode(mRenderTarget);
        ClearBackground(BLACK);

        DrawTexturePro(
            mBgTexture,
            { 0, 0, (float)mBgTexture.width, (float)mBgTexture.height },
            { 0, 0, 1000, 600 },
            { 0, 0 }, 0.0f, WHITE
        );

        mGameState.map->render();

        for (int i = 0; i < LEVEL_1_TARGET_COUNT; i++)
            if (mGameState.enemies[i].isActive())
                mGameState.enemies[i].render();

        mGameState.player->render();

        if (mBulletActive)
            DrawTextureEx(
                mBulletTexture,
                { mBulletPos.x - mBulletTexture.width  * 1.5f,
                  mBulletPos.y - mBulletTexture.height * 1.5f },
                0.0f, 3.0f, WHITE
            );

        if (!mLevelDone && mBulletsRemaining > 0)
        {
            Vector2 playerPos = mGameState.player->getPosition();
            Vector2 mousePos  = GetMousePosition();
            float   dx        = mousePos.x - playerPos.x;
            float   dy        = mousePos.y - playerPos.y;
            float   len       = sqrtf(dx * dx + dy * dy);

            if (len > 0.0f)
            {
                float nx = dx / len;
                float ny = dy / len;

                for (int i = 1; i <= 12; i++)
                {
                    float t     = (float)i / 12.0f;
                    float alpha = 1.0f - t * 0.8f;
                    DrawCircle(
                        (int)(playerPos.x + nx * 220.0f * t),
                        (int)(playerPos.y + ny * 220.0f * t),
                        3.5f, Fade(YELLOW, alpha)
                    );
                }
            }
        }
    EndTextureMode();

    Vector2 mousePos = GetMousePosition();
    mLightShader.setVector2("lightPosition", mousePos);
    mLightShader.begin();
        DrawTexturePro(
            mRenderTarget.texture,
            { 0.0f, 0.0f, 1000.0f, -600.0f },
            { 0.0f, 0.0f, 1000.0f,  600.0f },
            { 0.0f, 0.0f }, 0.0f, WHITE
        );
    mLightShader.end();

    DrawText("SHOTS", 70, 46, 18, LIGHTGRAY);
    for (int i = 0; i < LEVEL_1_MAX_BULLETS; i++)
    {
        Color c = (i < mBulletsRemaining) ? GOLD : Fade(WHITE, 0.25f);
        DrawCircle(74 + i * 22, 76, 8, c);
    }

    int targetsLeft = 0;
    for (int i = 0; i < LEVEL_1_TARGET_COUNT; i++)
        if (mGameState.enemies[i].isActive()) targetsLeft++;
    DrawText(TextFormat("TARGETS  %d", targetsLeft), 840, 58, 22, YELLOW);

    if (mLevelDone)
    {
        const char *msg  = mLevelWon ? "LEVEL COMPLETE!" : "OUT OF AMMO!";
        Color        col = mLevelWon ? GOLD : RED;
        int          sz  = 52;
        DrawRectangle(0, (int)mOrigin.y - 50, 1000, 100, { 0, 0, 0, 180 });
        DrawText(msg, (int)mOrigin.x - MeasureText(msg, sz) / 2, (int)mOrigin.y - sz / 2, sz, col);
    }
}

void Level1::shutdown()
{
    delete mGameState.player;
    mGameState.player = nullptr;

    delete[] mGameState.enemies;
    mGameState.enemies    = nullptr;
    mGameState.enemyCount = 0;

    delete mGameState.map;
    mGameState.map = nullptr;

    UnloadTexture(mBgTexture);
    mBgTexture = {};
    UnloadTexture(mBulletTexture);
    mBulletTexture = {};
    UnloadRenderTexture(mRenderTarget);
    mLightShader.unload();

    UnloadSound(mGunshotSound);
    mGunshotSound = {};
    UnloadSound(mBounceSound);
    mBounceSound = {};
    UnloadSound(mTargetHitSound);
    mTargetHitSound = {};
    UnloadSound(mLevelCompleteSound);
    mLevelCompleteSound = {};
    UnloadSound(mLevelFailedSound);
    mLevelFailedSound = {};
}