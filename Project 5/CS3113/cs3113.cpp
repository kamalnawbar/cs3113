#include "cs3113.h"

float GetLength(const Vector2 vector)
{
    return sqrtf(pow(vector.x, 2) + pow(vector.y, 2));
}

void Normalise(Vector2 *vector)
{
    float magnitude = GetLength(*vector);
    vector->x /= magnitude;
    vector->y /= magnitude;
}

Rectangle getUVRectangle(const Texture2D *texture, int index, int rows, int cols)
{
    float uCoord = (float)(index % cols) / (float)cols;
    uCoord *= texture->width;

    float vCoord = (float)(index / cols) / (float)rows;
    vCoord *= texture->height;

    float sliceWidth  = texture->width  / (float)cols;
    float sliceHeight = texture->height / (float)rows;

    return { uCoord, vCoord, sliceWidth, sliceHeight };
}