#include "CS3113/Entity.h"

// Global Constants
constexpr int SCREEN_WIDTH  = 800 * 1.5f,
              SCREEN_HEIGHT = 450 * 1.5f,
              FPS           = 60,
              MAX_BALLS     = 3;

constexpr float PADDLE_WIDTH  = 80.0f,
                PADDLE_HEIGHT = 200.0f,
                BALL_SIZE     = 60.0f,
                PADDLE_SPEED  = 300.0f,
                AI_SPEED      = 200.0f,
                BALL_SPEED    = 350.0f;

constexpr int MESSI_WIN_SCORE   = 7,
              RONALDO_MAX_SCORE = 5;

constexpr char    BG_COLOUR[] = "#4A7C2F";
constexpr Vector2 ORIGIN      = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };

constexpr char MESSI_FP[]     = "assets/messi.png";
constexpr char CRISTIANO_FP[] = "assets/cristiano.png";
constexpr char BALL_FP[]      = "assets/ball.png";

// Enums
enum GameMode { TWO_PLAYER, ONE_PLAYER };

// Global Variables
AppStatus gAppStatus     = RUNNING;
GameMode  gGameMode      = TWO_PLAYER;
float     gPreviousTicks = 0.0f;

Entity *gPaddles = nullptr;  
Entity *gBalls   = nullptr;  

int   gActiveBalls  = 1;
int   gMessiScore   = 0;
int   gRonaldoScore = 0;
bool  gGameOver     = false;
float gAIDirection  = 1.0f;  

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();

// Function Definitions
void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Messi vs Cristiano");

    gPaddles = new Entity[2] {
        Entity({ PADDLE_WIDTH,ORIGIN.y }, { PADDLE_WIDTH, PADDLE_HEIGHT }, MESSI_FP),
        Entity({ SCREEN_WIDTH - PADDLE_WIDTH, ORIGIN.y }, { PADDLE_WIDTH, PADDLE_HEIGHT }, CRISTIANO_FP)
    };

    gBalls = new Entity[MAX_BALLS] {
        Entity(ORIGIN, { BALL_SIZE, BALL_SIZE }, BALL_FP),
        Entity(ORIGIN, { BALL_SIZE, BALL_SIZE }, BALL_FP),
        Entity(ORIGIN, { BALL_SIZE, BALL_SIZE }, BALL_FP)
    };

    gBalls[0].setVelocity({  BALL_SPEED,  BALL_SPEED * 0.6f });
    gBalls[1].setVelocity({ -BALL_SPEED,  BALL_SPEED * 0.5f });
    gBalls[2].setVelocity({  BALL_SPEED, -BALL_SPEED * 0.4f });

    gBalls[1].deactivate();
    gBalls[2].deactivate();

    SetTargetFPS(FPS);
}

void processInput()
{
    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;

    if (gGameOver) return;

    if (IsKeyPressed(KEY_T)) gGameMode = (gGameMode == TWO_PLAYER) ? ONE_PLAYER : TWO_PLAYER;


    if (IsKeyPressed(KEY_ONE)) gActiveBalls = 1;
    else if (IsKeyPressed(KEY_TWO)) gActiveBalls = 2;
    else if (IsKeyPressed(KEY_THREE)) gActiveBalls = 3;

    for (int i = 0; i < MAX_BALLS; i++)
    { if (i < gActiveBalls) gBalls[i].activate();
      else gBalls[i].deactivate(); }

    
    gPaddles[0].setVelocity({ 0.0f, 0.0f });
    if (IsKeyDown(KEY_W)) gPaddles[0].setVelocity({ 0.0f, -PADDLE_SPEED });
    else if (IsKeyDown(KEY_S)) gPaddles[0].setVelocity({ 0.0f, PADDLE_SPEED });


    if (gGameMode == TWO_PLAYER)
    { gPaddles[1].setVelocity({ 0.0f, 0.0f });
      if (IsKeyDown(KEY_UP)) gPaddles[1].setVelocity({ 0.0f, -PADDLE_SPEED });
      else if (IsKeyDown(KEY_DOWN)) gPaddles[1].setVelocity({ 0.0f, PADDLE_SPEED });}
}

void update()
{
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    if (gGameOver) return;

    if (gGameMode == ONE_PLAYER)
    { gPaddles[1].setVelocity({ 0.0f, gAIDirection * AI_SPEED });
      float cristianoY = gPaddles[1].getPosition().y;
      if(cristianoY <= PADDLE_HEIGHT / 2.0f) gAIDirection =  1.0f;
      else if (cristianoY >= SCREEN_HEIGHT - PADDLE_HEIGHT / 2.0f)  gAIDirection = -1.0f;}

    gPaddles[0].update(deltaTime, nullptr, 0);
    gPaddles[1].update(deltaTime, nullptr, 0);

    gPaddles[0].clampToScreen(SCREEN_HEIGHT);
    gPaddles[1].clampToScreen(SCREEN_HEIGHT);

    for (int i = 0; i < MAX_BALLS; i++)
    {gBalls[i].update(deltaTime, gPaddles, 2);
     gBalls[i].bounceOffWalls(SCREEN_HEIGHT);
     Vector2 pos = gBalls[i].getPosition();
     if (pos.x < -BALL_SIZE){
        if (gRonaldoScore < MESSI_WIN_SCORE) gRonaldoScore++;
            gBalls[i].setPosition(ORIGIN);
            gBalls[i].setVelocity({ BALL_SPEED, BALL_SPEED * 0.6f });}
       
     else if (pos.x > SCREEN_WIDTH + BALL_SIZE){
        gMessiScore++;
        if (gMessiScore >= MESSI_WIN_SCORE) gGameOver = true;
            gBalls[i].setPosition(ORIGIN);
            gBalls[i].setVelocity({ -BALL_SPEED, BALL_SPEED * 0.6f });
        }
    }
}

void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));

    gPaddles[0].render();
    gPaddles[1].render();

    for (int i = 0; i < MAX_BALLS; i++) gBalls[i].render();
    if (gRonaldoScore <= RONALDO_MAX_SCORE) {
        DrawText(TextFormat("Messi: %d", gMessiScore), 20, 20, 30, WHITE);
        DrawText(TextFormat("Ronaldo: %d", gRonaldoScore), SCREEN_WIDTH - 220, 20, 30, WHITE);
    }
    else {
        DrawText(TextFormat("Messi: %d", gMessiScore), 20, 20, 30, WHITE);
        DrawText("Ronaldo: ", SCREEN_WIDTH - 220, 20, 30, WHITE);
        DrawText("CAP", SCREEN_WIDTH - 220 + 130, 20, 30, RED);
    }
   

    if (gGameMode == ONE_PLAYER)
        DrawText("1-Player Mode", SCREEN_WIDTH / 2 - 85, 20, 25, YELLOW);

    if (gGameOver)
        DrawText("GOAT won", SCREEN_WIDTH / 2 - 130, SCREEN_HEIGHT / 2 - 30, 60, YELLOW);

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