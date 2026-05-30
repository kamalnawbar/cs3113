/**
* Author: [Kamal Nawbar]
* Assignment: And I Do Make Art, Thank You Very Much
* Date due: [05/30/2026]
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/cs3113.h"
#include <math.h>

constexpr int   SCREEN_WIDTH  = 1600,
                SCREEN_HEIGHT = 900,
                FPS           = 60;

constexpr float MAX_AMP       = 80.0f;

constexpr Vector2 ORIGIN      = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

constexpr char FLAG_FP[]      = "assets/Flag_of_Lebanon.png";
constexpr char CEDAR_FP[]     = "assets/Lebanese_Cedar_Larousse_Extracted.png";
constexpr char CHARACTER_FP[] = "assets/character-r.png";


AppStatus gAppStatus     = RUNNING;

float     gAngle         = 0.0f,
          gPulseTime     = 0.0f;

float     gPreviousTicks = 0.0f;

Vector2   gCedarPosition     = ORIGIN;
Vector2   gFlagPosition      = ORIGIN;
Vector2   gCharacterPosition = ORIGIN;

Vector2   gCedarScale     = { 300.0f, 300.0f };
Vector2   gFlagScale      = { 260.0f, 160.0f };
Vector2   gCharacterScale = { 100.0f, 100.0f };

Texture2D gFlagTexture;
Texture2D gCedarTexture;
Texture2D gCharacterTexture;

void initialise();
void processInput();
void update();
void render();
void shutdown();


void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Lebanese Theme");

    gFlagTexture      = LoadTexture(FLAG_FP);
    gCedarTexture     = LoadTexture(CEDAR_FP);
    gCharacterTexture = LoadTexture(CHARACTER_FP);

    SetTargetFPS(FPS);
}

void processInput() 
{
    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() 
{
    float ticks = static_cast<float>(GetTime());
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;

    gPulseTime += 2.5f * deltaTime;

    gCedarPosition = {
        ORIGIN.x + 120.0f * cos(gPulseTime),
        ORIGIN.y + 80.0f  * cos(2.0f * gPulseTime)
    };

    gFlagPosition = {
    gCedarPosition.x + 250.0f * cos(1.5f * gPulseTime),
    gCedarPosition.y + 150.0f * cos(2.0f * gPulseTime)
    };

    gCharacterPosition = {
    gFlagPosition.x + 100.0f * cos(2.5f * gPulseTime),
    gFlagPosition.y + 75.0f * cos(3.0f * gPulseTime)
    };

    gCedarScale = {
        300.0f + MAX_AMP * cos(gPulseTime),
        300.0f + MAX_AMP * cos(gPulseTime)
    };

    gAngle += 50.0f * deltaTime;
}

void render()
{
    BeginDrawing();

    Color backgroundColour = {
        static_cast<unsigned char>(120 + 60 * cos(gPulseTime)),
        static_cast<unsigned char>(170 + 60 * cos(gPulseTime)),
        static_cast<unsigned char>(220 + 30 * cos(gPulseTime)),
        255
    };

    ClearBackground(backgroundColour);

    Rectangle cedarTextureArea = {
        0.0f, 0.0f,
        static_cast<float>(gCedarTexture.width),
        static_cast<float>(gCedarTexture.height)
    };

    Rectangle cedarDestinationArea = {
        gCedarPosition.x,
        gCedarPosition.y,
        gCedarScale.x,
        gCedarScale.y
    };

    Vector2 cedarOrigin = {
        gCedarScale.x / 2.0f,
        gCedarScale.y / 2.0f
    };

    DrawTexturePro(
        gCedarTexture,
        cedarTextureArea,
        cedarDestinationArea,
        cedarOrigin,
        0.0f,
        WHITE
    );

    Rectangle flagTextureArea = {
        0.0f, 0.0f,
        static_cast<float>(gFlagTexture.width),
        static_cast<float>(gFlagTexture.height)
    };

    Rectangle flagDestinationArea = {
        gFlagPosition.x,
        gFlagPosition.y,
        gFlagScale.x,
        gFlagScale.y
    };

    Vector2 flagOrigin = {
        gFlagScale.x / 2.0f,
        gFlagScale.y / 2.0f
    };

    DrawTexturePro(
        gFlagTexture,
        flagTextureArea,
        flagDestinationArea,
        flagOrigin,
        gAngle,
        WHITE
    );

    Rectangle characterTextureArea = {
        0.0f, 0.0f,
        static_cast<float>(gCharacterTexture.width),
        static_cast<float>(gCharacterTexture.height)
    };

    Rectangle characterDestinationArea = {
        gCharacterPosition.x,
        gCharacterPosition.y,
        gCharacterScale.x,
        gCharacterScale.y
    };

    Vector2 characterOrigin = {
        gCharacterScale.x / 2.0f,
        gCharacterScale.y / 2.0f
    };

    DrawTexturePro(
        gCharacterTexture,
        characterTextureArea,
        characterDestinationArea,
        characterOrigin,
        0.0f,
        WHITE
    );

    EndDrawing();
}

void shutdown() 
{ 
    CloseWindow(); 

    UnloadTexture(gFlagTexture);
    UnloadTexture(gCedarTexture);
    UnloadTexture(gCharacterTexture);
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