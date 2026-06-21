#include "Level1.h"

Level1::Level1()                                      : Scene{ {0.0f}, nullptr   } {}
Level1::Level1(Vector2 origin, const char *bgHexCode) : Scene{ origin, bgHexCode } {}

Level1::~Level1() { shutdown(); }

void Level1::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
    mGameState.lives       = 3;
    mPlayerState           = IDLE_STATE;
    mHitCooldown           = 0.0f;

    /*
        ----------- AUDIO -----------
    */
    mGameState.bgm = LoadMusicStream("assets/audio/Vibing Over Venus.mp3");
    SetMusicVolume(mGameState.bgm, 0.33f);
    PlayMusicStream(mGameState.bgm);

    mGameState.jumpSound = LoadSound("assets/audio/frog_jump.wav");
    mGameState.hitSound  = LoadSound("assets/audio/hit_enemy.wav");


    /*
        ----------- MAP -----------
    */
    mGameState.map = new Map(
        LEVEL_1_WIDTH, LEVEL_1_HEIGHT,
        (unsigned int *) mLevelData,
        "assets/tileset.png",
        TILE_DIMENSION,
        22, 11,
        mOrigin
    );

    float leftBoundary = mOrigin.x - (LEVEL_1_WIDTH  * TILE_DIMENSION) / 2.0f;
    float topBoundary  = mOrigin.y - (LEVEL_1_HEIGHT * TILE_DIMENSION) / 2.0f;

    /*
        ----------- frog -----------
    */
    mStartPosition = {
        leftBoundary + 1.5f * TILE_DIMENSION,
        topBoundary  + 5.5f * TILE_DIMENSION
    };

    mGameState.player = new Entity(
        mStartPosition,
        { TILE_DIMENSION, TILE_DIMENSION },
        "assets/Main Character/Ninja Frog/Idle (32x32).png",
        ATLAS,
        { 1, 11 },
        {{ LEFT,  { 0,1,2,3,4,5,6,7,8,9,10 } },
         { RIGHT, { 0,1,2,3,4,5,6,7,8,9,10 } }},
        PLAYER
    );

    mGameState.player->setJumpingPower(650.0f);     
    mGameState.player->setColliderDimensions({
        TILE_DIMENSION * 0.5f,
        TILE_DIMENSION * 0.7f
    });
    mGameState.player->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });
    mGameState.player->setFrameSpeed(10);
    mGameState.player->setSpeed(280);              

    /*
        ----------- ENNEMIS (les 6) -----------
    */
    mGameState.enemyCount = LEVEL_1_ENEMY_COUNT;
    mGameState.enemies    = new Entity[LEVEL_1_ENEMY_COUNT];


    mGameState.enemies[0].setPosition({
        leftBoundary + 4.5f * TILE_DIMENSION,
        topBoundary  + 6.0f * TILE_DIMENSION - TILE_DIMENSION * 0.2f
    });
    mGameState.enemies[0].setScale({ TILE_DIMENSION, TILE_DIMENSION * 0.4f });
    mGameState.enemies[0].setColliderDimensions({ TILE_DIMENSION * 0.8f, TILE_DIMENSION * 0.35f });
    mGameState.enemies[0].setTexture("assets/Traps/Spikes/Idle.png");

    mGameState.enemies[1].setPosition({
        leftBoundary + 30.5f * TILE_DIMENSION,
        topBoundary  +  6.0f * TILE_DIMENSION - TILE_DIMENSION * 0.2f
    });
    mGameState.enemies[1].setScale({ TILE_DIMENSION, TILE_DIMENSION * 0.4f });
    mGameState.enemies[1].setColliderDimensions({ TILE_DIMENSION * 0.8f, TILE_DIMENSION * 0.35f });
    mGameState.enemies[1].setTexture("assets/Traps/Spikes/Idle.png");

    mGameState.enemies[2].setPosition({
        leftBoundary + 10.0f * TILE_DIMENSION,
        topBoundary  +  5.0f * TILE_DIMENSION   
    });
    mGameState.enemies[2].setScale({ TILE_DIMENSION, TILE_DIMENSION });
    mGameState.enemies[2].setColliderDimensions({ TILE_DIMENSION * 0.8f, TILE_DIMENSION * 0.8f });
    mGameState.enemies[2].setTextureType(ATLAS);
    mGameState.enemies[2].setTexture("assets/Traps/Saw/On (38x38).png");
    mGameState.enemies[2].setSpriteSheetDimensions({ 1, 8 });
    mGameState.enemies[2].setAnimationAtlas({
        { LEFT,  { 0,1,2,3,4,5,6,7 } },
        { RIGHT, { 0,1,2,3,4,5,6,7 } }
    });
    mGameState.enemies[2].setEntityType(NPC);
    mGameState.enemies[2].setAIType(PATROL);
    mGameState.enemies[2].setPatrolRange(
        leftBoundary +  8.0f * TILE_DIMENSION,
        leftBoundary + 13.0f * TILE_DIMENSION
    );
    mGameState.enemies[2].setSpeed(150);
    mGameState.enemies[2].setFrameSpeed(12);

    mGameState.enemies[3].setPosition({
        leftBoundary + 25.0f * TILE_DIMENSION,
        topBoundary  +  5.0f * TILE_DIMENSION
    });
    mGameState.enemies[3].setScale({ TILE_DIMENSION, TILE_DIMENSION });
    mGameState.enemies[3].setColliderDimensions({ TILE_DIMENSION * 0.8f, TILE_DIMENSION * 0.8f });
    mGameState.enemies[3].setTextureType(ATLAS);
    mGameState.enemies[3].setTexture("assets/Traps/Saw/On (38x38).png");
    mGameState.enemies[3].setSpriteSheetDimensions({ 1, 8 });
    mGameState.enemies[3].setAnimationAtlas({
        { LEFT,  { 0,1,2,3,4,5,6,7 } },
        { RIGHT, { 0,1,2,3,4,5,6,7 } }
    });
    mGameState.enemies[3].setEntityType(NPC);
    mGameState.enemies[3].setAIType(PATROL);
    mGameState.enemies[3].setPatrolRange(
        leftBoundary + 23.0f * TILE_DIMENSION,
        leftBoundary + 27.0f * TILE_DIMENSION
    );
    mGameState.enemies[3].setSpeed(165);
    mGameState.enemies[3].setFrameSpeed(12);

    mGameState.enemies[4].setPosition({
        leftBoundary + 35.0f * TILE_DIMENSION,
        topBoundary  +  5.5f * TILE_DIMENSION
    });
    mGameState.enemies[4].setScale({ TILE_DIMENSION, TILE_DIMENSION });
    mGameState.enemies[4].setColliderDimensions({ TILE_DIMENSION * 0.75f, TILE_DIMENSION * 0.75f });
    mGameState.enemies[4].setTextureType(ATLAS);
    mGameState.enemies[4].setTexture("assets/Traps/Spike Head/Blink (54x52).png");
    mGameState.enemies[4].setSpriteSheetDimensions({ 1, 4 });
    mGameState.enemies[4].setAnimationAtlas({
        { LEFT,  { 0,1,2,3 } },
        { RIGHT, { 0,1,2,3 } }
    });
    mGameState.enemies[4].setEntityType(NPC);
    mGameState.enemies[4].setAIType(FOLLOWER);
    mGameState.enemies[4].setAIState(IDLE);
    mGameState.enemies[4].setSpeed(150);
    mGameState.enemies[4].setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });
    mGameState.enemies[4].setFrameSpeed(8);

    
    for (int i = 0; i < LEVEL_1_ENEMY_COUNT; i++)
        mEnemyStartPositions[i] = mGameState.enemies[i].getPosition();
}

void Level1::updatePlayerAnimation()
{
    PlayerState newState;

    if (!mGameState.player->isCollidingBottom())
    {
        if (mGameState.player->getVelocity().y < 0.0f)
            newState = JUMP_STATE;
        else
            newState = FALL_STATE;
    }
    else if (GetLength(mGameState.player->getMovement()) > 0.0f)
        newState = RUN_STATE;
    else
        newState = IDLE_STATE;

    if (newState == mPlayerState) return;

    mPlayerState = newState;

    switch (mPlayerState)
    {
    case IDLE_STATE:
        mGameState.player->setTexture("assets/Main Character/Ninja Frog/Idle (32x32).png");
        mGameState.player->setSpriteSheetDimensions({ 1, 11 });
        mGameState.player->setAnimationAtlas({
            { LEFT,  { 0,1,2,3,4,5,6,7,8,9,10 } },
            { RIGHT, { 0,1,2,3,4,5,6,7,8,9,10 } }
        });
        break;

    case RUN_STATE:
        mGameState.player->setTexture("assets/Main Character/Ninja Frog/Run (32x32).png");
        mGameState.player->setSpriteSheetDimensions({ 1, 12 });
        mGameState.player->setAnimationAtlas({
            { LEFT,  { 0,1,2,3,4,5,6,7,8,9,10,11 } },
            { RIGHT, { 0,1,2,3,4,5,6,7,8,9,10,11 } }
        });
        break;

    case JUMP_STATE:
        mGameState.player->setTexture("assets/Main Character/Ninja Frog/Jump (32x32).png");
        mGameState.player->setSpriteSheetDimensions({ 1, 1 });
        mGameState.player->setAnimationAtlas({
            { LEFT,  { 0 } },
            { RIGHT, { 0 } }
        });
        break;

    case FALL_STATE:
        mGameState.player->setTexture("assets/Main Character/Ninja Frog/Fall (32x32).png");
        mGameState.player->setSpriteSheetDimensions({ 1, 1 });
        mGameState.player->setAnimationAtlas({
            { LEFT,  { 0 } },
            { RIGHT, { 0 } }
        });
        break;
    }

    mGameState.player->setDirection(mGameState.player->getDirection());
}

