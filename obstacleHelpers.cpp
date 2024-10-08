#include "include/obstacleHelpers.h"

void createObstacle(
    ObstacleType obstacleType,
    Position pos,
    Texture2D &tex,
    VectorSharedPointer<Obstacle> &allObstacles)
{
    SharedPointer<Obstacle> newObstacle = std::make_shared<Obstacle>();
    newObstacle->uuid = generateUUID();
    newObstacle->obstacleType = obstacleType;
    newObstacle->pos = pos;
    newObstacle->tex = tex;

    if (obstacleType == ObstacleType::WALL)
    {
        newObstacle->health = 100.0f;
        newObstacle->isNavigable = false;
        newObstacle->cover = 100.0f;
        newObstacle->unitCanBeUnder = false;
        newObstacle->visibleInProne = false;
        newObstacle->visibleInStanding = false;
        newObstacle->projectileStoppage = ProjectileStoppage::ALWAYS;
    }

    allObstacles.push_back(newObstacle);
}

bool isMouseOverAnyObstacle(VectorSharedPointer<Obstacle> &allObstacles, Vector2 &worldMousePos)
{
    for (auto &obstacle : allObstacles)
    {
        if (CheckCollisionPointRec(worldMousePos, {obstacle->pos.x, obstacle->pos.y, static_cast<float>(obstacle->tex.width), static_cast<float>(obstacle->tex.height)}))
        {
            return true;
        }
    }
    return false;
}

void sObstacleHover(VectorSharedPointer<Obstacle> &allObstacles, SharedPointer<Obstacle> &hoveredObstacle, Vector2 &worldMousePos)
{
    for (auto &obstacle : allObstacles)
    {
        if (CheckCollisionPointRec(worldMousePos, {obstacle->pos.x, obstacle->pos.y, static_cast<float>(obstacle->tex.width), static_cast<float>(obstacle->tex.height)}))
        {
            hoveredObstacle = obstacle;
        }
        // else
        // {
        //     hoveredObstacle = nullptr;
        // }
    }
}