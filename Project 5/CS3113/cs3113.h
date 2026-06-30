#ifndef CS3113_H
#define CS3113_H

#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include <math.h>
#include <stdio.h>
#include <map>
#include <vector>
#include <string>
#include <iostream>

enum AppStatus   { TERMINATED, RUNNING                                                                                      };
enum TextureType { SINGLE, ATLAS                                                                                            };
enum SceneID     { NO_SCENE = -1, MENU_SCENE, LEVEL_1_INTRO, LEVEL_1, LEVEL_2_INTRO, LEVEL_2, LEVEL_3_INTRO, LEVEL_3, WIN_SCREEN, GAME_OVER_SCREEN };

void      Normalise(Vector2 *vector);
float     GetLength(const Vector2 vector);
Rectangle getUVRectangle(const Texture2D *texture, int index, int rows, int cols);

#endif