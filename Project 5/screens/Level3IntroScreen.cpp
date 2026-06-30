#include "Level3IntroScreen.h"

extern Music gBgm;

Level3IntroScreen::Level3IntroScreen()                                      : Scene{ {0.0f}, nullptr   } {}
Level3IntroScreen::Level3IntroScreen(Vector2 origin, const char *bgHexCode) : Scene{ origin, bgHexCode } {}
Level3IntroScreen::~Level3IntroScreen() { shutdown(); }

void Level3IntroScreen::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
    mGameState.player      = nullptr;
    mGameState.enemies     = nullptr;
    mGameState.map         = nullptr;
    mGameState.lives       = 3;

    mBgTexture = LoadTexture("assets/level3_intro_bg.png");
    SetMusicVolume(gBgm, 0.2f);
}

void Level3IntroScreen::update(float deltaTime)
{
    if (IsKeyPressed(KEY_ENTER))
        mGameState.nextSceneID = LEVEL_3;
}

void Level3IntroScreen::render()
{
    ClearBackground(BLACK);

    DrawTexturePro(
        mBgTexture,
        { 0, 0, (float)mBgTexture.width, (float)mBgTexture.height },
        { 0, 0, 1000, 600 },
        { 0, 0 }, 0.0f, WHITE
    );

    DrawRectangle(0, 55, 1000, 500, { 0, 0, 0, 185 });

    const char *challenge = "CHALLENGE 3";
    DrawText(challenge,
        (int)mOrigin.x - MeasureText(challenge, 60) / 2,
        68, 60, GOLD);

    const char *subtitle = "SURVIVAL";
    DrawText(subtitle,
        (int)mOrigin.x - MeasureText(subtitle, 36) / 2,
        138, 36, WHITE);

    DrawLine(140, 184, 860, 184, { 255, 255, 255, 55 });

    const char *prev = "You mastered the aim. You outran the stampede. Now defend your ranch.";
    DrawText(prev, (int)mOrigin.x - MeasureText(prev, 19) / 2, 196, 19, { 180, 180, 180, 255 });

    DrawLine(140, 224, 860, 224, { 255, 255, 255, 55 });

    DrawText("WAVE 1", 160, 236, 20, RED);
    DrawText("Wolf packs — fast, 1-shot kills, come in tight groups.", 160 + MeasureText("WAVE 1  ", 20), 236, 20, LIGHTGRAY);

    DrawText("WAVE 2", 160, 262, 20, RED);
    DrawText("Strong bears — slow and on the ground, need 2 shots each.", 160 + MeasureText("WAVE 2  ", 20), 262, 20, LIGHTGRAY);

    DrawText("WAVE 3", 160, 288, 20, RED);
    DrawText("All at once — wolves, bears, and unpredictable flying birds.", 160 + MeasureText("WAVE 3  ", 20), 288, 20, LIGHTGRAY);

    DrawLine(140, 320, 860, 320, { 255, 255, 255, 55 });

    DrawRectangle(140, 328, 720, 84, { 180, 20, 20, 55 });
    const char *rule1 = "You have 3 lives.";
    DrawText(rule1, (int)mOrigin.x - MeasureText(rule1, 24) / 2, 336, 24, { 255, 100, 100, 255 });
    const char *rule2 = "Lose one if an enemy hits you OR reaches your ranch.";
    DrawText(rule2, (int)mOrigin.x - MeasureText(rule2, 20) / 2, 368, 20, LIGHTGRAY);
    const char *rule3 = "0 lives — game over.";
    DrawText(rule3, (int)mOrigin.x - MeasureText(rule3, 18) / 2, 394, 18, { 160, 160, 160, 200 });

    DrawLine(140, 416, 860, 416, { 255, 255, 255, 55 });

    const char *c1 = "WASD — Move     Mouse — Aim     SPACE — Shoot";
    DrawText(c1, (int)mOrigin.x - MeasureText(c1, 20) / 2, 428, 20, WHITE);
    const char *c2 = "Ammo reloads automatically when empty.";
    DrawText(c2, (int)mOrigin.x - MeasureText(c2, 18) / 2, 456, 18, { 160, 160, 160, 255 });

    const char *prompt = "ENTER to begin";
    DrawText(prompt,
        (int)mOrigin.x - MeasureText(prompt, 20) / 2,
        510, 20, { 160, 160, 160, 200 });
}

void Level3IntroScreen::shutdown()
{
    UnloadTexture(mBgTexture);
    mBgTexture = {};
}