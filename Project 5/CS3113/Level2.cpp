#include "Level2.h"

extern Camera2D gCamera;
extern Music    gBgm;

Level2::Level2()                                      : Scene{ {0.0f}, nullptr   } {}
Level2::Level2(Vector2 origin, const char *bgHexCode) : Scene{ origin, bgHexCode } {}
Level2::~Level2() { shutdown(); }

void Level2::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
    mGameState.player      = nullptr;
    mGameState.enemies     = nullptr;
    mGameState.enemyCount  = 0;
    mGameState.lives       = 3;

    mPlayerAnimTimer   = 0.0f;
    mPlayerFrameIndex  = 0;
    mPlayerMoving      = false;
    mPlayerFacingRight = true;
    mInvincibleTimer   = 0.0f;
    mLives             = 3;
    mLevelDone         = false;
    mLevelWon          = false;
    mEndTimer          = 0.0f;

    SetMusicVolume(gBgm, 0.16f);

    mGallopSound = LoadSound("assets/audio/gallop.wav");
    mHitSound    = LoadSound("assets/audio/hit.wav");
    SetSoundVolume(mGallopSound, 0.45f);

    mBgTexture        = LoadTexture("assets/level2_bg.png");
    mHorseSpriteSheet = LoadTexture("assets/Pixilart Sprite Sheet.png");

    mIdleFrames[0] = LoadTexture("assets/protagonist/idle1.png");
    mIdleFrames[1] = LoadTexture("assets/protagonist/idle2.png");
    mIdleFrames[2] = LoadTexture("assets/protagonist/idle3.png");
    mIdleFrames[3] = LoadTexture("assets/protagonist/idle4.png");
    mIdleFrames[4] = LoadTexture("assets/protagonist/idle5.png");
    mIdleFrames[5] = LoadTexture("assets/protagonist/idle6.png");

    mMoveFrames[0] = LoadTexture("assets/protagonist/move1.png");
    mMoveFrames[1] = LoadTexture("assets/protagonist/move2.png");
    mMoveFrames[2] = LoadTexture("assets/protagonist/move3.png");
    mMoveFrames[3] = LoadTexture("assets/protagonist/move4.png");
    mMoveFrames[4] = LoadTexture("assets/protagonist/move5.png");
    mMoveFrames[5] = LoadTexture("assets/protagonist/move6.png");

    mGameState.map = new Map(
        LEVEL_2_WIDTH, LEVEL_2_HEIGHT,
        (unsigned int *)mLevelData,
        "assets/DesertTilemap16x16.png",
        TILE_DIMENSION,
        14, 15,
        mOrigin
    );

    float left  = mGameState.map->getLeftBoundary();
    float startX = left + 2.5f * TILE_DIMENSION;

    mGameState.player = new Entity();
    mGameState.player->setPosition({ startX, LANE_4_Y });
    mGameState.player->setScale({ PLAYER_SCALE, PLAYER_SCALE });
    mGameState.player->setEntityType(PLAYER);

    mHorses[0]  = { startX + 300.0f,  LANE_1_Y, LANE_1_SPEED, 0.0f, 0, 0 };
    mHorses[1]  = { startX + 600.0f,  LANE_1_Y, LANE_1_SPEED, 0.0f, 0, 0 };
    mHorses[2]  = { startX + 900.0f,  LANE_1_Y, LANE_1_SPEED, 0.0f, 0, 0 };
    mHorses[3]  = { startX + 1200.0f, LANE_1_Y, LANE_1_SPEED, 0.0f, 0, 0 };
    mHorses[4]  = { startX + 450.0f,  LANE_2_Y, LANE_2_SPEED, 0.0f, 0, 1 };
    mHorses[5]  = { startX + 750.0f,  LANE_2_Y, LANE_2_SPEED, 0.0f, 0, 1 };
    mHorses[6]  = { startX + 1050.0f, LANE_2_Y, LANE_2_SPEED, 0.0f, 0, 1 };
    mHorses[7]  = { startX + 1350.0f, LANE_2_Y, LANE_2_SPEED, 0.0f, 0, 1 };
    mHorses[8]  = { startX + 200.0f,  LANE_3_Y, LANE_3_SPEED, 0.0f, 0, 2 };
    mHorses[9]  = { startX + 500.0f,  LANE_3_Y, LANE_3_SPEED, 0.0f, 0, 2 };
    mHorses[10] = { startX + 800.0f,  LANE_3_Y, LANE_3_SPEED, 0.0f, 0, 2 };
    mHorses[11] = { startX + 1100.0f, LANE_3_Y, LANE_3_SPEED, 0.0f, 0, 2 };
    mHorses[12] = { startX + 350.0f,  LANE_4_Y, LANE_4_SPEED, 0.0f, 0, 3 };
    mHorses[13] = { startX + 650.0f,  LANE_4_Y, LANE_4_SPEED, 0.0f, 0, 3 };
    mHorses[14] = { startX + 950.0f,  LANE_4_Y, LANE_4_SPEED, 0.0f, 0, 3 };
    mHorses[15] = { startX + 1250.0f, LANE_4_Y, LANE_4_SPEED, 0.0f, 0, 3 };
}

