#ifndef SCREENS_WINSCREEN_H
#define SCREENS_WINSCREEN_H

#include "Scene.h"

class WinScreen : public Scene
{
private:
    Texture2D mBgTexture = {};

public:
    WinScreen();
    WinScreen(Vector2 origin, const char *bgHexCode);
    ~WinScreen();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif