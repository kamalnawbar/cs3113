#include "Level2IntroScreen.h"

extern Music gBgm;

Level2IntroScreen::Level2IntroScreen()                                      : Scene{ {0.0f}, nullptr   } {}
Level2IntroScreen::Level2IntroScreen(Vector2 origin, const char *bgHexCode) : Scene{ origin, bgHexCode } {}
Level2IntroScreen::~Level2IntroScreen() { shutdown(); }

void Level2IntroScreen::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
    mGameState.player      = nullptr;
    mGameState.enemies     = nullptr;
    mGameState.map         = nullptr;
    mGameState.lives       = 3;

    mBgTexture = LoadTexture("assets/level2_intro_bg.png");
    SetMusicVolume(gBgm, 0.2f);
}

void Level2IntroScreen::update(float deltaTime)
{
    if (IsKeyPressed(KEY_ENTER))
        mGameState.nextSceneID = LEVEL_2;
}

void Level2IntroScreen::render()
{
    ClearBackground(BLACK);

    DrawTexturePro(
        mBgTexture,
        { 0, 0, (float)mBgTexture.width, (float)mBgTexture.height },
        { 0, 0, 1000, 600 },
        { 0, 0 }, 0.0f, WHITE
    );

    DrawRectangle(0, 80, 1000, 440, { 0, 0, 0, 175 });

    const char *challenge = "CHALLENGE 2";
    DrawText(challenge,
        (int)mOrigin.x - MeasureText(challenge, 62) / 2,
        100, 62, GOLD);

    const char *subtitle = "RUN";
    DrawText(subtitle,
        (int)mOrigin.x - MeasureText(subtitle, 38) / 2,
        172, 38, WHITE);

    DrawLine(200, 220, 800, 220, { 255, 255, 255, 60 });

    const char *l1 = "A stampede is charging from the right.";
    DrawText(l1, (int)mOrigin.x - MeasureText(l1, 22) / 2, 240, 22, LIGHTGRAY);

    const char *l2 = "Dodge between lanes and reach the exit.";
    DrawText(l2, (int)mOrigin.x - MeasureText(l2, 22) / 2, 274, 22, LIGHTGRAY);

    const char *l3 = "The horses get faster the closer you get to the exit.";
    DrawText(l3, (int)mOrigin.x - MeasureText(l3, 22) / 2, 308, 22, LIGHTGRAY);

    DrawLine(200, 346, 800, 346, { 255, 255, 255, 60 });

    const char *c1 = "WASD  —  Move";
    DrawText(c1, (int)mOrigin.x - MeasureText(c1, 24) / 2, 364, 24, WHITE);

    const char *c2 = "3 lives  —  Getting hit by a horse costs one";
    DrawText(c2, (int)mOrigin.x - MeasureText(c2, 22) / 2, 398, 22, LIGHTGRAY);

    const char *prompt = "ENTER to begin";
    DrawText(prompt,
        (int)mOrigin.x - MeasureText(prompt, 20) / 2,
        460, 20, { 180, 180, 180, 200 });
}

void Level2IntroScreen::shutdown()
{
    UnloadTexture(mBgTexture);
    mBgTexture = {};
}