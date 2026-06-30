#ifndef SCREENS_GAMEOVERSCREEN_H
#define SCREENS_GAMEOVERSCREEN_H

#include "Scene.h"

class GameOverScreen : public Scene
{
private:
    Texture2D mBgTexture = {};

public:
    GameOverScreen();
    GameOverScreen(Vector2 origin, const char *bgHexCode);
    ~GameOverScreen();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif