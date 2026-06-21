/**
* Author: [Kamal Nawbar]
* Assignment: Rise of the AI
* Date due: 2026-06-20, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/MenuScene.h"
#include "CS3113/Level1.h"
#include "CS3113/Level2.h"
#include "CS3113/Level3.h"
#include "CS3113/GameOverScreen.h"
#include "CS3113/WinScreen.h"

// Global Constants
constexpr int SCREEN_WIDTH = 1000, SCREEN_HEIGHT = 600, FPS = 120;

constexpr Vector2 ORIGIN = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

// Global Variables
AppStatus gAppStatus     = RUNNING;
float     gPreviousTicks = 0.0f, gTimeAccumulator = 0.0f;

Camera2D gCamera = { 0 };

Scene                     *gCurrentScene = nullptr;
std::map<SceneID, Scene *> gLevels       = {};

// Function Declarations
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

    if (gCurrentScene->getState().player != nullptr)
        gCurrentScene->setLives(savedLives);

    if (gCurrentScene->getStateRef().player != nullptr)
        gCamera.target = gCurrentScene->getStateRef().player->getPosition();
    else
        gCamera.target = ORIGIN;
}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Rise of the AI");
    InitAudioDevice();

    gLevels[MENU_SCENE]       = new MenuScene(ORIGIN, "#1a1a2e");
    gLevels[LEVEL_1]          = new Level1(ORIGIN, "#4a90d9");
    gLevels[LEVEL_2]          = new Level2(ORIGIN, "#1d4a1d");
    gLevels[LEVEL_3]          = new Level3(ORIGIN, "#2d0a3a");
    gLevels[WIN_SCREEN]       = new WinScreen(ORIGIN, "#1a1a2e");
    gLevels[GAME_OVER_SCREEN] = new GameOverScreen(ORIGIN, "#1a0000");

    switchToScene(gLevels[MENU_SCENE]);

    gCamera.offset   = ORIGIN;
    gCamera.rotation = 0.0f;
    gCamera.zoom     = 1.0f;

    SetTargetFPS(FPS);
}

void processInput()
{
    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;


    // if (IsKeyPressed(KEY_ONE)   && gLevels[LEVEL_1] != nullptr) switchToScene(gLevels[LEVEL_1]);
    // if (IsKeyPressed(KEY_TWO)   && gLevels[LEVEL_2] != nullptr) switchToScene(gLevels[LEVEL_2]);
    // if (IsKeyPressed(KEY_THREE) && gLevels[LEVEL_3] != nullptr) switchToScene(gLevels[LEVEL_3]);

    
    if (gCurrentScene->getStateRef().player == nullptr) return;

    gCurrentScene->getStateRef().player->resetMovement();

    if (IsKeyDown(KEY_A))
        gCurrentScene->getStateRef().player->moveLeft();
    else if (IsKeyDown(KEY_D))
        gCurrentScene->getStateRef().player->moveRight();

    if (IsKeyPressed(KEY_W) &&
        gCurrentScene->getStateRef().player->isCollidingBottom())
    {
        gCurrentScene->getStateRef().player->jump();
        PlaySound(gCurrentScene->getStateRef().jumpSound);
    }

    if (GetLength(gCurrentScene->getStateRef().player->getMovement()) > 1.0f)
        gCurrentScene->getStateRef().player->normaliseMovement();
}

void update()
{

    if (gCurrentScene->getStateRef().player != nullptr)
        UpdateMusicStream(gCurrentScene->getStateRef().bgm);

    float ticks     = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

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

        if (gCurrentScene->getStateRef().player != nullptr)
        {
            Vector2 currentPlayerPosition = {
                gCurrentScene->getStateRef().player->getPosition().x, ORIGIN.y
            };
            panCamera(&gCamera, &currentPlayerPosition);
        }
    }

    gTimeAccumulator = deltaTime;
}

void render()
{
    BeginDrawing();
    BeginMode2D(gCamera);

    gCurrentScene->render();

    EndMode2D();

    if (gCurrentScene->getStateRef().player != nullptr)
        DrawText(
            TextFormat("Lives: %d", gCurrentScene->getStateRef().lives),
            20, 20, 30, WHITE
        );

    EndDrawing();
}

void shutdown()
{
    for (std::pair<const SceneID, Scene *> &entry : gLevels)
        delete entry.second;
    gLevels.clear();

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