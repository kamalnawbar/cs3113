#ifndef GAME_OVER_SCREEN_H
#define GAME_OVER_SCREEN_H

#include "Scene.h"

class GameOverScreen : public Scene
{
public:
    GameOverScreen();
    GameOverScreen(Vector2 origin, const char *bgHexCode);
    ~GameOverScreen();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // GAME_OVER_SCREEN_H