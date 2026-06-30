#ifndef MAP_H
#define MAP_H

#include "cs3113.h"

class Map
{
private:
    int mMapColumns;
    int mMapRows;

    unsigned int *mLevelData;
    Texture2D     mTextureAtlas;

    float mTileSize;
    int   mTextureColumns;
    int   mTextureRows;

    std::vector<Rectangle> mTextureAreas;
    Vector2                mOrigin;

    float mLeftBoundary;
    float mRightBoundary;
    float mTopBoundary;
    float mBottomBoundary;

public:
    Map(
        int           mapColumns,
        int           mapRows,
        unsigned int *levelData,
        const char   *textureFilePath,
        float         tileSize,
        int           textureColumns,
        int           textureRows,
        Vector2       origin
    );
    ~Map();

    void build();
    void render();
    bool isSolidTileAt(Vector2 position, float *xOverlap, float *yOverlap);

    float getLeftBoundary()   const { return mLeftBoundary;   }
    float getRightBoundary()  const { return mRightBoundary;  }
    float getTopBoundary()    const { return mTopBoundary;    }
    float getBottomBoundary() const { return mBottomBoundary; }
};

#endif