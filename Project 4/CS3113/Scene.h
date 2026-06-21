#ifndef SCENE_H
#define SCENE_H

#include "Entity.h"

struct GameState
{
    Entity *player;
    Entity *enemies;
    int     enemyCount;

    Map    *map;

    int     lives;

    Music bgm;
    Sound jumpSound;
    Sound hitSound;

    SceneID nextSceneID;
};

class Scene
{
protected:
    GameState   mGameState;
    Vector2     mOrigin;
    const char *mBGColourHexCode = "#000000";

public:
    Scene();
    Scene(Vector2 origin, const char *bgHexCode);
    virtual ~Scene() = default;

    virtual void initialise() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void shutdown() = 0;

    GameState        getState()           const { return mGameState;        }
    const GameState &getStateRef()        const { return mGameState;        }
    Vector2          getOrigin()          const { return mOrigin;           }
    const char      *getBGColourHexCode() const { return mBGColourHexCode; }

    void setLives(int n) { mGameState.lives = n; }
};

#endif // SCENE_H