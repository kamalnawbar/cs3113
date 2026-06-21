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
    mGameState.lives       = 3;

    // son de victoire — joue une seule fois a l'arrivee sur l'ecran
    mGameState.jumpSound = LoadSound("assets/audio/game_won.wav");
    PlaySound(mGameState.jumpSound);
}

void WinScreen::update(float deltaTime)
{
    if (IsKeyPressed(KEY_ENTER))
        mGameState.nextSceneID = MENU_SCENE;
}

void WinScreen::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    const char *title = "YOU WIN!";
    int titleSize     = 72;
    int titleWidth    = MeasureText(title, titleSize);
    DrawText(title, (int)mOrigin.x - titleWidth / 2, (int)mOrigin.y - 70, titleSize, GREEN);

    const char *sub  = "Press ENTER to return to menu";
    int subSize      = 24;
    int subWidth     = MeasureText(sub, subSize);
    DrawText(sub, (int)mOrigin.x - subWidth / 2, (int)mOrigin.y + 30, subSize, GRAY);
}

void WinScreen::shutdown()
{
    UnloadSound(mGameState.jumpSound);
    mGameState.jumpSound = {};
}