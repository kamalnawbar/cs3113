#ifndef SCENE_H
#define SCENE_H

#include "Entity.h"

struct GameState
{
    Entity *player      = nullptr;
    Entity *enemies     = nullptr;
    int     enemyCount  = 0;
    Map    *map         = nullptr;
    int     lives       = 3;
    SceneID nextSceneID = NO_SCENE;
};

class Scene
{
protected:
    GameState mGameState;
    Vector2   mOrigin;

public:
    Scene();
    Scene(Vector2 origin, const char *bgHexCode);
    virtual ~Scene() = default;

    virtual void initialise() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void shutdown() = 0;

    GameState        getState()    const { return mGameState; }
    const GameState &getStateRef() const { return mGameState; }
    Vector2          getOrigin()   const { return mOrigin;    }

    void setLives(int n) { mGameState.lives = n; }
};

#endif