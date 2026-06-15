#include "Entity.h"

Entity::Entity() : mPosition {0.0f, 0.0f}, mMovement {0.0f, 0.0f},
                   mVelocity {0.0f, 0.0f},mAcceleration {0.0f, 0.0f},
                   mScale {DEFAULT_SIZE, DEFAULT_SIZE},
                   mColliderDimensions {DEFAULT_SIZE, DEFAULT_SIZE},
                   mTexture {} { }

Entity::Entity(Vector2 position, Vector2 scale, const char *textureFilepath) :
    mPosition {position}, mVelocity {0.0f, 0.0f}, mAcceleration {0.0f, 0.0f},
    mScale {scale}, mMovement {0.0f, 0.0f},
    mColliderDimensions {scale}, mTexture {LoadTexture(textureFilepath)} { }

Entity::~Entity() { UnloadTexture(mTexture); };

void Entity::checkCollisionY(Entity *collidableEntities, int collisionCheckCount)
{
    for (int i = 0; i < collisionCheckCount; i++)
    {
        Entity *collidableEntity = &collidableEntities[i];

        if (isColliding(collidableEntity))
        {
            float yDistance = fabs(mPosition.y - collidableEntity->mPosition.y);
            float yOverlap  = fabs(yDistance - (mColliderDimensions.y / 2.0f) - (collidableEntity->mColliderDimensions.y / 2.0f));

            if (mVelocity.y > 0) {
                mPosition.y -= yOverlap;
                mVelocity.y = 0.0f;
                mIsCollidingBottom = true;
            } else if (mVelocity.y < 0) {
                mPosition.y += yOverlap;
                mVelocity.y = 0.0f;
                mIsCollidingTop = true;
            }
        }
    }
}


void Entity::checkCollisionX(Entity *collidableEntities, int collisionCheckCount)
{
    for (int i = 0; i < collisionCheckCount; i++)
    {
        Entity *collidableEntity = &collidableEntities[i];

        if (isColliding(collidableEntity))
        {
            float xDistance = fabs(mPosition.x - collidableEntity->mPosition.x);
            float xOverlap  = fabs(xDistance - (mColliderDimensions.x / 2.0f) - (collidableEntity->mColliderDimensions.x / 2.0f));

            if (mVelocity.x > 0) {
                mPosition.x -= xOverlap;
                mVelocity.x *= -1;

            } else if (mVelocity.x < 0) {
                mPosition.x += xOverlap;
                mVelocity.x *= -1;
            }
        }
    }
}


Rectangle Entity::getUVRectangle(int index)
{
    float uCoord = (float)(index % mSpritesheetCols) / (float)mSpritesheetCols;
    uCoord *= mTexture.width;

    float vCoord = (float)(index / mSpritesheetCols) / (float)mSpritesheetRows;
    vCoord *= mTexture.height;

    float sliceWidth  = mTexture.width  / (float)mSpritesheetCols;
    float sliceHeight = mTexture.height / (float)mSpritesheetRows;

    return { uCoord, vCoord, sliceWidth, sliceHeight };
}





/**
 * Checks if two entities are colliding based on their positions and collider
 * dimensions.
 *
 * @param other represents another Entity with which you want to check for
 * collision. It is a pointer to the Entity class.
 *
 * @return returns `true` if the two entities are colliding based on their
 * positions and collider dimensions, and `false` otherwise.
 */
bool Entity::isColliding(Entity *other) const
{
    if (!other->isActive()) return false;

    float xDistance = fabs(mPosition.x - other->getPosition().x) -
        ((mColliderDimensions.x + other->getColliderDimensions().x) / 2.0f);
    float yDistance = fabs(mPosition.y - other->getPosition().y) -
        ((mColliderDimensions.y + other->getColliderDimensions().y) / 2.0f);

    if (xDistance < 0.0f && yDistance < 0.0f) return true;

    return false;
}

void Entity::displayCollider()
{
    Rectangle colliderBox = {
        mPosition.x - mColliderDimensions.x / 2.0f,
        mPosition.y - mColliderDimensions.y / 2.0f,
        mColliderDimensions.x,
        mColliderDimensions.y
    };

    DrawRectangleLines(
        colliderBox.x,
        colliderBox.y,
        colliderBox.width,
        colliderBox.height,
        GREEN
    );
}

void Entity::bounceOffWalls(int screenHeight)
{
    if (mEntityStatus == INACTIVE) return;

    if (mPosition.y - mColliderDimensions.y / 2.0f <= 0.0f)
    {
        mVelocity.y  =  fabs(mVelocity.y);
        mPosition.y  =  mColliderDimensions.y / 2.0f;
    }
    else if (mPosition.y + mColliderDimensions.y / 2.0f >= (float) screenHeight)
    {
        mVelocity.y  = -fabs(mVelocity.y);
        mPosition.y  =  (float) screenHeight - mColliderDimensions.y / 2.0f;
    }
}

void Entity::clampToScreen(int screenHeight)
{
    if (mPosition.y - mScale.y / 2.0f < 0.0f)
        mPosition.y = mScale.y / 2.0f;
    else if (mPosition.y + mScale.y / 2.0f > (float) screenHeight)
        mPosition.y = (float) screenHeight - mScale.y / 2.0f;
}

void Entity::update(float deltaTime, Entity *collidableEntities, int collisionCheckCount)
{
    if (mEntityStatus == INACTIVE) return;

    mIsCollidingBottom = false;
    mIsCollidingTop = false;

    mVelocity.x += mAcceleration.x * deltaTime;
    mVelocity.y += mAcceleration.y * deltaTime;

    mPosition.x += mVelocity.x * deltaTime;
    mPosition.y += mVelocity.y * deltaTime;

    checkCollisionY(collidableEntities, collisionCheckCount);
    if (!mIsCollidingBottom) checkCollisionX(collidableEntities, collisionCheckCount);

    if (!mAnimationFrames.empty()) {
        mAnimationTime += deltaTime;
        float secondsPerFrame = 1.0f / (float)mFramesPerSecond;

        if (mAnimationTime >= secondsPerFrame) {
            mAnimationTime  = 0.0f;
            mAnimationIndex = (mAnimationIndex + 1) % (int)mAnimationFrames.size();
        }
    }
}
void Entity::render()
{
    if (mEntityStatus == INACTIVE) return;

    Rectangle textureArea;
    if (!mAnimationFrames.empty()) {
        textureArea = getUVRectangle(mAnimationFrames[mAnimationIndex]);
    }
    else {
        textureArea = {
            0.0f, 0.0f,
            static_cast<float>(mTexture.width),
            static_cast<float>(mTexture.height)
        };
    }

    Rectangle destinationArea = {
        mPosition.x,
        mPosition.y,
        static_cast<float>(mScale.x),
        static_cast<float>(mScale.y)
    };

    Vector2 originOffset = {
        static_cast<float>(mScale.x) / 2.0f,
        static_cast<float>(mScale.y) / 2.0f
    };

    DrawTexturePro(
        mTexture,
        textureArea, destinationArea, originOffset,
        0.0f, WHITE
    );

     //displayCollider();
}