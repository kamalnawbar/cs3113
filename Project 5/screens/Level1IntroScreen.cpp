#include "Level1IntroScreen.h"

extern Music gBgm;

Level1IntroScreen::Level1IntroScreen()                                      : Scene{ {0.0f}, nullptr   } {}
Level1IntroScreen::Level1IntroScreen(Vector2 origin, const char *bgHexCode) : Scene{ origin, bgHexCode } {}
Level1IntroScreen::~Level1IntroScreen() { shutdown(); }

void Level1IntroScreen::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
    mGameState.player      = nullptr;
    mGameState.enemies     = nullptr;
    mGameState.map         = nullptr;
    mGameState.lives       = 3;

    mBgTexture = LoadTexture("assets/level1_intro_bg.png");
    SetMusicVolume(gBgm, 0.2f);
}

void Level1IntroScreen::update(float deltaTime)
{
    if (IsKeyPressed(KEY_ENTER))
        mGameState.nextSceneID = LEVEL_1;
}

void Level1IntroScreen::render()
{
    ClearBackground(BLACK);

    DrawTexturePro(
        mBgTexture,
        { 0, 0, (float)mBgTexture.width, (float)mBgTexture.height },
        { 0, 0, 1000, 600 },
        { 0, 0 }, 0.0f, WHITE
    );

    DrawRectangle(0, 80, 1000, 440, { 0, 0, 0, 175 });

    const char *challenge = "CHALLENGE 1";
    DrawText(challenge,
        (int)mOrigin.x - MeasureText(challenge, 62) / 2,
        100, 62, GOLD);

    const char *subtitle = "AIM";
    DrawText(subtitle,
        (int)mOrigin.x - MeasureText(subtitle, 38) / 2,
        172, 38, WHITE);

    DrawLine(200, 220, 800, 220, { 255, 255, 255, 60 });

    const char *l1 = "3 targets are hidden across the canyon.";
    DrawText(l1, (int)mOrigin.x - MeasureText(l1, 22) / 2, 240, 22, LIGHTGRAY);

    const char *l2 = "You only have 5 bullets — make every shot count.";
    DrawText(l2, (int)mOrigin.x - MeasureText(l2, 22) / 2, 274, 22, LIGHTGRAY);

    const char *l3 = "Bullets bounce off walls. Use the angles.";
    DrawText(l3, (int)mOrigin.x - MeasureText(l3, 22) / 2, 308, 22, LIGHTGRAY);

    DrawLine(200, 346, 800, 346, { 255, 255, 255, 60 });

    const char *c1 = "Mouse  —  Aim";
    DrawText(c1, (int)mOrigin.x - MeasureText(c1, 24) / 2, 364, 24, WHITE);

    const char *c2 = "SPACE  —  Shoot";
    DrawText(c2, (int)mOrigin.x - MeasureText(c2, 24) / 2, 396, 24, WHITE);

    const char *prompt = "ENTER to begin";
    DrawText(prompt,
        (int)mOrigin.x - MeasureText(prompt, 20) / 2,
        460, 20, { 180, 180, 180, 200 });
}

void Level1IntroScreen::shutdown()
{
    UnloadTexture(mBgTexture);
    mBgTexture = {};
}