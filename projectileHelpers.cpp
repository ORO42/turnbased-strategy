#include "include/projectileHelpers.h"

void createProjectile(
    std::vector<Projectile> &allProjectiles,
    Texture2D tex,
    Position &originPos,
    Position &destinationPos,
    std::string &originUnitUuid,
    Teams &team,
    bool isAerial,
    float &facingAngle,
    float speed,
    bool canCollideBeforeDestination,
    bool causesExplosion,
    int &effectRadius,
    float damage,
    float &accuracy)
{
    Projectile newProjectile;
    newProjectile.tex = tex;
    newProjectile.originPos = originPos;
    newProjectile.destinationPos = destinationPos;
    newProjectile.currentPos = originPos;
    newProjectile.originUnitUuid = originUnitUuid;
    newProjectile.team = team;
    newProjectile.isAerial = isAerial;
    newProjectile.facingAngle = facingAngle;
    newProjectile.speed = speed;
    newProjectile.canCollideBeforeDestination = canCollideBeforeDestination;
    newProjectile.shouldDestroy = false;
    newProjectile.causesExplosion = causesExplosion;
    newProjectile.effectRadius = effectRadius;
    newProjectile.damage = damage;
    newProjectile.accuracy = accuracy;

    allProjectiles.push_back(newProjectile);
}

