#include "Level3.h"
#include <cmath>

Level3::Level3()                                      : Scene{ {0.0f}, nullptr   } {}
Level3::Level3(Vector2 origin, const char *bgHexCode) : Scene{ origin, bgHexCode } {}

Level3::~Level3() { shutdown(); }

void Level3::initialise()
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
        LEVEL_3_WIDTH, LEVEL_3_HEIGHT,
        (unsigned int *) mLevelData,
        "assets/tileset.png",
        TILE_DIMENSION,
        22, 11,
        mOrigin
    );

    float leftBoundary = mOrigin.x - (LEVEL_3_WIDTH  * TILE_DIMENSION) / 2.0f;
    float topBoundary  = mOrigin.y - (LEVEL_3_HEIGHT * TILE_DIMENSION) / 2.0f;

    /*
        ----------- PROTAGONIST (grenouille) -----------
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
        ----------- ENNEMIS (10 au total) -----------
        Comportements (Requirement 4) :
          Statique   : Spikes [0][1]         — pieges au sol
          Patroller  : Saws   [2][3][4]      — PATROL horizontal (comportement 1)
          Follower   : SpikeHeads [5][6]     — FOLLOWER (comportement 2)
          Flyer      : SpikedBalls [7][8][9] — position overridee via sinf() (comportement 3)

        Disposition par section :
          Section 1 (cols 0-3)   : Spike[0] col 3.5
          Gap 1 (cols 4-6)       : pierre col 5
          Section 2 (cols 7-11)  : Saw[2] PATROL 7-11 + plateforme cols 8-10
          Gap 2 (cols 12-13)     : pierre col 12
          Section 3 (cols 14-18) : Spike[1] col 15.5 + Ball[7] col 16.5 (combo sol+air)
          Gap 3 (cols 19-21)     : pierres cols 20-21
          Section 4 (cols 22-26) : Saw[3] PATROL 22-26
          Gap 4 (cols 27-28)     : pierre col 28
          Section 5 (cols 29-34) : SpikeHead[5] col 31 + Ball[8] col 31.5 (combo air+sol)
                                   + plateforme cols 30-32
          Gap 5 (cols 35-37)     : pierre col 36
          Section 6 (cols 38-42) : Ball[9] col 40.5 + plateforme cols 40-42 (eviter la ball)
          Gap 6 (cols 43-44)     : pierre col 44
          Section 7 (cols 45-49) : Saw[4] PATROL 45-49 + SpikeHead[6] col 48 (gauntlet final)
    */
    mGameState.enemyCount = LEVEL_3_ENEMY_COUNT;
    mGameState.enemies    = new Entity[LEVEL_3_ENEMY_COUNT];

    // --- [0] Spike — statique col 3.5, avertissement avant le 1er gap ---
    mGameState.enemies[0].setPosition({
        leftBoundary + 3.5f * TILE_DIMENSION,
        topBoundary  + 6.0f * TILE_DIMENSION - TILE_DIMENSION * 0.2f
    });
    mGameState.enemies[0].setScale({ TILE_DIMENSION, TILE_DIMENSION * 0.4f });
    mGameState.enemies[0].setColliderDimensions({ TILE_DIMENSION * 0.8f, TILE_DIMENSION * 0.35f });
    mGameState.enemies[0].setTexture("assets/Traps/Spikes/Idle.png");

    // --- [1] Spike — statique col 15.5, piege dans la section 3 ---
    mGameState.enemies[1].setPosition({
        leftBoundary + 15.5f * TILE_DIMENSION,
        topBoundary  +  6.0f * TILE_DIMENSION - TILE_DIMENSION * 0.2f
    });
    mGameState.enemies[1].setScale({ TILE_DIMENSION, TILE_DIMENSION * 0.4f });
    mGameState.enemies[1].setColliderDimensions({ TILE_DIMENSION * 0.8f, TILE_DIMENSION * 0.35f });
    mGameState.enemies[1].setTexture("assets/Traps/Spikes/Idle.png");

    // --- [2] Saw — PATROL cols 7-11, section 2 (apres gap 1) ---
    // La plateforme row4 cols 8-10 permet de passer au-dessus
    mGameState.enemies[2].setPosition({
        leftBoundary +  9.0f * TILE_DIMENSION,
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
        leftBoundary +  7.0f * TILE_DIMENSION,
        leftBoundary + 11.0f * TILE_DIMENSION
    );
    mGameState.enemies[2].setSpeed(145);
    // PAS de setAcceleration → gravite zero → flotte
    mGameState.enemies[2].setFrameSpeed(12);

    // --- [3] Saw — PATROL cols 22-26, section 4 ---
    mGameState.enemies[3].setPosition({
        leftBoundary + 24.0f * TILE_DIMENSION,
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
        leftBoundary + 22.0f * TILE_DIMENSION,
        leftBoundary + 26.0f * TILE_DIMENSION
    );
    mGameState.enemies[3].setSpeed(165);
    mGameState.enemies[3].setFrameSpeed(12);

    // --- [4] Saw — PATROL cols 45-49, gauntlet final ---
    mGameState.enemies[4].setPosition({
        leftBoundary + 47.0f * TILE_DIMENSION,
        topBoundary  +  5.0f * TILE_DIMENSION
    });
    mGameState.enemies[4].setScale({ TILE_DIMENSION, TILE_DIMENSION });
    mGameState.enemies[4].setColliderDimensions({ TILE_DIMENSION * 0.8f, TILE_DIMENSION * 0.8f });
    mGameState.enemies[4].setTextureType(ATLAS);
    mGameState.enemies[4].setTexture("assets/Traps/Saw/On (38x38).png");
    mGameState.enemies[4].setSpriteSheetDimensions({ 1, 8 });
    mGameState.enemies[4].setAnimationAtlas({
        { LEFT,  { 0,1,2,3,4,5,6,7 } },
        { RIGHT, { 0,1,2,3,4,5,6,7 } }
    });
    mGameState.enemies[4].setEntityType(NPC);
    mGameState.enemies[4].setAIType(PATROL);
    mGameState.enemies[4].setPatrolRange(
        leftBoundary + 45.0f * TILE_DIMENSION,
        leftBoundary + 49.0f * TILE_DIMENSION
    );
    mGameState.enemies[4].setSpeed(180);
    mGameState.enemies[4].setFrameSpeed(12);

    // --- [5] Spike Head — FOLLOWER col 31, section 5 ---
    // Spawne en hauteur, tombe sur le sol → suit le joueur
    // Combine avec Ball[8] → section la plus complexe du niveau
    mGameState.enemies[5].setPosition({
        leftBoundary + 31.0f * TILE_DIMENSION,
        topBoundary  +  1.0f * TILE_DIMENSION
    });
    mGameState.enemies[5].setScale({ TILE_DIMENSION, TILE_DIMENSION });
    mGameState.enemies[5].setColliderDimensions({ TILE_DIMENSION * 0.75f, TILE_DIMENSION * 0.75f });
    mGameState.enemies[5].setTextureType(ATLAS);
    mGameState.enemies[5].setTexture("assets/Traps/Spike Head/Blink (54x52).png");
    mGameState.enemies[5].setSpriteSheetDimensions({ 1, 4 });
    mGameState.enemies[5].setAnimationAtlas({
        { LEFT,  { 0,1,2,3 } },
        { RIGHT, { 0,1,2,3 } }
    });
    mGameState.enemies[5].setEntityType(NPC);
    mGameState.enemies[5].setAIType(FOLLOWER);
    mGameState.enemies[5].setAIState(IDLE);
    mGameState.enemies[5].setSpeed(155);
    mGameState.enemies[5].setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });
    mGameState.enemies[5].setFrameSpeed(8);

    // --- [6] Spike Head — FOLLOWER col 48, gauntlet final ---
    // Combine avec la Saw[4] → double menace dans la derniere section
    mGameState.enemies[6].setPosition({
        leftBoundary + 48.0f * TILE_DIMENSION,
        topBoundary  +  1.0f * TILE_DIMENSION
    });
    mGameState.enemies[6].setScale({ TILE_DIMENSION, TILE_DIMENSION });
    mGameState.enemies[6].setColliderDimensions({ TILE_DIMENSION * 0.75f, TILE_DIMENSION * 0.75f });
    mGameState.enemies[6].setTextureType(ATLAS);
    mGameState.enemies[6].setTexture("assets/Traps/Spike Head/Blink (54x52).png");
    mGameState.enemies[6].setSpriteSheetDimensions({ 1, 4 });
    mGameState.enemies[6].setAnimationAtlas({
        { LEFT,  { 0,1,2,3 } },
        { RIGHT, { 0,1,2,3 } }
    });
    mGameState.enemies[6].setEntityType(NPC);
    mGameState.enemies[6].setAIType(FOLLOWER);
    mGameState.enemies[6].setAIState(IDLE);
    mGameState.enemies[6].setSpeed(168);
    mGameState.enemies[6].setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });
    mGameState.enemies[6].setFrameSpeed(8);

    // --- [7] Spiked Ball — FLYER col 16.5, section 3 ---
    // Oscille verticalement : y = baseY + BALL_AMPLITUDE * sinf(t * 1.8f)
    // Combine avec Spike[1] au sol → joueur doit eviter spike ET timer la ball
    mGameState.enemies[7].setPosition({
        leftBoundary + 16.5f * TILE_DIMENSION,
        topBoundary  +  3.5f * TILE_DIMENSION  // centre de l'oscillation
    });
    mGameState.enemies[7].setScale({ TILE_DIMENSION * 0.8f, TILE_DIMENSION * 0.8f });
    mGameState.enemies[7].setColliderDimensions({ TILE_DIMENSION * 0.75f, TILE_DIMENSION * 0.75f });
    mGameState.enemies[7].setTexture("assets/Traps/Spiked Ball/Spiked Ball.png");
    // PAS de setAcceleration, PAS de setEntityType(NPC) → aucune physique ni AI
    // Position dictee entierement par sinf() dans update()

    // --- [8] Spiked Ball — FLYER col 31.5, section 5 ---
    // Combine avec SpikeHead[5] qui suit le joueur → section la plus difficile
    mGameState.enemies[8].setPosition({
        leftBoundary + 31.5f * TILE_DIMENSION,
        topBoundary  +  3.5f * TILE_DIMENSION
    });
    mGameState.enemies[8].setScale({ TILE_DIMENSION * 0.8f, TILE_DIMENSION * 0.8f });
    mGameState.enemies[8].setColliderDimensions({ TILE_DIMENSION * 0.75f, TILE_DIMENSION * 0.75f });
    mGameState.enemies[8].setTexture("assets/Traps/Spiked Ball/Spiked Ball.png");

    // --- [9] Spiked Ball — FLYER col 40.5, section 6 ---
    // La plus rapide → force un timing precis avant le gauntlet final
    mGameState.enemies[9].setPosition({
        leftBoundary + 40.5f * TILE_DIMENSION,
        topBoundary  +  3.5f * TILE_DIMENSION
    });
    mGameState.enemies[9].setScale({ TILE_DIMENSION * 0.8f, TILE_DIMENSION * 0.8f });
    mGameState.enemies[9].setColliderDimensions({ TILE_DIMENSION * 0.75f, TILE_DIMENSION * 0.75f });
    mGameState.enemies[9].setTexture("assets/Traps/Spiked Ball/Spiked Ball.png");

    // Memorise les positions initiales pour le reset au respawn
    for (int i = 0; i < LEVEL_3_ENEMY_COUNT; i++)
        mEnemyStartPositions[i] = mGameState.enemies[i].getPosition();

    // Memorise les positions de base des balls (centre de l'oscillation)
    for (int b = 0; b < LEVEL_3_BALL_COUNT; b++)
    {
        int idx      = LEVEL_3_ENEMY_COUNT - LEVEL_3_BALL_COUNT + b;
        mBallBaseX[b] = mGameState.enemies[idx].getPosition().x;
        mBallBaseY[b] = mGameState.enemies[idx].getPosition().y;
    }
}

