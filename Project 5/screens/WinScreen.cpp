#include "WinScreen.h"

WinScreen::WinScreen()                                      : Scene{ {0.0f}, nullptr   } {}
WinScreen::WinScreen(Vector2 origin, const char *bgHexCode) : Scene{ origin, bgHexCode } {}
WinScreen::~WinScreen() { shutdown(); }

void WinScreen::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
    mGameState.player      = nullptr;
    mGameState.enemies     = nullptr;
    mGameState.map         = nullptr;
    mGameState.lives       = 0;

    mBgTexture = LoadTexture("assets/WinScreen_bg.png");
}

void WinScreen::update(float deltaTime)
{
    if (IsKeyPressed(KEY_ENTER))
        mGameState.nextSceneID = MENU_SCENE;
}

void WinScreen::render()
{
    ClearBackground({ 10, 20, 10, 255 });

    DrawTexturePro(
        mBgTexture,
        { 0, 0, (float)mBgTexture.width, (float)mBgTexture.height },
        { 0, 0, 1000, 600 },
        { 0, 0 }, 0.0f, WHITE
    );

    DrawRectangle(0, (int)mOrigin.y - 120, 1000, 220, { 0, 0, 0, 170 });

    const char *title = "YOU WIN!";
    DrawText(title,
        (int)mOrigin.x - MeasureText(title, 72) / 2,
        (int)mOrigin.y - 100,
        72, GOLD);

    const char *sub = "The ranch is safe. The cowboy triathlon is complete.";
    DrawText(sub,
        (int)mOrigin.x - MeasureText(sub, 22) / 2,
        (int)mOrigin.y + 10,
        22, WHITE);

    const char *prompt = "ENTER to return to menu";
    DrawText(prompt,
        (int)mOrigin.x - MeasureText(prompt, 20) / 2,
        (int)mOrigin.y + 60,
        20, LIGHTGRAY);
}

void WinScreen::shutdown()
{
    UnloadTexture(mBgTexture);
    mBgTexture = {};
}