#include "Scene.h"

Scene::Scene() : mOrigin{ {} } {}

Scene::Scene(Vector2 origin, const char *)
    : mOrigin{ origin }
{}