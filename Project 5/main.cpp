/**
* Author: Kamal Nawbar
* Assignment: Cowboy Thriathlon
* Date due: [06/30/2026], 2:00pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "screens/MenuScene.h"
#include "screens/Level1IntroScreen.h"
#include "CS3113/Level1.h"
#include "screens/Level2IntroScreen.h"
#include "CS3113/Level2.h"
#include "screens/Level3IntroScreen.h"
#include "CS3113/Level3.h"
#include "screens/WinScreen.h"
#include "screens/GameOverScreen.h"

constexpr int SCREEN_WIDTH  = 1000,
              SCREEN_HEIGHT = 600,
              FPS           = 120;

constexpr Vector2 ORIGIN = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

AppStatus gAppStatus     = RUNNING;
float     gPreviousTicks = 0.0f,
          gTimeAccumulator = 0.0f;

Camera2D gCamera = { 0 };
Music    gBgm    = {};

Scene                     *gCurrentScene = nullptr;
std::map<SceneID, Scene *> gLevels       = {};

void switchToScene(Scene *scene);
void initialise();
void processInput();
void update();
void render();
void shutdown();

void switchToScene(Scene *scene)
{
    if (scene == nullptr) return;

    int savedLives = gCurrentScene ? gCurrentScene->getState().lives : 3;

    if (gCurrentScene) gCurrentScene->shutdown();

    gCurrentScene = scene;
    gCurrentScene->initialise();

    if (gCurrentScene->getStateRef().player != nullptr)
        gCurrentScene->setLives(savedLives);

    gCamera.target = ORIGIN;
}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Cowboy Triathlon");
    InitAudioDevice();

    gBgm = LoadMusicStream("assets/audio/bgm.mp3");
    SetMusicVolume(gBgm, 0.26f);
    PlayMusicStream(gBgm);

    gLevels[MENU_SCENE]       = new MenuScene(ORIGIN, "#000000");
    gLevels[LEVEL_1_INTRO]    = new Level1IntroScreen(ORIGIN, "#000000");
    gLevels[LEVEL_1]          = new Level1(ORIGIN, "#000000");
    gLevels[LEVEL_2_INTRO]    = new Level2IntroScreen(ORIGIN, "#000000");
    gLevels[LEVEL_2]          = new Level2(ORIGIN, "#000000");
    gLevels[LEVEL_3_INTRO]    = new Level3IntroScreen(ORIGIN, "#000000");
    gLevels[LEVEL_3]          = new Level3(ORIGIN, "#000000");
    gLevels[WIN_SCREEN]       = new WinScreen(ORIGIN, "#000000");
    gLevels[GAME_OVER_SCREEN] = new GameOverScreen(ORIGIN, "#000000");

    switchToScene(gLevels[MENU_SCENE]);

    gCamera.offset   = ORIGIN;
    gCamera.rotation = 0.0f;
    gCamera.zoom     = 1.0f;

    SetTargetFPS(FPS);
}

void processInput()
{
    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;

    if (IsKeyPressed(KEY_ONE))   switchToScene(gLevels[LEVEL_1]);
    if (IsKeyPressed(KEY_TWO))   switchToScene(gLevels[LEVEL_2]);
    if (IsKeyPressed(KEY_THREE)) switchToScene(gLevels[LEVEL_3]);
}

void update()
{
    UpdateMusicStream(gBgm);

    float ticks      = (float) GetTime();
    float deltaTime  = ticks - gPreviousTicks;
    gPreviousTicks   = ticks;

    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP)
    {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP)
    {
        gCurrentScene->update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
    }

    gTimeAccumulator = deltaTime;
}

void render()
{
    BeginDrawing();
    BeginMode2D(gCamera);

    gCurrentScene->render();

    EndMode2D();
    EndDrawing();
}

void shutdown()
{
    for (std::pair<const SceneID, Scene *> &entry : gLevels)
        delete entry.second;
    gLevels.clear();

    UnloadMusicStream(gBgm);
    CloseAudioDevice();
    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();

        if (gCurrentScene->getStateRef().nextSceneID != NO_SCENE)
        {
            SceneID id = gCurrentScene->getStateRef().nextSceneID;
            switchToScene(gLevels[id]);
        }

        render();
    }

    shutdown();

    return 0;
}