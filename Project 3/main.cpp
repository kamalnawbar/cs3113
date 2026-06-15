/**
* Author: [Kamal Nawbar]
* Assignment: Lunar Lander
* Date due: [06/14/2026]
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/Entity.h"

// Global Constants
constexpr int SCREEN_WIDTH  = 800 * 1.5f,
              SCREEN_HEIGHT = 450 * 1.5f,
              FPS           = 60,
              PLATFORM_COUNT      = 10,
              SAFE_PLATFORM_IDX   = 0,
              FORBIDDEN_1_IDX     = 1,
              MOVING_PLATFORM_IDX = 9,
              FRAMES_PER_SECOND   = 8;

constexpr float FIXED_TIMESTEP          = 1.0f / 60.0f,
                ACCELERATION_OF_GRAVITY = 20.0f,
                THRUST_POWER            = 70.0f,
                HORIZONTAL_ACCELERATION = 10.0f,
                FUEL_CONSUMPTION_RATE   = 15.0f,
                MAX_FUEL                = 300.0f,
                PLAYER_WIDTH            = 80.0f,
                PLAYER_HEIGHT           = 100.0f,
                PLATFORM_HEIGHT         = 55.0f,
                MOVING_PLATFORM_WIDTH   = 80.0f,
                MOVING_PLATFORM_HEIGHT  = 40.0f,
                MOVING_PLATFORM_SPEED   = 220.0f,
                MOVING_PLATFORM_MIN_X   = 740.0f,
                MOVING_PLATFORM_MAX_X   = 810.0f;

constexpr char BG_COLOUR[] = "#0D0D1A";
constexpr Vector2 ORIGIN = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };

constexpr char ASTRONAUT_FP[]          = "assets/jetpack.png";
constexpr char SPACE_BACKGROUND_FP[]   = "assets/space_background.png";
constexpr char PLATFORM_SAFE_FP[]      = "assets/platform_safe.png";
constexpr char PLATFORM_FORBIDDEN_FP[] = "assets/platform_forbidden.png";
constexpr char PLATFORM_MOVING_FP[]    = "assets/platform_moving.png";

// Enums
enum GameStatus { PLAYING, MISSION_ACCOMPLISHED, MISSION_FAILED };

// Global Variables
AppStatus  gAppStatus  = RUNNING;
GameStatus gGameStatus = PLAYING;
float gPreviousTicks   = 0.0f,
      gTimeAccumulator = 0.0f,
      gFuel            = MAX_FUEL;
bool  gThrusting         = false;
bool  gMovingHorizontal  = false;
float gMovingPlatformDir = 1.0f;

Texture2D gBackground;

Entity *gPlayer    = nullptr;
Entity *gPlatforms = nullptr; // just fait 1-8 forb e 9 moving

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();

// Function Definitions
void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Lunar Lander");

    gBackground = LoadTexture(SPACE_BACKGROUND_FP);

    gPlayer = new Entity({ ORIGIN.x, 80.0f }, { PLAYER_WIDTH, PLAYER_HEIGHT }, ASTRONAUT_FP);
    gPlayer->setColliderDimensions({ PLAYER_WIDTH * 0.5f, PLAYER_HEIGHT * 0.7f });
    gPlayer->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });
    gPlayer->setSpritesheetDimensions(5, 2);
    gPlayer->setAnimationFrames({ 0, 1, 2 });
    gPlayer->setFramesPerSecond(FRAMES_PER_SECOND);

    gPlatforms = new Entity[PLATFORM_COUNT] {
        Entity({  425.0f, 600.0f }, { 100.0f,PLATFORM_HEIGHT}, PLATFORM_SAFE_FP),  
        Entity({  187.0f, 600.0f }, { 374.0f,PLATFORM_HEIGHT}, PLATFORM_FORBIDDEN_FP),
        Entity({  837.0f, 600.0f }, { 726.0f, PLATFORM_HEIGHT}, PLATFORM_FORBIDDEN_FP),  
        Entity({  175.0f, 440.0f }, { 350.0f,PLATFORM_HEIGHT }, PLATFORM_FORBIDDEN_FP),  
        Entity({  600.0f, 440.0f }, { 200.0f, PLATFORM_HEIGHT}, PLATFORM_FORBIDDEN_FP), 
        Entity({ 1025.0f, 440.0f }, { 350.0f,PLATFORM_HEIGHT}, PLATFORM_FORBIDDEN_FP), 
        Entity({  600.0f, 290.0f }, { 400.0f, PLATFORM_HEIGHT}, PLATFORM_FORBIDDEN_FP),  
        Entity({   75.0f, 340.0f }, { 150.0f,PLATFORM_HEIGHT }, PLATFORM_FORBIDDEN_FP),  
        Entity({ 1125.0f, 340.0f }, { 150.0f, PLATFORM_HEIGHT}, PLATFORM_FORBIDDEN_FP), 
        Entity({  775.0f, 440.0f }, { MOVING_PLATFORM_WIDTH,  MOVING_PLATFORM_HEIGHT }, PLATFORM_MOVING_FP) 
    };

    // spike hitbox was too big
    for (int i = FORBIDDEN_1_IDX; i < MOVING_PLATFORM_IDX; i++)
        gPlatforms[i].setColliderDimensions({ gPlatforms[i].getScale().x * 0.92f, PLATFORM_HEIGHT * 0.45f });

    SetTargetFPS(FPS);
}

void processInput()
{
    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;

    if (gGameStatus != PLAYING) return;

    bool wasThrustingBefore = gThrusting;
    gThrusting = IsKeyDown(KEY_SPACE) && gFuel > 0.0f;

    float xAccel = 0.0f;
    gMovingHorizontal = false;
    if (IsKeyDown(KEY_A) && gFuel > 0.0f) { xAccel = -HORIZONTAL_ACCELERATION; gMovingHorizontal = true; }
    else if (IsKeyDown(KEY_D) && gFuel > 0.0f) { xAccel =  HORIZONTAL_ACCELERATION; gMovingHorizontal = true; }
    else xAccel = -gPlayer->getVelocity().x * 2.0f;

    gPlayer->setAcceleration({ xAccel, gThrusting ? ACCELERATION_OF_GRAVITY - THRUST_POWER : ACCELERATION_OF_GRAVITY });

    if (gThrusting != wasThrustingBefore) {
        if (gThrusting) gPlayer->setAnimationFrames({ 5, 6, 7, 8, 9 });
        else gPlayer->setAnimationFrames({ 0, 1, 2 });
    }
}

void update()
{
    float ticks     = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP)
    { gTimeAccumulator = deltaTime; return; }

    while (deltaTime >= FIXED_TIMESTEP)
    {
        if (gGameStatus != PLAYING)
        { deltaTime -= FIXED_TIMESTEP; continue; }

        float newX = gPlatforms[MOVING_PLATFORM_IDX].getPosition().x + MOVING_PLATFORM_SPEED * gMovingPlatformDir * FIXED_TIMESTEP;
        gPlatforms[MOVING_PLATFORM_IDX].setPosition({ newX, gPlatforms[MOVING_PLATFORM_IDX].getPosition().y });

        if (newX > MOVING_PLATFORM_MAX_X) gMovingPlatformDir = -1.0f;
        if (newX < MOVING_PLATFORM_MIN_X) gMovingPlatformDir =  1.0f;

        gPlayer->update(FIXED_TIMESTEP, gPlatforms, PLATFORM_COUNT);


        // screen boundaries
        float halfW = gPlayer->getColliderDimensions().x / 2.0f;
        float halfH = gPlayer->getColliderDimensions().y / 2.0f;
        Vector2 pos = gPlayer->getPosition();
        Vector2 vel = gPlayer->getVelocity();

        if (pos.x - halfW < 0.0f)
        { gPlayer->setPosition({ halfW, pos.y }); gPlayer->setVelocity({ 0.0f, vel.y }); }
        else if (pos.x + halfW > SCREEN_WIDTH)
        { gPlayer->setPosition({ (float) SCREEN_WIDTH - halfW, pos.y }); gPlayer->setVelocity({ 0.0f, vel.y }); }

        if (pos.y - halfH < 0.0f)
        { gPlayer->setPosition({ pos.x, halfH }); gPlayer->setVelocity({ vel.x, 0.0f }); }

        if (gThrusting || gMovingHorizontal)
        { gFuel -= FUEL_CONSUMPTION_RATE * FIXED_TIMESTEP;
          if (gFuel < 0.0f) gFuel = 0.0f; }

        if (gPlayer->isCollidingBottom())
        {
            for (int i = 0; i < PLATFORM_COUNT; i++) // i know its ugly but needed this to know which plat i hit
            {
                float playerBottom  = gPlayer->getPosition().y + gPlayer->getColliderDimensions().y / 2.0f;
                float platformTop   = gPlatforms[i].getPosition().y - gPlatforms[i].getColliderDimensions().y / 2.0f;
                float playerLeft    = gPlayer->getPosition().x - gPlayer->getColliderDimensions().x / 2.0f;
                float playerRight   = gPlayer->getPosition().x + gPlayer->getColliderDimensions().x / 2.0f;
                float platformLeft  = gPlatforms[i].getPosition().x - gPlatforms[i].getColliderDimensions().x / 2.0f;
                float platformRight = gPlatforms[i].getPosition().x + gPlatforms[i].getColliderDimensions().x / 2.0f;

                bool vertTouching  = fabs(playerBottom - platformTop) < 5.0f;
                bool horizOverlap  = playerRight > platformLeft && playerLeft < platformRight;
                bool playerIsAbove = gPlayer->getPosition().y < gPlatforms[i].getPosition().y;

                if (vertTouching && horizOverlap && playerIsAbove) {
                    gGameStatus = (i == SAFE_PLATFORM_IDX || i == MOVING_PLATFORM_IDX) ? MISSION_ACCOMPLISHED : MISSION_FAILED;
                    gPlayer->setVelocity({ 0.0f, 0.0f });
                    gPlayer->setAcceleration({ 0.0f, 0.0f });
                }
            }
        }

        deltaTime -= FIXED_TIMESTEP;
    }

    gTimeAccumulator = deltaTime;
}

void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));

    DrawTexturePro(
        gBackground,
        { 0.0f, 0.0f, (float) gBackground.width, (float) gBackground.height },
        { 0.0f, 0.0f, (float) SCREEN_WIDTH,       (float) SCREEN_HEIGHT      },
        { 0.0f, 0.0f },
        0.0f, WHITE
    );

    for (int i = 0; i < PLATFORM_COUNT; i++) gPlatforms[i].render();
    gPlayer->render();

    DrawRectangle(20, 20, 200, 18, DARKGRAY);
    DrawRectangle(20, 20, (int)(200.0f * gFuel / MAX_FUEL), 18, LIME);
    DrawText("FUEL", 20, 42, 18, WHITE);

    if (gGameStatus == MISSION_ACCOMPLISHED)
        DrawText("Mission Accomplished", SCREEN_WIDTH / 2 - 225, SCREEN_HEIGHT / 2 - 30, 50, GREEN);
    else if (gGameStatus == MISSION_FAILED)
        DrawText("Mission Failed", SCREEN_WIDTH / 2 - 165, SCREEN_HEIGHT / 2 - 30, 50, RED);

    EndDrawing();
}

void shutdown()
{
    CloseWindow(); // Close window and OpenGL context
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}