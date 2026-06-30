#ifndef LEVEL1INTROSCREEN_H
#define LEVEL1INTROSCREEN_H

#include "../CS3113/Scene.h"

class Level1IntroScreen : public Scene
{
private:
    Texture2D mBgTexture;

public:
    Level1IntroScreen();
    Level1IntroScreen(Vector2 origin, const char *bgHexCode);
    ~Level1IntroScreen();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif