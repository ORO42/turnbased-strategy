#pragma once

#include "components.h"
#include "uuid.h"

void createObstacle(
    ObstacleType obstacleType,
    Position pos,
    Texture2D &tex,
    std::vector<Obstacle> &allObstacles);
bool isMouseOverAnyObstacle(std::vector<Obstacle> &allObstacles, Vector2 &worldMousePos);
void sObstacleHover(std::vector<Obstacle> &allObstacles, Obstacle *&hoveredObstacle, Vector2 &worldMousePos);