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
    mGameState.lives       = 0;

    mBgTexture = LoadTexture("assets/loss_bg.png");
}

void GameOverScreen::update(float deltaTime)
{
    if (IsKeyPressed(KEY_ENTER))
        mGameState.nextSceneID = LEVEL_1;
}

void GameOverScreen::render()
{
    ClearBackground({ 20, 5, 5, 255 });

    DrawTexturePro(
        mBgTexture,
        { 0, 0, (float)mBgTexture.width, (float)mBgTexture.height },
        { 0, 0, 1000, 600 },
        { 0, 0 }, 0.0f, WHITE
    );

    DrawRectangle(0, (int)mOrigin.y - 120, 1000, 220, { 0, 0, 0, 170 });

    const char *title = "GAME OVER";
    DrawText(title,
        (int)mOrigin.x - MeasureText(title, 72) / 2,
        (int)mOrigin.y - 100,
        72, RED);

    const char *sub = "You didn't make it, cowboy. Try again.";
    DrawText(sub,
        (int)mOrigin.x - MeasureText(sub, 22) / 2,
        (int)mOrigin.y + 10,
        22, WHITE);

    const char *prompt = "ENTER to restart from challenge 1";
    DrawText(prompt,
        (int)mOrigin.x - MeasureText(prompt, 20) / 2,
        (int)mOrigin.y + 60,
        20, LIGHTGRAY);
}

void GameOverScreen::shutdown()
{
    UnloadTexture(mBgTexture);
    mBgTexture = {};
}