void Level1::checkEnemyCollisions()
{
    if (mHitCooldown > 0.0f) return;
    if (mGameState.nextSceneID != NO_SCENE) return;

    for (int i = 0; i < mGameState.enemyCount; i++)
    {
        if (!mGameState.enemies[i].isActive()) continue;

        if (mGameState.player->isCollidingWith(&mGameState.enemies[i]))
        {
            PlaySound(mGameState.hitSound);
            mGameState.lives--;
            mHitCooldown = HIT_COOLDOWN_DURATION;

            if (mGameState.lives <= 0)
                mGameState.nextSceneID = GAME_OVER_SCREEN;
            else
                mGameState.nextSceneID = LEVEL_1;

            return;
        }
    }
}

void Level1::update(float deltaTime)
{
    if (mHitCooldown > 0.0f) mHitCooldown -= deltaTime;

    mGameState.player->update(
        deltaTime,
        nullptr,
        mGameState.map,
        nullptr,
        0
    );

    for (int i = 0; i < mGameState.enemyCount; i++)
        mGameState.enemies[i].update(
            deltaTime,
            mGameState.player,
            mGameState.map,
            nullptr,
            0
        );

    updatePlayerAnimation();
    checkEnemyCollisions();


    float rightBoundary = mOrigin.x + (LEVEL_1_WIDTH * TILE_DIMENSION) / 2.0f;
    if (mGameState.nextSceneID == NO_SCENE &&
        mGameState.player->getPosition().x > rightBoundary - TILE_DIMENSION)
        mGameState.nextSceneID = LEVEL_2;

   
    float bottomBoundary = mOrigin.y + (LEVEL_1_HEIGHT * TILE_DIMENSION) / 2.0f;
    if (mGameState.nextSceneID == NO_SCENE &&
        mGameState.player->getPosition().y > bottomBoundary)
    {
        PlaySound(mGameState.hitSound);
        mGameState.lives--;

        if (mGameState.lives <= 0)
            mGameState.nextSceneID = GAME_OVER_SCREEN;
        else
            mGameState.nextSceneID = LEVEL_1;
    }
}

void Level1::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.map->render();
    mGameState.player->render();

    for (int i = 0; i < mGameState.enemyCount; i++)
        mGameState.enemies[i].render();
}

void Level1::shutdown()
{
    delete mGameState.player;
    mGameState.player = nullptr;

    delete[] mGameState.enemies;
    mGameState.enemies = nullptr;

    delete mGameState.map;
    mGameState.map = nullptr;

    UnloadMusicStream(mGameState.bgm);
    mGameState.bgm = {};
    UnloadSound(mGameState.jumpSound);
    mGameState.jumpSound = {};
 
    while (IsSoundPlaying(mGameState.hitSound)) {}
    UnloadSound(mGameState.hitSound);
    mGameState.hitSound = {};
}