#include "Level2.h"

Level2::Level2()                                      : Scene{ {0.0f}, nullptr   } {}
Level2::Level2(Vector2 origin, const char *bgHexCode) : Scene{ origin, bgHexCode } {}

Level2::~Level2() { shutdown(); }

void Level2::initialise()
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
        LEVEL_2_WIDTH, LEVEL_2_HEIGHT,
        (unsigned int *) mLevelData,
        "assets/tileset.png",
        TILE_DIMENSION,
        22, 11,
        mOrigin
    );

    float leftBoundary = mOrigin.x - (LEVEL_2_WIDTH  * TILE_DIMENSION) / 2.0f;
    float topBoundary  = mOrigin.y - (LEVEL_2_HEIGHT * TILE_DIMENSION) / 2.0f;

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
        ----------- ENNEMIS (8 au total) -----------
        Disposition:
          Section 1 (cols 0-3)   : Spike [0] col 3.5
          Gap 1 (cols 4-6)       : pierre col 5
          Section 2 (cols 7-11)  : Saw [3] PATROL 7-11 + plateforme row4 cols 8-10
          Gap 2 (cols 12-13)     : pierre col 12
          Section 3 (cols 14-17) : Spike [1] col 15.5
          Gap 3 (cols 18-20)     : pierres cols 19-20
          Section 4 (cols 21-22) : Saw [4] PATROL 21-25 (section etroite + gap 4)
          Gap 4 (cols 23-25)     : pierre col 24
          Section 5 (cols 26-30) : Spike [2] col 26.5 + Saw [5] PATROL 26-30
                                   + plateforme row4 cols 27-29 + SpikeHead [6] col 29
          Gap 5 (cols 31-33)     : pierres cols 31-32
          Section 6 (cols 34-37) : (respiration apres section 5)
          Gap 6 (cols 38-39)     : pierre col 38
          Section 7 (cols 40-45) : SpikeHead [7] col 43 + plateforme row4 cols 41-43
    */
    mGameState.enemyCount = LEVEL_2_ENEMY_COUNT;
    mGameState.enemies    = new Entity[LEVEL_2_ENEMY_COUNT];

    // --- [0] Spike — col 3.5, juste avant le 1er gap ---
    mGameState.enemies[0].setPosition({
        leftBoundary + 3.5f * TILE_DIMENSION,
        topBoundary  + 6.0f * TILE_DIMENSION - TILE_DIMENSION * 0.2f
    });
    mGameState.enemies[0].setScale({ TILE_DIMENSION, TILE_DIMENSION * 0.4f });
    mGameState.enemies[0].setColliderDimensions({ TILE_DIMENSION * 0.8f, TILE_DIMENSION * 0.35f });
    mGameState.enemies[0].setTexture("assets/Traps/Spikes/Idle.png");

    // --- [1] Spike — col 15.5, piege discret apres gap 2 ---
    mGameState.enemies[1].setPosition({
        leftBoundary + 15.5f * TILE_DIMENSION,
        topBoundary  +  6.0f * TILE_DIMENSION - TILE_DIMENSION * 0.2f
    });
    mGameState.enemies[1].setScale({ TILE_DIMENSION, TILE_DIMENSION * 0.4f });
    mGameState.enemies[1].setColliderDimensions({ TILE_DIMENSION * 0.8f, TILE_DIMENSION * 0.35f });
    mGameState.enemies[1].setTexture("assets/Traps/Spikes/Idle.png");

    // --- [2] Spike — col 26.5, entree de la section 5 (danger double) ---
    mGameState.enemies[2].setPosition({
        leftBoundary + 26.5f * TILE_DIMENSION,
        topBoundary  +  6.0f * TILE_DIMENSION - TILE_DIMENSION * 0.2f
    });
    mGameState.enemies[2].setScale({ TILE_DIMENSION, TILE_DIMENSION * 0.4f });
    mGameState.enemies[2].setColliderDimensions({ TILE_DIMENSION * 0.8f, TILE_DIMENSION * 0.35f });
    mGameState.enemies[2].setTexture("assets/Traps/Spikes/Idle.png");

    // --- [3] Saw — flottante PATROL cols 7-11, section 2 ---
    // La plateforme a row4 cols 8-10 permet de passer au-dessus
    mGameState.enemies[3].setPosition({
        leftBoundary +  9.0f * TILE_DIMENSION,
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
        leftBoundary +  7.0f * TILE_DIMENSION,
        leftBoundary + 11.0f * TILE_DIMENSION
    );
    mGameState.enemies[3].setSpeed(140);
    // PAS de setAcceleration → gravite zero → flotte
    mGameState.enemies[3].setFrameSpeed(12);

    // --- [4] Saw — flottante PATROL cols 21-25, section etroite + gap 4 ---
    // Force un timing precis : la section 4 n'a que 2 tuiles de large
    mGameState.enemies[4].setPosition({
        leftBoundary + 22.0f * TILE_DIMENSION,
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
        leftBoundary + 21.0f * TILE_DIMENSION,
        leftBoundary + 25.0f * TILE_DIMENSION
    );
    mGameState.enemies[4].setSpeed(165);
    mGameState.enemies[4].setFrameSpeed(12);

    // --- [5] Saw — flottante PATROL cols 26-30, section 5 (danger double) ---
    // Combine avec le spike [2] et le spike head [6] → section la plus difficile
    // La plateforme row4 cols 27-29 permet d'eviter la saw par le dessus
    mGameState.enemies[5].setPosition({
        leftBoundary + 28.0f * TILE_DIMENSION,
        topBoundary  +  5.0f * TILE_DIMENSION
    });
    mGameState.enemies[5].setScale({ TILE_DIMENSION, TILE_DIMENSION });
    mGameState.enemies[5].setColliderDimensions({ TILE_DIMENSION * 0.8f, TILE_DIMENSION * 0.8f });
    mGameState.enemies[5].setTextureType(ATLAS);
    mGameState.enemies[5].setTexture("assets/Traps/Saw/On (38x38).png");
    mGameState.enemies[5].setSpriteSheetDimensions({ 1, 8 });
    mGameState.enemies[5].setAnimationAtlas({
        { LEFT,  { 0,1,2,3,4,5,6,7 } },
        { RIGHT, { 0,1,2,3,4,5,6,7 } }
    });
    mGameState.enemies[5].setEntityType(NPC);
    mGameState.enemies[5].setAIType(PATROL);
    mGameState.enemies[5].setPatrolRange(
        leftBoundary + 26.0f * TILE_DIMENSION,
        leftBoundary + 30.0f * TILE_DIMENSION
    );
    mGameState.enemies[5].setSpeed(155);
    mGameState.enemies[5].setFrameSpeed(12);

    // --- [6] Spike Head — FOLLOWER col 29.5, section 5 ---
    // Combine avec la saw [5] → gauntlet central du niveau
    // Strategie : monter sur la plateforme row4 cols 27-29, puis sprinter vers gap 5
    mGameState.enemies[6].setPosition({
        leftBoundary + 29.5f * TILE_DIMENSION,
        topBoundary  +  1.0f * TILE_DIMENSION   // spawne en hauteur, tombe sur le sol
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
    mGameState.enemies[6].setSpeed(155);
    mGameState.enemies[6].setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });
    mGameState.enemies[6].setFrameSpeed(8);

    // --- [7] Spike Head — FOLLOWER col 43, gauntlet final (section 7) ---
    // La plateforme row4 cols 41-43 offre une echappatoire par le dessus
    // Sprinter jusqu'au bord droit pour terminer le niveau
    mGameState.enemies[7].setPosition({
        leftBoundary + 43.0f * TILE_DIMENSION,
        topBoundary  +  1.0f * TILE_DIMENSION   // spawne en hauteur, tombe sur le sol
    });
    mGameState.enemies[7].setScale({ TILE_DIMENSION, TILE_DIMENSION });
    mGameState.enemies[7].setColliderDimensions({ TILE_DIMENSION * 0.75f, TILE_DIMENSION * 0.75f });
    mGameState.enemies[7].setTextureType(ATLAS);
    mGameState.enemies[7].setTexture("assets/Traps/Spike Head/Blink (54x52).png");
    mGameState.enemies[7].setSpriteSheetDimensions({ 1, 4 });
    mGameState.enemies[7].setAnimationAtlas({
        { LEFT,  { 0,1,2,3 } },
        { RIGHT, { 0,1,2,3 } }
    });
    mGameState.enemies[7].setEntityType(NPC);
    mGameState.enemies[7].setAIType(FOLLOWER);
    mGameState.enemies[7].setAIState(IDLE);
    mGameState.enemies[7].setSpeed(160);
    mGameState.enemies[7].setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });
    mGameState.enemies[7].setFrameSpeed(8);

    // memorise les positions initiales pour le reset au respawn
    for (int i = 0; i < LEVEL_2_ENEMY_COUNT; i++)
        mEnemyStartPositions[i] = mGameState.enemies[i].getPosition();
}

