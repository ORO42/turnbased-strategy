#pragma once

#include "components.h"
#include "uuid.h"

void createObstacle(
    ObstacleType obstacleType,
    Position pos,
    Texture2D &tex,
    VectorSharedPointer<Obstacle> &allObstacles);
bool isMouseOverAnyObstacle(VectorSharedPointer<Obstacle> &allObstacles, Vector2 &worldMousePos);
void sObstacleHover(VectorSharedPointer<Obstacle> &allObstacles, SharedPointer<Obstacle> &hoveredObstacle, Vector2 &worldMousePos);