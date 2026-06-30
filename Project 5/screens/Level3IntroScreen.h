#ifndef SCREENS_LEVEL3INTROSCREEN_H
#define SCREENS_LEVEL3INTROSCREEN_H

#include "Scene.h"

class Level3IntroScreen : public Scene
{
private:
    Texture2D mBgTexture;

public:
    Level3IntroScreen();
    Level3IntroScreen(Vector2 origin, const char *bgHexCode);
    ~Level3IntroScreen();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif