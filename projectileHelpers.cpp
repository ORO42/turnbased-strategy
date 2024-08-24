#include "include/projectileHelpers.h"

SharedPointer<Projectile> createProjectile(
    VectorSharedPointer<Projectile> &allProjectiles,
    Texture2D tex,
    Position originPos,
    Rectangle targetRect,
    std::string originUnitUuid,
    Teams team,
    float facingAngle,
    float speed,
    bool canCollideBeforeDestination,
    bool causesExplosion,
    int projectileAccuracyRadius,
    int projectileDamageRadius,
    float damage,
    float accuracy)
{
    SharedPointer<Projectile> newProjectile = std::make_shared<Projectile>();
    newProjectile->tex = tex;
    newProjectile->originPos = originPos;
    // newProjectile.destinationPos = destinationPos;
    newProjectile->targetRect = targetRect;
    newProjectile->currentPos = originPos;
    newProjectile->originUnitUuid = originUnitUuid;
    newProjectile->team = team;
    newProjectile->facingAngle = facingAngle;
    newProjectile->speed = speed;
    newProjectile->canCollideBeforeDestination = canCollideBeforeDestination;
    newProjectile->shouldDestroy = false;
    newProjectile->causesExplosion = causesExplosion;
    newProjectile->damageRadius = projectileDamageRadius;
    newProjectile->accuracyRadius = projectileAccuracyRadius;
    newProjectile->damage = damage;
    newProjectile->accuracy = accuracy;

    allProjectiles.push_back(newProjectile);

    return newProjectile;
}