void Level2::update(float deltaTime)
{
    if (mLevelDone)
    {
        mEndTimer -= deltaTime;
        if (mEndTimer <= 0.0f)
            mGameState.nextSceneID = mLevelWon ? LEVEL_3_INTRO : GAME_OVER_SCREEN;
        return;
    }

    if (mInvincibleTimer > 0.0f)
        mInvincibleTimer -= deltaTime;

    if (!IsSoundPlaying(mGallopSound)) PlaySound(mGallopSound);

    Vector2 curPos = mGameState.player->getPosition();
    float   dx     = 0.0f;
    float   dy     = 0.0f;

    if (IsKeyDown(KEY_W)) dy -= PLAYER_SPEED * deltaTime;
    if (IsKeyDown(KEY_S)) dy += PLAYER_SPEED * deltaTime;
    if (IsKeyDown(KEY_D)) { dx += PLAYER_SPEED * deltaTime; mPlayerFacingRight = true;  }
    if (IsKeyDown(KEY_A)) { dx -= PLAYER_SPEED * deltaTime; mPlayerFacingRight = false; }

    mPlayerMoving = (dx != 0.0f || dy != 0.0f);

    float pw  = PLAYER_SCALE * 0.28f;
    float ph  = PLAYER_SCALE * 0.28f;
    float xOv = 0.0f, yOv = 0.0f;

    auto hitsWall = [&](Vector2 p) {
        return mGameState.map->isSolidTileAt({ p.x - pw, p.y }, &xOv, &yOv) ||
               mGameState.map->isSolidTileAt({ p.x + pw, p.y }, &xOv, &yOv) ||
               mGameState.map->isSolidTileAt({ p.x, p.y - ph }, &xOv, &yOv) ||
               mGameState.map->isSolidTileAt({ p.x, p.y + ph }, &xOv, &yOv);
    };

    Vector2 newPos = curPos;

    Vector2 tryX = { curPos.x + dx, curPos.y };
    if (!hitsWall(tryX)) newPos.x = tryX.x;

    Vector2 tryY = { newPos.x, curPos.y + dy };
    if (!hitsWall(tryY)) newPos.y = tryY.y;

    mGameState.player->setPosition(newPos);

    float camMinX = mGameState.map->getLeftBoundary()  + mOrigin.x;
    float camMaxX = mGameState.map->getRightBoundary() - mOrigin.x;
    gCamera.target.x = fmaxf(camMinX, fminf(camMaxX, newPos.x));

    mPlayerAnimTimer += deltaTime;
    if (mPlayerAnimTimer >= PLAYER_ANIM_SPD)
    {
        mPlayerAnimTimer  = 0.0f;
        mPlayerFrameIndex = (mPlayerFrameIndex + 1) % 6;
    }

    for (int i = 0; i < HORSE_COUNT; i++)
    {
        mHorses[i].x -= mHorses[i].speed * deltaTime;

        if (mHorses[i].x < mGameState.map->getLeftBoundary() - HORSE_DISPLAY_W)
        {
            float laneY   = (mHorses[i].lane == 0) ? LANE_1_Y :
                            (mHorses[i].lane == 1) ? LANE_2_Y :
                            (mHorses[i].lane == 2) ? LANE_3_Y : LANE_4_Y;
            mHorses[i].x = newPos.x + 200.0f + (float)(rand() % 200);
            mHorses[i].y = laneY;
        }

        mHorses[i].animTimer += deltaTime;
        if (mHorses[i].animTimer >= HORSE_ANIM_SPD)
        {
            mHorses[i].animTimer  = 0.0f;
            mHorses[i].frameIndex = (mHorses[i].frameIndex + 1) % 7;
        }

        if (mInvincibleTimer <= 0.0f)
        {
            float hw = HORSE_DISPLAY_W * 0.34f;
            float hh = HORSE_DISPLAY_H * 0.30f;

            bool hit = fabsf(newPos.x - mHorses[i].x) < hw + pw &&
                       fabsf(newPos.y - mHorses[i].y) < hh + ph;

            if (hit)
            {
                mLives--;
                mInvincibleTimer = INVINCIBLE_TIME;
                PlaySound(mHitSound);

                if (mLives <= 0)
                {
                    mLevelDone = true;
                    mLevelWon  = false;
                    mEndTimer  = END_DELAY;
                    return;
                }
            }
        }
    }

    float exitX = mGameState.map->getLeftBoundary() + (LEVEL_2_WIDTH - EXIT_MARGIN) * TILE_DIMENSION;
    if (newPos.x >= exitX)
    {
        mLevelDone = true;
        mLevelWon  = true;
        mEndTimer  = END_DELAY;
    }
}

