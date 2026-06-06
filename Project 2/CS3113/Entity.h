#ifndef ENTITY_H
#define ENTITY_H

#include "cs3113.h"

enum EntityStatus { ACTIVE, INACTIVE              };


class Entity
{
private:
    Vector2 mPosition;
    Vector2 mMovement;
    Vector2 mVelocity;

    Vector2 mScale;
    Vector2 mColliderDimensions;

    Texture2D mTexture;



    EntityStatus mEntityStatus = ACTIVE;

    bool isColliding(Entity *other) const;
    void checkCollisionX(Entity *collidableEntities, int collisionCheckCount);



public:
    static constexpr int   DEFAULT_SIZE          = 250;


    Entity();
    Entity(Vector2 position, Vector2 scale, const char *textureFilepath);
    ~Entity();

    void update(float deltaTime, Entity *collidableEntities, int collisionCheckCount);
    void render();
    void normaliseMovement() { Normalise(&mMovement); }

    void activate()   { mEntityStatus  = ACTIVE;   }
    void deactivate() { mEntityStatus  = INACTIVE; }
    void displayCollider();
    void bounceOffWalls(int screenHeight);
    void clampToScreen(int screenHeight);

    bool isActive() { return mEntityStatus == ACTIVE ? true : false; }


    void resetMovement() { mMovement = { 0.0f, 0.0f }; }

    Vector2     getPosition()              const { return mPosition;              }
    Vector2     getMovement()              const { return mMovement;              }
    Vector2     getVelocity()              const { return mVelocity;              }
    Vector2     getScale()                     const { return mScale;                 }
    Vector2     getColliderDimensions()    const { return mColliderDimensions;                 }




    void setPosition(Vector2 newPosition)
        { mPosition = newPosition;                 }
    void setMovement(Vector2 newMovement)
        { mMovement = newMovement;                 }
    void setVelocity(Vector2 newVelocity)
        { mVelocity = newVelocity;                 }
    void setColliderDimensions(Vector2 newDimensions)
        { mColliderDimensions = newDimensions;     }
   void setScale(Vector2 newScale)
        { mScale = newScale;                       }

};

#endif // ENTITY_H