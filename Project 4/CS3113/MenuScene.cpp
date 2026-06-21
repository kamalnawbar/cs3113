#include "MenuScene.h"

MenuScene::MenuScene()                                      : Scene{ {0.0f}, nullptr   } {}
MenuScene::MenuScene(Vector2 origin, const char *bgHexCode) : Scene{ origin, bgHexCode } {}

MenuScene::~MenuScene() { shutdown(); }

void MenuScene::initialise()
{
    mGameState.nextSceneID = NO_SCENE;

    mGameState.player   = nullptr;
    mGameState.enemies  = nullptr;
    mGameState.map      = nullptr;
    mGameState.lives    = 3;
}

void MenuScene::update(float deltaTime)
{
    if (IsKeyPressed(KEY_ENTER))
        mGameState.nextSceneID = LEVEL_1;
}

void MenuScene::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    // titre
    DrawText(
        "Rise of the AI",
        (int) mOrigin.x - 200,
        (int) mOrigin.y - 80,
        60,
        WHITE
    );

    // instructions
    DrawText(
        "Press ENTER to start",
        (int) mOrigin.x - 130,
        (int) mOrigin.y + 20,
        24,
        GRAY
    );
}

void MenuScene::shutdown()
{
    // rien a delete — pas de map, pas de joueur
}
