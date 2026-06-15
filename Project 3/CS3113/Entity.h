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
    
    Vector2 mAcceleration;
    bool mIsCollidingBottom = false;
    bool mIsCollidingTop    = false;
    int mSpritesheetCols = 1;
    int mSpritesheetRows = 1;
    int mAnimationIndex  = 0;
    float mAnimationTime   = 0.0f;
    int mFramesPerSecond = 8;
    std::vector<int> mAnimationFrames;


    EntityStatus mEntityStatus = ACTIVE;

    bool isColliding(Entity *other) const;
    void checkCollisionX(Entity *collidableEntities, int collisionCheckCount);

    void checkCollisionY(Entity *collidableEntities, int collisionCheckCount);
    Rectangle getUVRectangle(int index);


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

    bool isCollidingBottom() const {return mIsCollidingBottom;}
    bool isCollidingTop() const {return mIsCollidingTop;}

    
    
    
    
    
    Vector2     getPosition()              const { return mPosition;              }
    Vector2     getMovement()              const { return mMovement;              }
    Vector2     getVelocity()              const { return mVelocity;              }
    Vector2     getScale()                 const { return mScale;                 }
    Vector2     getColliderDimensions()    const { return mColliderDimensions;    }
    Vector2     getAcceleration()          const { return mAcceleration;          }




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



    void setSpritesheetDimensions(int cols, int rows) 
        { mSpritesheetCols = cols; mSpritesheetRows = rows;}
    void setAnimationFrames(std::vector<int> frames)  
        { mAnimationFrames = frames; mAnimationIndex = 0;}
    void setFramesPerSecond(int fps)                  
        { mFramesPerSecond = fps;                   }
        
    void setAcceleration(Vector2 newAcceleration)
        { mAcceleration = newAcceleration;          }
    void setTexture(const char *filepath)
        { mTexture = LoadTexture(filepath);         }
    
    

};

#endif // ENTITY_H