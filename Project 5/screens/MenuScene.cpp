#include "MenuScene.h"

extern Music gBgm;

MenuScene::MenuScene()                                      : Scene{ {0.0f}, nullptr   } {}
MenuScene::MenuScene(Vector2 origin, const char *bgHexCode) : Scene{ origin, bgHexCode } {}
MenuScene::~MenuScene() { shutdown(); }

void MenuScene::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
    mGameState.player      = nullptr;
    mGameState.enemies     = nullptr;
    mGameState.map         = nullptr;
    mGameState.lives       = 3;

    mBgTexture = LoadTexture("assets/menuscreen_bg.png");

    SetMusicVolume(gBgm, 0.4f);
}

void MenuScene::update(float deltaTime)
{
    if (IsKeyPressed(KEY_ENTER))
        mGameState.nextSceneID = LEVEL_1_INTRO;
}

void MenuScene::render()
{
    ClearBackground(BLACK);

    DrawTexturePro(
        mBgTexture,
        { 0, 0, (float)mBgTexture.width, (float)mBgTexture.height },
        { 0, 0, 1000, 600 },
        { 0, 0 }, 0.0f, WHITE
    );

    DrawRectangle(0, (int)mOrigin.y - 160, 1000, 240, { 0, 0, 0, 140 });

    const char *title = "COWBOY TRIATHLON";
    DrawText(title,
        (int)mOrigin.x - MeasureText(title, 64) / 2,
        (int)mOrigin.y - 130,
        64, GOLD);

    const char *tagline = "3 challenges. One cowboy. Prove yourself.";
    DrawText(tagline,
        (int)mOrigin.x - MeasureText(tagline, 22) / 2,
        (int)mOrigin.y + 0,
        22, LIGHTGRAY);

    const char *startText = "ENTER to play     Q to quit";
    DrawText(startText,
        (int)mOrigin.x - MeasureText(startText, 24) / 2,
        (int)mOrigin.y + 46,
        24, WHITE);
}

void MenuScene::shutdown()
{
    UnloadTexture(mBgTexture);
    mBgTexture = {};

}