void sMoveProjectiles(std::vector<Projectile> &allProjectiles, std::vector<Unit> &allUnits, std::vector<Obstacle> &allObstacles, float &deltaTime)
{
    for (auto &projectile : allProjectiles)
    {
        if (projectile.destinationPos.x != -1.0f || projectile.destinationPos.y != -1.0)
        {
            // Calculate movement in the x and y directions based on the angle
            float moveStep = projectile.speed * deltaTime;
            float moveX = moveStep * std::cos(projectile.facingAngle);
            float moveY = moveStep * std::sin(projectile.facingAngle);

            Rectangle hypotheticalEndRect = {-1, -1, -1, -1};
            Rectangle endRect = {-1, -1, -1, -1};
            Rectangle effectRadiusRect;
            std::vector<Unit *> unitsInEffectRadiusRect;
            std::vector<Obstacle *> obstaclesInEffectRadiusRect;

            for (auto &unit : allUnits)
            {
                if (CheckCollisionRecs({projectile.currentPos.x, projectile.currentPos.y, static_cast<float>(projectile.tex.width), static_cast<float>(projectile.tex.height)}, {unit.pos.x, unit.pos.y, static_cast<float>(unit.tex.width), static_cast<float>(unit.tex.height)}))
                {
                    hypotheticalEndRect = {unit.pos.x, unit.pos.y, static_cast<float>(unit.tex.width), static_cast<float>(unit.tex.height)};
                    effectRadiusRect = createRectAroundRect(hypotheticalEndRect, projectile.effectRadius);
                    if (CheckCollisionRecs(hypotheticalEndRect, effectRadiusRect))
                    {
                        unitsInEffectRadiusRect.push_back(&unit);
                    };
                    if (unit.projectileStoppage == ProjectileStoppage::ALWAYS)
                    {
                        if (!projectile.isAerial)
                        {
                            // end at unit center pos
                            endRect = hypotheticalEndRect;
                        }
                    }
                    if (unit.projectileStoppage == ProjectileStoppage::SOMETIMES)
                    {
                        if (!projectile.isAerial)
                        {
                            if (projectile.canCollideBeforeDestination)
                            {
                                if (!calculateChance(projectile.accuracy))
                                {
                                    // end at unit center pos
                                    endRect = hypotheticalEndRect;
                                }
                            }
                        }
                    }
                }
            }

            for (auto &obstacle : allObstacles)
            {
                if (CheckCollisionRecs({projectile.currentPos.x, projectile.currentPos.y, static_cast<float>(projectile.tex.width), static_cast<float>(projectile.tex.height)}, {obstacle.pos.x, obstacle.pos.y, static_cast<float>(obstacle.tex.width), static_cast<float>(obstacle.tex.height)}))
                {
                    hypotheticalEndRect = {obstacle.pos.x, obstacle.pos.y, static_cast<float>(obstacle.tex.width), static_cast<float>(obstacle.tex.height)};
                    effectRadiusRect = createRectAroundRect(hypotheticalEndRect, projectile.effectRadius);
                    if (CheckCollisionRecs(hypotheticalEndRect, effectRadiusRect))
                    {
                        obstaclesInEffectRadiusRect.push_back(&obstacle);
                    };
                    if (obstacle.projectileStoppage == ProjectileStoppage::ALWAYS)
                    {
                        if (!projectile.isAerial)
                        {
                            // end at obstacle center pos
                            endRect = hypotheticalEndRect;
                        }
                    }
                    if (obstacle.projectileStoppage == ProjectileStoppage::SOMETIMES)
                    {
                        if (!projectile.isAerial)
                        {
                            if (projectile.canCollideBeforeDestination)
                            {
                                if (!calculateChance(projectile.accuracy))
                                {
                                    // end at obstacle center pos
                                    endRect = hypotheticalEndRect;
                                }
                            }
                        }
                    }
                }
            }

            Position projectileCenter = getRectCenter({projectile.currentPos.x, projectile.currentPos.y, static_cast<float>(projectile.tex.width), static_cast<float>(projectile.tex.height)});

            if (endRect.x == -1)
            {
                projectile.currentPos.x += moveX;
                projectile.currentPos.y += moveY;
            }
            else if (endRect.x > -1 || (projectile.isAerial && Vector2Distance({projectileCenter.x, projectileCenter.y}, {projectile.destinationPos.x, projectile.destinationPos.y}) <= 2.0f)) // or for aerial projectile check, could check if destination pos inside hypothetical end rect
            {
                projectile.shouldDestroy = true;

                // for obstacles and units in effectRadiusRects
                // draw lines between corners
                // if not overlapping, damage that item

                if (projectile.isAerial)
                {
                    endRect = hypotheticalEndRect;
                }

                for (auto &unit : unitsInEffectRadiusRect)
                {
                    bool shouldDamage = true;
                    std::vector<Line> endRectCornersToUnitCorners = getCornerToCornerLines(endRect, (Rectangle){unit->pos.x, unit->pos.y, static_cast<float>(unit->tex.width), static_cast<float>(unit->tex.height)});
                    for (auto &ray : endRectCornersToUnitCorners)
                    {
                        for (auto &obstacle : obstaclesInEffectRadiusRect)
                        {
                            if (lineRectOverlap((Rectangle){obstacle->pos.x, obstacle->pos.y, static_cast<float>(obstacle->tex.width), static_cast<float>(obstacle->tex.height)}, ray))
                            {
                                shouldDamage = false;
                            }
                        }
                        for (auto &unit2 : unitsInEffectRadiusRect)
                        {
                            if (lineRectOverlap((Rectangle){unit2->pos.x, unit2->pos.y, static_cast<float>(unit2->tex.width), static_cast<float>(unit2->tex.height)}, ray))
                            {
                                shouldDamage = false;
                            }
                        }
                    }

                    if (shouldDamage)
                    {
                        unit->health -= projectile.damage;
                    }
                }
                for (auto &obstacle : obstaclesInEffectRadiusRect)
                {
                    bool shouldDamage = true;
                    std::vector<Line> endRectCornersToObstacleCorners = getCornerToCornerLines(endRect, {obstacle->pos.x, obstacle->pos.y, static_cast<float>(obstacle->tex.width), static_cast<float>(obstacle->tex.height)});
                    for (auto &ray : endRectCornersToObstacleCorners)
                    {
                        for (auto &obstacle2 : obstaclesInEffectRadiusRect)
                        {
                            if (lineRectOverlap({obstacle2->pos.x, obstacle2->pos.y, static_cast<float>(obstacle2->tex.width), static_cast<float>(obstacle2->tex.height)}, ray))
                            {
                                shouldDamage = false;
                            }
                        }
                        for (auto &unit : unitsInEffectRadiusRect)
                        {
                            if (lineRectOverlap({unit->pos.x, unit->pos.y, static_cast<float>(unit->tex.width), static_cast<float>(unit->tex.height)}, ray))
                            {
                                shouldDamage = false;
                            }
                        }
                    }

                    if (shouldDamage)
                    {
                        obstacle->health -= projectile.damage;
                    }
                }
            }
        }
    }
}