void sMoveProjectiles(VectorSharedPointer<Projectile> &allProjectiles, VectorSharedPointer<Unit> &allUnits, VectorSharedPointer<Obstacle> &allObstacles, float deltaTime)
{
    for (auto &projectile : allProjectiles)
    {
        if (!projectile->shouldDestroy)
        {
            // Calculate movement in the x and y directions based on the angle
            float moveStep = projectile->speed * deltaTime;
            float angleInRadians = projectile->facingAngle * (PI / 180.0f);
            float moveX = moveStep * std::cos(angleInRadians);
            float moveY = moveStep * std::sin(angleInRadians);

            Rectangle endRect;

            if (CheckCollisionRecs({projectile->currentPos.x, projectile->currentPos.y, static_cast<float>(projectile->tex.width), static_cast<float>(projectile->tex.height)}, projectile->targetRect))
            {
                projectile->shouldDestroy = true;
                endRect = projectile->targetRect;
            }

            if (projectile->canCollideBeforeDestination)
            {
                for (auto &unit : allUnits)
                {
                    if (CheckCollisionRecs({projectile->currentPos.x, projectile->currentPos.y, static_cast<float>(projectile->tex.width), static_cast<float>(projectile->tex.height)}, {unit->pos.x, unit->pos.y, static_cast<float>(unit->tex.width), static_cast<float>(unit->tex.height)}))
                    {
                        if (unit->uuid != projectile->originUnitUuid)
                        {
                            if (unit->projectileStoppage == ProjectileStoppage::ALWAYS)
                            {
                                endRect = {unit->pos.x, unit->pos.y, static_cast<float>(unit->tex.width), static_cast<float>(unit->tex.height)};
                                projectile->shouldDestroy = true;
                                break;
                            }
                            if (unit->projectileStoppage == ProjectileStoppage::SOMETIMES)
                            {
                                if (!calculateChance(projectile->accuracy))
                                {
                                    endRect = {unit->pos.x, unit->pos.y, static_cast<float>(unit->tex.width), static_cast<float>(unit->tex.height)};
                                    projectile->shouldDestroy = true;
                                    break;
                                }
                            }
                        }
                    }
                }

                for (auto &obstacle : allObstacles)
                {
                    if (CheckCollisionRecs({projectile->currentPos.x, projectile->currentPos.y, static_cast<float>(projectile->tex.width), static_cast<float>(projectile->tex.height)}, {obstacle->pos.x, obstacle->pos.y, static_cast<float>(obstacle->tex.width), static_cast<float>(obstacle->tex.height)}))
                    {
                        // TODO ignore obstacle unit is occupying
                        if (obstacle->projectileStoppage == ProjectileStoppage::ALWAYS)
                        {
                            endRect = {obstacle->pos.x, obstacle->pos.y, static_cast<float>(obstacle->tex.width), static_cast<float>(obstacle->tex.height)};
                            projectile->shouldDestroy = true;
                            break;
                        }
                        if (obstacle->projectileStoppage == ProjectileStoppage::SOMETIMES)
                        {
                            if (!calculateChance(projectile->accuracy))
                            {
                                endRect = {obstacle->pos.x, obstacle->pos.y, static_cast<float>(obstacle->tex.width), static_cast<float>(obstacle->tex.height)};
                                projectile->shouldDestroy = true;
                                break;
                            }
                        }
                    }
                }
            }

            if (!projectile->shouldDestroy)
            {
                projectile->currentPos.x += moveX;
                projectile->currentPos.y += moveY;
            }
        }
        else
        {
            VectorSharedPointer<Unit> unitsInAttackRadius;
            VectorSharedPointer<Obstacle> obstaclesInAttackRadius;
            Rectangle originRect;

            // handle damage
            for (auto &unit : allUnits)
            {
                // if unit is in damage radius
                // get blockers between object and endRect
                float distanceBetween = chebyshevTileDistance({projectile->originPos.x, projectile->originPos.y}, {unit->pos.x, unit->pos.y});
                if (distanceBetween == 0)
                {
                    originRect = {unit->pos.x,
                                  unit->pos.y,
                                  static_cast<float>(unit->tex.width),
                                  static_cast<float>(unit->tex.height)};
                }
                if (distanceBetween <= projectile->damageRadius && distanceBetween != 0)
                {
                    unitsInAttackRadius.push_back(unit);
                }
            }
            for (auto &obstacle : allObstacles)
            {
                // if obstacle is in damage radius
                // get blockers between object and endRect
                float distanceBetween = chebyshevTileDistance({projectile->originPos.x, projectile->originPos.y}, {obstacle->pos.x, obstacle->pos.y});
                if (distanceBetween <= projectile->damageRadius && distanceBetween != 0)
                {
                    obstaclesInAttackRadius.push_back(obstacle);
                }
            }

            // damage all units/obstacles 1 distance away
            int blockedCt = 0;
            for (auto &unit : unitsInAttackRadius)
            {
                std::vector<Line> lines = getCornerToCornerLines(originRect, projectile->targetRect);
                for (auto &line : lines)
                {
                    for (auto &unit2 : unitsInAttackRadius)
                    {
                        if (lineRectOverlap({unit2->pos.x, unit2->pos.y, static_cast<float>(unit2->tex.width), static_cast<float>(unit2->tex.height)}, line))
                        {
                            blockedCt += 1;
                            break;
                        }
                    }
                    for (auto &obstacle : obstaclesInAttackRadius)
                    {
                        if (lineRectOverlap({obstacle->pos.x, obstacle->pos.y, static_cast<float>(obstacle->tex.width), static_cast<float>(obstacle->tex.height)}, line))
                        {
                            blockedCt += 1;
                            break;
                        }
                    }
                }
                if (blockedCt < 32)
                {
                    unit->health -= projectile->damage;
                }
            }

            blockedCt = 0;
            for (auto &obstacle : obstaclesInAttackRadius)
            {
                std::vector<Line> lines = getCornerToCornerLines(originRect, projectile->targetRect);
                for (auto &line : lines)
                {
                    for (auto &unit : unitsInAttackRadius)
                    {
                        if (lineRectOverlap({unit->pos.x, unit->pos.y, static_cast<float>(unit->tex.width), static_cast<float>(unit->tex.height)}, line))
                        {
                            blockedCt += 1;
                            break;
                        }
                    }
                    for (auto &obstacle2 : obstaclesInAttackRadius)
                    {
                        if (lineRectOverlap({obstacle2->pos.x, obstacle2->pos.y, static_cast<float>(obstacle2->tex.width), static_cast<float>(obstacle2->tex.height)}, line))
                        {
                            blockedCt += 1;
                            break;
                        }
                    }
                }
                if (blockedCt < 32)
                {
                    obstacle->health -= projectile->damage;
                }
            }
        }
    }
}