void Level2::render()
{
    ClearBackground({ 120, 80, 40, 255 });

    float camLeft = gCamera.target.x - mOrigin.x;

    DrawTexturePro(
        mBgTexture,
        { 0, 0, (float)mBgTexture.width, (float)mBgTexture.height },
        { camLeft, 0, 1000, 600 },
        { 0, 0 }, 0.0f, WHITE
    );

    DrawRectangle((int)(mGameState.map->getLeftBoundary()), (int)(LANE_1_Y - HORSE_DISPLAY_H * 0.5f),
                  LEVEL_2_WIDTH * (int)TILE_DIMENSION, (int)HORSE_DISPLAY_H, { 80, 50, 10, 50 });
    DrawRectangle((int)(mGameState.map->getLeftBoundary()), (int)(LANE_2_Y - HORSE_DISPLAY_H * 0.5f),
                  LEVEL_2_WIDTH * (int)TILE_DIMENSION, (int)HORSE_DISPLAY_H, { 60, 40, 10, 40 });
    DrawRectangle((int)(mGameState.map->getLeftBoundary()), (int)(LANE_3_Y - HORSE_DISPLAY_H * 0.5f),
                  LEVEL_2_WIDTH * (int)TILE_DIMENSION, (int)HORSE_DISPLAY_H, { 80, 50, 10, 50 });

    for (int i = 0; i < HORSE_COUNT; i++)
    {
        float srcX = mHorses[i].frameIndex * HORSE_FRAME_W;
        DrawTexturePro(
            mHorseSpriteSheet,
            { srcX, 0.0f, HORSE_FRAME_W, HORSE_FRAME_H },
            { mHorses[i].x, mHorses[i].y, HORSE_DISPLAY_W, HORSE_DISPLAY_H },
            { HORSE_DISPLAY_W / 2.0f, HORSE_DISPLAY_H / 2.0f },
            0.0f, WHITE
        );
    }

    mGameState.map->render();

    float exitX = mGameState.map->getLeftBoundary() +
                  (LEVEL_2_WIDTH - EXIT_MARGIN) * TILE_DIMENSION;
    DrawLineEx({ exitX, 0 }, { exitX, 600 }, 4.0f, GOLD);
    DrawText("EXIT",
        (int)exitX - MeasureText("EXIT", 22) / 2,
        (int)mOrigin.y - 11, 22, GOLD);

    Vector2 pos      = mGameState.player->getPosition();
    bool invisible   = mInvincibleTimer > 0.0f && ((int)(mInvincibleTimer * 10) % 2 == 0);

    if (!invisible)
    {
        Texture2D &frame = mPlayerMoving
            ? mMoveFrames[mPlayerFrameIndex]
            : mIdleFrames[mPlayerFrameIndex];

        float srcW = mPlayerFacingRight ?  (float)frame.width : -(float)frame.width;
        float srcX = mPlayerFacingRight ? 0.0f : (float)frame.width;

        DrawTexturePro(
            frame,
            { srcX, 0.0f, srcW, (float)frame.height },
            { pos.x, pos.y, PLAYER_SCALE, PLAYER_SCALE },
            { PLAYER_SCALE / 2.0f, PLAYER_SCALE / 2.0f },
            0.0f, WHITE
        );
    }

    DrawText("LIVES", (int)(camLeft + 20), 18, 18, LIGHTGRAY);
    for (int i = 0; i < 3; i++)
    {
        Color c = (i < mLives) ? RED : Fade(WHITE, 0.25f);
        DrawCircle((int)(camLeft + 28) + i * 26, 48, 10, c);
    }

    float distToExit = exitX - pos.x;
    if (distToExit > 0)
    {
        const char *distText = TextFormat("EXIT  %.0f m", distToExit / TILE_DIMENSION);
        DrawText(distText, (int)(camLeft + 650), 18, 20, GOLD);
    }

    if (mLevelDone)
    {
        const char *msg  = mLevelWon ? "YOU MADE IT!" : "TRAMPLED!";
        Color        col = mLevelWon ? GOLD : RED;
        int          sz  = 52;
        DrawRectangle((int)camLeft, (int)mOrigin.y - 50, 1000, 100, { 0, 0, 0, 180 });
        DrawText(msg,
            (int)(camLeft + 500 - MeasureText(msg, sz) / 2),
            (int)mOrigin.y - sz / 2, sz, col);
    }
}

void Level2::shutdown()
{
    delete mGameState.player;
    mGameState.player = nullptr;

    delete mGameState.map;
    mGameState.map = nullptr;

    UnloadTexture(mBgTexture);
    mBgTexture = {};

    UnloadTexture(mHorseSpriteSheet);
    mHorseSpriteSheet = {};

    for (int i = 0; i < 6; i++)
    {
        UnloadTexture(mIdleFrames[i]);
        mIdleFrames[i] = {};
        UnloadTexture(mMoveFrames[i]);
        mMoveFrames[i] = {};
    }

    UnloadSound(mGallopSound);
    mGallopSound = {};
    UnloadSound(mHitSound);
    mHitSound = {};

    gCamera.target = mOrigin;
}