void Level2::updatePlayerAnimation()
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

void Level2::checkEnemyCollisions()
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

            // respawn au niveau 2 (les ennemis et le joueur se remettent a zero)
            if (mGameState.lives <= 0)
                mGameState.nextSceneID = GAME_OVER_SCREEN;
            else
                mGameState.nextSceneID = LEVEL_2;

            return;
        }
    }
}

void Level2::update(float deltaTime)
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

    // fin du niveau : joueur atteint le bord droit → ecran de victoire
    float rightBoundary = mOrigin.x + (LEVEL_2_WIDTH * TILE_DIMENSION) / 2.0f;
    if (mGameState.nextSceneID == NO_SCENE &&
        mGameState.player->getPosition().x > rightBoundary - TILE_DIMENSION)
        mGameState.nextSceneID = LEVEL_3;

    // tombe dans un trou
    float bottomBoundary = mOrigin.y + (LEVEL_2_HEIGHT * TILE_DIMENSION) / 2.0f;
    if (mGameState.nextSceneID == NO_SCENE &&
        mGameState.player->getPosition().y > bottomBoundary)
    {
        PlaySound(mGameState.hitSound);
        mGameState.lives--;

        if (mGameState.lives <= 0)
            mGameState.nextSceneID = GAME_OVER_SCREEN;
        else
            mGameState.nextSceneID = LEVEL_2;
    }
}

void Level2::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.map->render();
    mGameState.player->render();

    for (int i = 0; i < mGameState.enemyCount; i++)
        mGameState.enemies[i].render();
}

void Level2::shutdown()
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