void Level3::updatePlayerAnimation()
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

void Level3::checkEnemyCollisions()
{
    if (mHitCooldown > 0.0f) return;
    if (mGameState.nextSceneID != NO_SCENE) return; // evite le double-declenchement

    for (int i = 0; i < mGameState.enemyCount; i++)
    {
        if (!mGameState.enemies[i].isActive()) continue;

        if (mGameState.player->isCollidingWith(&mGameState.enemies[i]))
        {
            PlaySound(mGameState.hitSound);
            mGameState.lives--;
            mHitCooldown = HIT_COOLDOWN_DURATION;

            // respawn au niveau 3 (remet tous les ennemis + joueur a zero)
            if (mGameState.lives <= 0)
                mGameState.nextSceneID = GAME_OVER_SCREEN;
            else
                mGameState.nextSceneID = LEVEL_3;

            return;
        }
    }
}

void Level3::update(float deltaTime)
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

    // Spiked Balls (Flyer) — override de position via sinusoide verticale
    // Comportement Flyer : mouvement dans les airs selon un schema mathematique,
    // independant du joueur (cf. Requirement 4 : "doesn't respond to the player")
    float t = (float) GetTime();
    float freqs[LEVEL_3_BALL_COUNT] = { 1.8f, 2.3f, 2.8f };
    for (int b = 0; b < LEVEL_3_BALL_COUNT; b++)
    {
        int idx = LEVEL_3_ENEMY_COUNT - LEVEL_3_BALL_COUNT + b;
        mGameState.enemies[idx].setPosition({
            mBallBaseX[b],
            mBallBaseY[b] + BALL_AMPLITUDE * sinf(t * freqs[b])
        });
    }

    updatePlayerAnimation();
    checkEnemyCollisions();

    // fin du niveau : joueur atteint le bord droit → ecran de victoire
    float rightBoundary = mOrigin.x + (LEVEL_3_WIDTH * TILE_DIMENSION) / 2.0f;
    if (mGameState.nextSceneID == NO_SCENE &&
        mGameState.player->getPosition().x > rightBoundary - TILE_DIMENSION)
        mGameState.nextSceneID = WIN_SCREEN;

    // tombe dans un trou
    float bottomBoundary = mOrigin.y + (LEVEL_3_HEIGHT * TILE_DIMENSION) / 2.0f;
    if (mGameState.nextSceneID == NO_SCENE &&
        mGameState.player->getPosition().y > bottomBoundary)
    {
        PlaySound(mGameState.hitSound);
        mGameState.lives--;

        if (mGameState.lives <= 0)
            mGameState.nextSceneID = GAME_OVER_SCREEN;
        else
            mGameState.nextSceneID = LEVEL_3;
    }
}

void Level3::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.map->render();
    mGameState.player->render();

    for (int i = 0; i < mGameState.enemyCount; i++)
        mGameState.enemies[i].render();
}

void Level3::shutdown()
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
    // attendre que le son de mort finisse avant de liberer le buffer
    while (IsSoundPlaying(mGameState.hitSound)) {}
    UnloadSound(mGameState.hitSound);
    mGameState.hitSound = {};
}