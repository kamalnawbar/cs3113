#ifndef ENTITY_H
#define ENTITY_H

#include "Map.h"

enum Direction    { LEFT, UP, RIGHT, DOWN               };
enum EntityStatus { ACTIVE, INACTIVE                    };
enum EntityType   { PLAYER, BLOCK, PLATFORM, NPC, EMPTY };
enum AIType       { WANDERER, FOLLOWER, FLYER           };
enum AIState      { WALKING, IDLE, FOLLOWING            };

class Entity
{
private:
    Vector2 mPosition;
    Vector2 mMovement;
    Vector2 mVelocity;
    Vector2 mAcceleration;

    Vector2 mScale;
    Vector2 mColliderDimensions;

    Texture2D   mTexture;
    TextureType mTextureType;
    Vector2     mSpriteSheetDimensions;

    std::map<Direction, std::vector<int>> mAnimationAtlas;
    std::vector<int>                      mAnimationIndices;
    Direction                             mDirection;
    int                                   mFrameSpeed;

    int   mCurrentFrameIndex = 0;
    float mAnimationTime     = 0.0f;

    bool  mIsJumping    = false;
    float mJumpingPower = 0.0f;

    int   mSpeed;
    float mAngle;

    bool mIsCollidingTop    = false;
    bool mIsCollidingBottom = false;
    bool mIsCollidingRight  = false;
    bool mIsCollidingLeft   = false;

    EntityStatus mEntityStatus = ACTIVE;
    EntityType   mEntityType;

    AIType  mAIType;
    AIState mAIState;

    float mFlightTimer     = 0.0f;
    float mFlightAmplitude = 100.0f;
    float mBaseY           = 0.0f;
    bool  mFlipX           = false;
    int   mHealth          = 1;
    bool  mAutoAnimate     = false;

    bool isColliding(Entity *other) const;

    void checkCollisionY(Entity *collidableEntities, int collisionCheckCount);
    void checkCollisionY(Map *map);

    void checkCollisionX(Entity *collidableEntities, int collisionCheckCount);
    void checkCollisionX(Map *map);

    void resetColliderFlags()
    {
        mIsCollidingTop    = false;
        mIsCollidingBottom = false;
        mIsCollidingRight  = false;
        mIsCollidingLeft   = false;
    }

    void AIActivate(Entity *target, float deltaTime);
    void AIWander();
    void AIFollow(Entity *target);
    void AIFly(float deltaTime);

public:
    static constexpr int   DEFAULT_SIZE          = 250;
    static constexpr int   DEFAULT_SPEED         = 200;
    static constexpr int   DEFAULT_FRAME_SPEED   = 14;
    static constexpr float Y_COLLISION_THRESHOLD = 0.5f;

    Entity();
    Entity(Vector2 position, Vector2 scale, const char *textureFilepath, EntityType entityType);
    ~Entity();

    void update(float deltaTime, Entity *player, Map *map, Entity *collidableEntities, int collisionCheckCount);
    void render();
    void animate(float deltaTime);
    void normaliseMovement() { Normalise(&mMovement); }

    void jump()       { mIsJumping    = true;    }
    void activate()   { mEntityStatus = ACTIVE;  }
    void deactivate() { mEntityStatus = INACTIVE; }
    void takeDamage()
    {
        mHealth--;
        if (mHealth <= 0) deactivate();
    }

    bool isActive() { return mEntityStatus == ACTIVE; }

    void moveUp()    { mMovement.y = -1; mDirection = UP;    }
    void moveDown()  { mMovement.y =  1; mDirection = DOWN;  }
    void moveLeft()  { mMovement.x = -1; mDirection = LEFT;  }
    void moveRight() { mMovement.x =  1; mDirection = RIGHT; }

    void resetMovement() { mMovement = { 0.0f, 0.0f }; }

    Vector2    getPosition() const { return mPosition; }
    Vector2    getScale()    const { return mScale;    }
    int        getHealth()   const { return mHealth;   }
    bool       isCollidingBottom() const { return mIsCollidingBottom; }

    void setPosition(Vector2 v)         { mPosition = v;                   }
    void setMovement(Vector2 v)         { mMovement = v;                   }
    void setAcceleration(Vector2 v)     { mAcceleration = v;               }
    void setScale(Vector2 v)            { mScale = v;                      }
    void setTexture(const char *path)   { mTexture = LoadTexture(path);    }
    void setColliderDimensions(Vector2 v) { mColliderDimensions = v;       }
    void setSpriteSheetDimensions(Vector2 v) { mSpriteSheetDimensions = v; }
    void setSpeed(int s)                { mSpeed = s;                      }
    void setFrameSpeed(int s)           { mFrameSpeed = s;                 }
    void setJumpingPower(float p)       { mJumpingPower = p;               }
    void setAngle(float a)              { mAngle = a;                      }
    void setEntityType(EntityType t)    { mEntityType = t;                 }
    void setDirection(Direction d)
    {
        mDirection = d;
        if (mTextureType == ATLAS && mAnimationAtlas.count(mDirection))
            mAnimationIndices = mAnimationAtlas.at(mDirection);
    }
    void setAIState(AIState s)          { mAIState = s;                    }
    void setAIType(AIType t)            { mAIType = t;                     }
    void setFlipX(bool f)               { mFlipX = f;                      }
    void setHealth(int h)               { mHealth = h;                     }
    void setFlightAmplitude(float a)    { mFlightAmplitude = a;            }
    void setBaseY(float y)              { mBaseY = y;                      }
    void setFlightTimer(float t)        { mFlightTimer = t;                }
    void setAutoAnimate(bool a)         { mAutoAnimate = a;                }
    void setTextureType(TextureType t)  { mTextureType = t;                }
    void setAnimationAtlas(std::map<Direction, std::vector<int>> atlas)
    {
        mAnimationAtlas = atlas;
        if (atlas.count(mDirection))
            mAnimationIndices = atlas.at(mDirection);
    }
};

#endif