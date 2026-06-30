#include "Entity.h"

Entity::Entity()
    : mPosition{ 0.0f, 0.0f }, mMovement{ 0.0f, 0.0f },
      mVelocity{ 0.0f, 0.0f }, mAcceleration{ 0.0f, 0.0f },
      mScale{ DEFAULT_SIZE, DEFAULT_SIZE },
      mColliderDimensions{ DEFAULT_SIZE, DEFAULT_SIZE },
      mTexture{}, mTextureType{ SINGLE }, mAngle{ 0.0f },
      mSpriteSheetDimensions{}, mDirection{ RIGHT },
      mAnimationAtlas{ {} }, mAnimationIndices{}, mFrameSpeed{ 0 },
      mEntityType{ EMPTY }
{}

Entity::Entity(
    Vector2     position,
    Vector2     scale,
    const char *textureFilepath,
    EntityType  entityType
)
    : mPosition{ position }, mVelocity{ 0.0f, 0.0f },
      mAcceleration{ 0.0f, 0.0f }, mScale{ scale }, mMovement{ 0.0f, 0.0f },
      mColliderDimensions{ scale }, mTexture{ LoadTexture(textureFilepath) },
      mTextureType{ SINGLE }, mDirection{ RIGHT }, mAnimationAtlas{ {} },
      mAnimationIndices{}, mFrameSpeed{ 0 }, mSpeed{ DEFAULT_SPEED },
      mAngle{ 0.0f }, mEntityType{ entityType }
{}

Entity::~Entity()
{
    UnloadTexture(mTexture);
}

void Entity::checkCollisionY(Entity *collidableEntities, int collisionCheckCount)
{
    for (int i = 0; i < collisionCheckCount; i++)
    {
        Entity *collidableEntity = &collidableEntities[i];

        if (isColliding(collidableEntity))
        {
            float yDistance = fabs(mPosition.y - collidableEntity->mPosition.y);
            float yOverlap  = fabs(
                yDistance - (mColliderDimensions.y / 2.0f) -
                (collidableEntity->mColliderDimensions.y / 2.0f)
            );

            if (mVelocity.y > 0)
            {
                mPosition.y       -= yOverlap;
                mVelocity.y        = 0;
                mIsCollidingBottom = true;
            }
            else if (mVelocity.y < 0)
            {
                mPosition.y    += yOverlap;
                mVelocity.y     = 0;
                mIsCollidingTop = true;

                if (collidableEntity->mEntityType == BLOCK)
                    collidableEntity->deactivate();
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
            float yDistance = fabs(mPosition.y - collidableEntity->mPosition.y);
            float yOverlap  = fabs(
                yDistance - (mColliderDimensions.y / 2.0f) -
                (collidableEntity->mColliderDimensions.y / 2.0f)
            );

            if (yOverlap < Y_COLLISION_THRESHOLD) continue;

            float xDistance = fabs(mPosition.x - collidableEntity->mPosition.x);
            float xOverlap  = fabs(
                xDistance - (mColliderDimensions.x / 2.0f) -
                (collidableEntity->mColliderDimensions.x / 2.0f)
            );

            if (mVelocity.x > 0)
            {
                mPosition.x      -= xOverlap;
                mVelocity.x       = 0;
                mIsCollidingRight = true;
            }
            else if (mVelocity.x < 0)
            {
                mPosition.x     += xOverlap;
                mVelocity.x      = 0;
                mIsCollidingLeft = true;
            }
        }
    }
}

void Entity::checkCollisionY(Map *map)
{
    if (map == nullptr) return;

    Vector2 topCentreProbe    = { mPosition.x, mPosition.y - (mColliderDimensions.y / 2.0f) };
    Vector2 topLeftProbe      = { mPosition.x - (mColliderDimensions.x / 2.0f), mPosition.y - (mColliderDimensions.y / 2.0f) };
    Vector2 topRightProbe     = { mPosition.x + (mColliderDimensions.x / 2.0f), mPosition.y - (mColliderDimensions.y / 2.0f) };
    Vector2 bottomCentreProbe = { mPosition.x, mPosition.y + (mColliderDimensions.y / 2.0f) };
    Vector2 bottomLeftProbe   = { mPosition.x - (mColliderDimensions.x / 2.0f), mPosition.y + (mColliderDimensions.y / 2.0f) };
    Vector2 bottomRightProbe  = { mPosition.x + (mColliderDimensions.x / 2.0f), mPosition.y + (mColliderDimensions.y / 2.0f) };

    float xOverlap = 0.0f;
    float yOverlap = 0.0f;

    if ((map->isSolidTileAt(topCentreProbe,  &xOverlap, &yOverlap) ||
         map->isSolidTileAt(topLeftProbe,    &xOverlap, &yOverlap) ||
         map->isSolidTileAt(topRightProbe,   &xOverlap, &yOverlap)) &&
        mVelocity.y < 0.0f)
    {
        mPosition.y    += yOverlap;
        mVelocity.y     = 0.0f;
        mIsCollidingTop = true;
    }

    if ((map->isSolidTileAt(bottomCentreProbe, &xOverlap, &yOverlap) ||
         map->isSolidTileAt(bottomLeftProbe,   &xOverlap, &yOverlap) ||
         map->isSolidTileAt(bottomRightProbe,  &xOverlap, &yOverlap)) &&
        mVelocity.y > 0.0f)
    {
        mPosition.y       -= yOverlap;
        mVelocity.y        = 0.0f;
        mIsCollidingBottom = true;
    }
}

void Entity::checkCollisionX(Map *map)
{
    if (map == nullptr) return;

    Vector2 leftCentreProbe  = { mPosition.x - (mColliderDimensions.x / 2.0f), mPosition.y };
    Vector2 rightCentreProbe = { mPosition.x + (mColliderDimensions.x / 2.0f), mPosition.y };

    float xOverlap = 0.0f;
    float yOverlap = 0.0f;

    if (map->isSolidTileAt(rightCentreProbe, &xOverlap, &yOverlap) &&
        mVelocity.x > 0.0f && yOverlap >= 0.5f)
    {
        mPosition.x      -= xOverlap * 1.01f;
        mVelocity.x       = 0.0f;
        mIsCollidingRight = true;
    }

    if (map->isSolidTileAt(leftCentreProbe, &xOverlap, &yOverlap) &&
        mVelocity.x < 0.0f && yOverlap >= 0.5f)
    {
        mPosition.x     += xOverlap * 1.01f;
        mVelocity.x      = 0.0f;
        mIsCollidingLeft = true;
    }
}

bool Entity::isColliding(Entity *other) const
{
    if (!other->isActive() || other == this) return false;

    float xDistance =
        fabs(mPosition.x - other->mPosition.x) -
        ((mColliderDimensions.x + other->mColliderDimensions.x) / 2.0f);
    float yDistance =
        fabs(mPosition.y - other->mPosition.y) -
        ((mColliderDimensions.y + other->mColliderDimensions.y) / 2.0f);

    return xDistance < 0.0f && yDistance < 0.0f;
}

void Entity::animate(float deltaTime)
{
    mAnimationIndices = mAnimationAtlas.at(mDirection);

    mAnimationTime       += deltaTime;
    float secondsPerFrame = 1.0f / mFrameSpeed;

    if (mAnimationTime >= secondsPerFrame)
    {
        mAnimationTime = 0.0f;
        mCurrentFrameIndex++;
        mCurrentFrameIndex %= mAnimationIndices.size();
    }
}

void Entity::AIWander() { moveLeft(); }

void Entity::AIFollow(Entity *target)
{
    if (target == nullptr) { moveLeft(); return; }

    switch (mAIState)
    {
    case IDLE:
        if (Vector2Distance(mPosition, target->getPosition()) < 250.0f)
            mAIState = WALKING;
        break;

    case WALKING:
        resetMovement();
        if (mPosition.x > target->getPosition().x + 5.0f)       moveLeft();
        else if (mPosition.x < target->getPosition().x - 5.0f)  moveRight();
        if (mPosition.y > target->getPosition().y + 5.0f)       moveUp();
        else if (mPosition.y < target->getPosition().y - 5.0f)  moveDown();
        normaliseMovement();
        break;

    default:
        break;
    }
}

void Entity::AIFly(float deltaTime)
{
    moveLeft();
    mFlightTimer += deltaTime;
    mPosition.y   = mBaseY + mFlightAmplitude * sinf(mFlightTimer * 2.5f);
}

void Entity::AIActivate(Entity *target, float deltaTime)
{
    switch (mAIType)
    {
    case WANDERER: AIWander();         break;
    case FOLLOWER: AIFollow(target);   break;
    case FLYER:    AIFly(deltaTime);   break;
    default:                           break;
    }
}

void Entity::update(
    float   deltaTime,
    Entity *player,
    Map    *map,
    Entity *collidableEntities,
    int     collisionCheckCount
)
{
    if (mEntityStatus == INACTIVE) return;

    if (mEntityType == NPC) AIActivate(player, deltaTime);

    resetColliderFlags();

    mVelocity.x  = mMovement.x * mSpeed;
    mVelocity.x += mAcceleration.x * deltaTime;

    if (mAcceleration.y == 0.0f)
        mVelocity.y = mMovement.y * mSpeed;
    else
        mVelocity.y += mAcceleration.y * deltaTime;

    if (mIsJumping)
    {
        mIsJumping   = false;
        mVelocity.y -= mJumpingPower;
    }

    mPosition.y += mVelocity.y * deltaTime;
    checkCollisionY(collidableEntities, collisionCheckCount);
    checkCollisionY(map);

    mPosition.x += mVelocity.x * deltaTime;
    checkCollisionX(collidableEntities, collisionCheckCount);
    checkCollisionX(map);

    if (mTextureType == ATLAS && (mAutoAnimate || GetLength(mMovement) != 0))
        animate(deltaTime);
}

void Entity::render()
{
    if (mEntityStatus == INACTIVE) return;

    Rectangle textureArea;

    switch (mTextureType)
    {
    case SINGLE:
        textureArea = { 0.0f, 0.0f,
                        static_cast<float>(mTexture.width),
                        static_cast<float>(mTexture.height) };
        break;
    case ATLAS:
        textureArea = getUVRectangle(
            &mTexture,
            mAnimationIndices[mCurrentFrameIndex],
            mSpriteSheetDimensions.x,
            mSpriteSheetDimensions.y
        );
        break;
    default:
        textureArea = { 0.0f, 0.0f,
                        static_cast<float>(mTexture.width),
                        static_cast<float>(mTexture.height) };
        break;
    }

    if (mFlipX)
    {
        textureArea.x    += textureArea.width;
        textureArea.width = -textureArea.width;
    }

    Rectangle destinationArea = { mPosition.x, mPosition.y,
                                  static_cast<float>(mScale.x),
                                  static_cast<float>(mScale.y) };

    Vector2 originOffset = { static_cast<float>(mScale.x) / 2.0f,
                             static_cast<float>(mScale.y) / 2.0f };

    DrawTexturePro(mTexture, textureArea, destinationArea, originOffset, mAngle, WHITE);
}