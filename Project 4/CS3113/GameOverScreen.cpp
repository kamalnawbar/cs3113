#include "GameOverScreen.h"

GameOverScreen::GameOverScreen()                                      : Scene{ {0.0f}, nullptr   } {}
GameOverScreen::GameOverScreen(Vector2 origin, const char *bgHexCode) : Scene{ origin, bgHexCode } {}

GameOverScreen::~GameOverScreen() { shutdown(); }

void GameOverScreen::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
    mGameState.player      = nullptr;
    mGameState.enemies     = nullptr;
    mGameState.map         = nullptr;
    mGameState.lives       = 3; // remet les vies a zero pour la prochaine partie
}

void GameOverScreen::update(float deltaTime)
{
    if (IsKeyPressed(KEY_ENTER))
        mGameState.nextSceneID = MENU_SCENE;
}

void GameOverScreen::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    const char *title = "GAME OVER";
    int titleSize     = 72;
    int titleWidth    = MeasureText(title, titleSize);
    DrawText(title, (int)mOrigin.x - titleWidth / 2, (int)mOrigin.y - 70, titleSize, RED);

    const char *sub  = "Press ENTER to return to menu";
    int subSize      = 24;
    int subWidth     = MeasureText(sub, subSize);
    DrawText(sub, (int)mOrigin.x - subWidth / 2, (int)mOrigin.y + 30, subSize, GRAY);
}

void GameOverScreen::shutdown()
{
    // rien a supprimer
}