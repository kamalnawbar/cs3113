#ifndef SCREENS_LEVEL2INTROSCREEN_H
#define SCREENS_LEVEL2INTROSCREEN_H

#include "Scene.h"

class Level2IntroScreen : public Scene
{
private:
    Texture2D mBgTexture;

public:
    Level2IntroScreen();
    Level2IntroScreen(Vector2 origin, const char *bgHexCode);
    ~Level2IntroScreen();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif