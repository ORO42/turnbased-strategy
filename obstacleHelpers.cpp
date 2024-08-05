#include "include/obstacleHelpers.h"

void createObstacle(
    ObstacleType &obstacleType,
    Position &pos,
    Texture2D &tex,
    std::vector<Obstacle> &allObstacles)
{
    Obstacle newObstacle;
    newObstacle.obstacleType = obstacleType;
    newObstacle.pos = pos;
    newObstacle.tex = tex;
    newObstacle.health = 100.0f;

    if (obstacleType == ObstacleType::WALL)
    {
        newObstacle.isNavigable = false;
        newObstacle.cover = 100.0f;
        newObstacle.unitCanBeUnder = false;
        newObstacle.visibleInStance = {Stance::NA};
    }

    allObstacles.push_back(newObstacle);
}