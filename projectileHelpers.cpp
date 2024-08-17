#include "include/projectileHelpers.h"

void createProjectile(
    std::vector<Projectile> &allProjectiles,
    Texture2D tex,
    Position &originPos,
    // Position &destinationPos,
    Rectangle &targetRect,
    std::string &originUnitUuid,
    Teams &team,
    float &facingAngle,
    float speed,
    bool canCollideBeforeDestination,
    bool causesExplosion,
    int projectileEffectRadius,
    float damage,
    float &accuracy)
{
    Projectile newProjectile;
    newProjectile.tex = tex;
    newProjectile.originPos = originPos;
    // newProjectile.destinationPos = destinationPos;
    newProjectile.targetRect = targetRect;
    newProjectile.currentPos = originPos;
    newProjectile.originUnitUuid = originUnitUuid;
    newProjectile.team = team;
    newProjectile.facingAngle = facingAngle;
    newProjectile.speed = speed;
    newProjectile.canCollideBeforeDestination = canCollideBeforeDestination;
    newProjectile.shouldDestroy = false;
    newProjectile.causesExplosion = causesExplosion;
    newProjectile.effectRadius = projectileEffectRadius;
    newProjectile.damage = damage;
    newProjectile.accuracy = accuracy;

    allProjectiles.push_back(newProjectile);
}

void sMoveProjectiles(std::vector<Projectile> &allProjectiles, std::vector<Unit> &allUnits, std::vector<Obstacle> &allObstacles, float &deltaTime, Rectangle &debug_endRect)
{
    for (auto &projectile : allProjectiles)
    {
        if (!projectile.shouldDestroy)
        {
            // Calculate movement in the x and y directions based on the angle
            float moveStep = projectile.speed * deltaTime;
            float angleInRadians = projectile.facingAngle * (PI / 180.0f);
            float moveX = moveStep * std::cos(angleInRadians);
            float moveY = moveStep * std::sin(angleInRadians);

            if (CheckCollisionRecs({projectile.currentPos.x, projectile.currentPos.y, static_cast<float>(projectile.tex.width), static_cast<float>(projectile.tex.height)}, projectile.targetRect))
            {
                projectile.endRect = projectile.targetRect;
                debug_endRect = projectile.endRect;
                projectile.shouldDestroy = true;
            }

            for (auto &unit : allUnits)
            {
                if (projectile.originUnitUuid != unit.uuid)
                {
                    if (CheckCollisionRecs({projectile.currentPos.x, projectile.currentPos.y, static_cast<float>(projectile.tex.width), static_cast<float>(projectile.tex.height)}, {unit.pos.x, unit.pos.y, static_cast<float>(unit.tex.width), static_cast<float>(unit.tex.height)}))
                    {
                        if (unit.projectileStoppage == ProjectileStoppage::ALWAYS)
                        {
                            if (projectile.canCollideBeforeDestination)
                            {
                                projectile.endRect = {unit.pos.x, unit.pos.y, static_cast<float>(unit.tex.width), static_cast<float>(unit.tex.height)};
                                debug_endRect = projectile.endRect;
                                projectile.shouldDestroy = true;
                            }
                        }
                        if (unit.projectileStoppage == ProjectileStoppage::SOMETIMES)
                        {
                            if (projectile.canCollideBeforeDestination)
                            {
                                if (!doesVectorContainString(projectile.idsToIgnoreInFlight, unit.uuid))
                                {
                                    if (!calculateChance(projectile.accuracy))
                                    {
                                        projectile.endRect = {unit.pos.x, unit.pos.y, static_cast<float>(unit.tex.width), static_cast<float>(unit.tex.height)};
                                        debug_endRect = projectile.endRect;
                                        projectile.shouldDestroy = true;
                                    }
                                    else
                                    {
                                        projectile.idsToIgnoreInFlight.push_back(unit.uuid);
                                    }
                                }
                            }
                        }
                    }
                }
            }

            for (auto &obstacle : allObstacles)
            {
                if (projectile.originUnitUuid != obstacle.uuid)
                {
                    if (CheckCollisionRecs({projectile.currentPos.x, projectile.currentPos.y, static_cast<float>(projectile.tex.width), static_cast<float>(projectile.tex.height)}, {obstacle.pos.x, obstacle.pos.y, static_cast<float>(obstacle.tex.width), static_cast<float>(obstacle.tex.height)}))
                    {
                        if (obstacle.projectileStoppage == ProjectileStoppage::ALWAYS)
                        {
                            if (projectile.canCollideBeforeDestination)
                            {
                                projectile.endRect = {obstacle.pos.x, obstacle.pos.y, static_cast<float>(obstacle.tex.width), static_cast<float>(obstacle.tex.height)};
                                debug_endRect = projectile.endRect;
                                projectile.shouldDestroy = true;
                            }
                        }
                        if (obstacle.projectileStoppage == ProjectileStoppage::SOMETIMES)
                        {
                            if (projectile.canCollideBeforeDestination)
                            {
                                if (!doesVectorContainString(projectile.idsToIgnoreInFlight, obstacle.uuid))
                                {
                                    if (!calculateChance(projectile.accuracy))
                                    {
                                        projectile.endRect = {obstacle.pos.x, obstacle.pos.y, static_cast<float>(obstacle.tex.width), static_cast<float>(obstacle.tex.height)};
                                        debug_endRect = projectile.endRect;
                                        projectile.shouldDestroy = true;
                                    }
                                    else
                                    {
                                        projectile.idsToIgnoreInFlight.push_back(obstacle.uuid);
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if (!projectile.shouldDestroy)
            {
                projectile.currentPos.x += moveX;
                projectile.currentPos.y += moveY;
            }

            // TODO handle non colliding mode projectiles
        }
    }
}

// void sProjectileDamage(std::vector<Projectile> &allProjectiles, std::vector<Unit> &allUnits, std::vector<Obstacle> &allObstacles)
// {
//     // NOTE if poor performance even with position based item chunking, could break steps up across frames
//     for (auto &projectile : allProjectiles)
//     {
//         if (projectile.shouldDestroy)
//         {
//             Rectangle endRect = projectile.endRect;
//             Rectangle effectRadiusRect = createRectAroundRect(endRect, projectile.effectRadius);
//             Unit *unitInCenter = nullptr;
//             Obstacle *obstacleInCenter = nullptr;

//             std::vector<Unit *> unitsInEffectRect;
//             std::vector<Obstacle *> obstaclesInEffectRect;

//             std::vector<Unit *> unitsToDamage;
//             std::vector<Obstacle *> obstaclesToDamage;

//             for (auto &unit : allUnits)
//             {
//                 Rectangle unitRect = {unit.pos.x, unit.pos.y, static_cast<float>(unit.tex.width), static_cast<float>(unit.tex.height)};
//                 if (CheckCollisionRecs(unitRect, endRect))
//                 {
//                     unitInCenter = &unit;
//                 }
//                 if (CheckCollisionRecs(unitRect, effectRadiusRect))
//                 {
//                     unitsInEffectRect.push_back(&unit);
//                 }
//             }

//             for (auto &obstacle : allObstacles)
//             {
//                 Rectangle obstacleRect = {obstacle.pos.x, obstacle.pos.y, static_cast<float>(obstacle.tex.width), static_cast<float>(obstacle.tex.height)};
//                 if (CheckCollisionRecs(obstacleRect, endRect))
//                 {
//                     obstacleInCenter = &obstacle;
//                 }
//                 if (CheckCollisionRecs(obstacleRect, effectRadiusRect))
//                 {
//                     obstaclesInEffectRect.push_back(&obstacle);
//                 }
//             }

//             // TODO if obj not behind always stop projectile object, projectile.accuracy % chance to be damaged
//             // NOTE if obstacle and unit occupying same tile, the obstacle is evaluated first

//             if (unitsInEffectRect.size() > 0 || obstaclesInEffectRect.size() > 0)
//             {
//                 if (unitInCenter && !obstacleInCenter)
//                 {
//                     if (unitInCenter->projectileStoppage == ProjectileStoppage::ALWAYS)
//                     {
//                         // damage this unit only
//                         unitInCenter->health -= projectile.damage;
//                         continue;
//                     }
//                 }
//                 if (obstacleInCenter && !unitInCenter)
//                 {
//                     if (obstacleInCenter->projectileStoppage == ProjectileStoppage::ALWAYS)
//                     {
//                         // damage this obstacle only
//                         obstacleInCenter->health -= projectile.damage;
//                         continue;
//                     }
//                 }
//                 if (obstacleInCenter && unitInCenter)
//                 {
//                     unitInCenter->health -= projectile.damage;
//                     obstacleInCenter->health -= projectile.damage;
//                     continue;
//                 }
//             }
//             else
//             {
//                 continue;
//             }

//             for (auto &unit : unitsInEffectRect)
//             {
//                 Rectangle unitRect = {unit->pos.x, unit->pos.y, static_cast<float>(unit->tex.width), static_cast<float>(unit->tex.height)};
//                 if (unitInCenter && (unit->uuid != unitInCenter->uuid))
//                 {
//                     for (auto line : getCornerToCornerLines(unitRect, endRect))
//                     {
//                         for (auto &unit2 : unitsInEffectRect)
//                         {
//                             Rectangle unit2Rect = {unit2->pos.x, unit2->pos.y, static_cast<float>(unit2->tex.width), static_cast<float>(unit2->tex.height)};
//                             if (unitInCenter && (unit->uuid != unitInCenter->uuid) && unit->uuid != unit2->uuid)
//                             {
//                                 if (!lineRectOverlap(unitRect, line))
//                                 {
//                                     unitsToDamage.push_back(unit);
//                                     break;
//                                 }
//                             }
//                         }
//                         for (auto &obstacle : obstaclesInEffectRect)
//                         {
//                             Rectangle obstacleRect = {obstacle->pos.x, obstacle->pos.y, static_cast<float>(obstacle->tex.width), static_cast<float>(obstacle->tex.height)};
//                             if (obstacleInCenter && (obstacle->uuid != obstacleInCenter->uuid))
//                             {
//                                 if (!lineRectOverlap(obstacleRect, line))
//                                 {
//                                     unitsToDamage.push_back(unit);
//                                     break;
//                                 }
//                             }
//                         }
//                     }
//                 }
//             }

//             for (auto &obstacle : obstaclesInEffectRect)
//             {
//                 if (unitInCenter && (obstacle->uuid != obstacleInCenter->uuid))
//                 {
//                 }
//                 Rectangle obstacleRect = {obstacle->pos.x, obstacle->pos.y, static_cast<float>(obstacle->tex.width), static_cast<float>(obstacle->tex.height)};
//             }
//         }
//     }
// }

void sProjectileDamage(std::vector<Projectile> &allProjectiles, std::vector<Unit> &allUnits, std::vector<Obstacle> &allObstacles)
{
    for (auto &projectile : allProjectiles)
    {
        if (projectile.shouldDestroy)
        {
            Rectangle endRect = projectile.endRect;
            Rectangle effectRadiusRect = createRectAroundRect(endRect, projectile.effectRadius);
            Unit *unitInCenter = nullptr;
            Obstacle *obstacleInCenter = nullptr;

            std::vector<Unit *> unitsInEffectRect;
            std::vector<Obstacle *> obstaclesInEffectRect;

            bool continueDueToImpassableCenter = false;

            for (auto &unit : allUnits)
            {
                Rectangle unitRect = {unit.pos.x, unit.pos.y, static_cast<float>(unit.tex.width), static_cast<float>(unit.tex.height)};
                if (CheckCollisionRecs(unitRect, endRect))
                {
                    unitInCenter = &unit;
                }
                if (CheckCollisionRecs(unitRect, effectRadiusRect))
                {
                    unitsInEffectRect.push_back(&unit);
                }
            }

            for (auto &obstacle : allObstacles)
            {
                Rectangle obstacleRect = {obstacle.pos.x, obstacle.pos.y, static_cast<float>(obstacle.tex.width), static_cast<float>(obstacle.tex.height)};
                if (CheckCollisionRecs(obstacleRect, endRect))
                {
                    obstacleInCenter = &obstacle;
                }
                if (CheckCollisionRecs(obstacleRect, effectRadiusRect))
                {
                    obstaclesInEffectRect.push_back(&obstacle);
                }
            }

            if (projectile.effectRadius == 0 && !unitInCenter && !obstacleInCenter)
            {
                continue;
            }

            bool shouldContinue = false;
            if (projectile.effectRadius == 0 && unitInCenter)
            {
                std::cout << "reached" << std::endl;
                unitInCenter->health -= projectile.damage;
                shouldContinue = true;
            }

            if (projectile.effectRadius == 0 && obstacleInCenter)
            {
                obstacleInCenter->health -= projectile.damage;
                shouldContinue = true;
            }

            if (shouldContinue)
            {
                continue;
            }

            if (unitInCenter)
            {
                unitInCenter->health -= projectile.damage;
                if (unitInCenter->projectileStoppage == ProjectileStoppage::ALWAYS)
                {
                    continueDueToImpassableCenter = true;
                }
                if (unitInCenter->projectileStoppage == ProjectileStoppage::SOMETIMES)
                {
                    if (!calculateChance(projectile.accuracy))
                    {
                        continueDueToImpassableCenter = true;
                    }
                }
                if (unitsInEffectRect.size() == 1 && obstaclesInEffectRect.size() == 0)
                {
                    continue;
                }
            }

            if (obstacleInCenter)
            {
                obstacleInCenter->health -= projectile.damage;
                if (obstacleInCenter->projectileStoppage == ProjectileStoppage::ALWAYS)
                {
                    continueDueToImpassableCenter = true;
                }
                if (obstacleInCenter->projectileStoppage == ProjectileStoppage::SOMETIMES)
                {
                    if (!calculateChance(projectile.accuracy))
                    {
                        continueDueToImpassableCenter = true;
                    }
                }
                if (obstaclesInEffectRect.size() == 1 && unitsInEffectRect.size() == 0)
                {
                    continue;
                }
            }

            // if made it to this point, means all directly adjacent items can be damaged
            if (projectile.effectRadius == 1)
            {
                for (auto &unit : unitsInEffectRect)
                {
                    unit->health -= projectile.damage;
                }
                for (auto &obstacle : obstaclesInEffectRect)
                {
                    obstacle->health -= projectile.damage;
                }
                continue;
            }

            // facts:
            // if effectRadius == 0 and no center unit and no center obstacle, continue;
            // if only 1 unit and no obstacles in effect rect, damage that unit, continue;
            // if only 1 obstacle and no units in effect rect, damage that obstacle, continue;
            // if unitInCenter and always stops projectile, damage and continue;
            // if obstacleInCenter and always stops projectile, damage and continue;
            // if unitInCenter and sometimes stops projectile, roll, if !, damage and continue;
            // if obstacleInCenter and sometimes stops projectile, roll, if !, damage and continue;

            if (continueDueToImpassableCenter)
            {
                continue;
            }

            if (unitsInEffectRect.size() > 0 || obstaclesInEffectRect.size() > 0)
            {
                // NOTE at this point, if center unit or obstacle exist, they were already damaged

                // std::vector<Line> lines = createLinesWithinRect(endRect, effectRadiusRect, 5);
                // for (auto &line : lines)
                // {
                //     for (auto &unit : unitsInEffectRect)
                //     {
                //         if (unitInCenter && unitInCenter->uuid == unit->uuid)
                //         {
                //             continue;
                //         }
                //         if (lineRectOverlap({unit->pos.x, unit->pos.y, static_cast<float>(unit->tex.width), static_cast<float>(unit->tex.height)}, line))
                //         {
                //         }
                //     }
                //     for (auto &obstacle : obstaclesInEffectRect)
                //     {
                //         if (obstacleInCenter && obstacleInCenter->uuid == obstacle->uuid)
                //         {
                //             continue;
                //         }
                //         if (lineRectOverlap({obstacle->pos.x, obstacle->pos.y, static_cast<float>(obstacle->tex.width), static_cast<float>(obstacle->tex.height)}, line))
                //         {
                //         }
                //     }
                // }

                // check if there is a unit in between this unit and center
                // if false or if true and unit never stops projectile, damage unit
                // if true, if unit always stops projectile, continue
                // if true, if unit sometimes stops projectile, roll accuracy chance to damage unit

                // check if there is an obstacle in between this obstacle and center
                // if false or if true and obstacle never stops projectile, damage obstacle
                // if true, if obstacle always stops projectile, continue
                // if true, if obstacle sometimes stops projectile, roll accuracy chance to damage obstacle

                for (auto &unit : unitsInEffectRect)
                {
                    if (unit->uuid == unitInCenter->uuid)
                    {
                        continue;
                    }

                    bool lineClear = false;

                    for (auto &line : getCornerToCornerLines({unit->pos.x, unit->pos.y, static_cast<float>(unit->tex.width), static_cast<float>(unit->tex.height)}, endRect))
                    {
                        bool obstacleOrUnitFound = false;

                        for (auto &unit2 : unitsInEffectRect)
                        {
                            if (unit2->uuid == unit->uuid)
                            {
                                continue;
                            }

                            if (lineRectOverlap({unit2->pos.x, unit2->pos.y, static_cast<float>(unit2->tex.width), static_cast<float>(unit2->tex.height)}, line))
                            {
                                if (unitInCenter && unit2->uuid == unitInCenter->uuid)
                                {
                                    continue;
                                }
                                // TODO: Account for projectile stoppage type and accuracy of blocking item
                                obstacleOrUnitFound = true;
                                break;
                            }
                        }

                        if (!obstacleOrUnitFound)
                        {
                            for (auto &obstacle : obstaclesInEffectRect)
                            {
                                if (lineRectOverlap({obstacle->pos.x, obstacle->pos.y, static_cast<float>(obstacle->tex.width), static_cast<float>(obstacle->tex.height)}, line))
                                {
                                    if (obstacleInCenter && obstacle->uuid == obstacleInCenter->uuid)
                                    {
                                        continue;
                                    }
                                    obstacleOrUnitFound = true;
                                    // TODO: Account for projectile stoppage type and accuracy of blocking item
                                    break;
                                }
                            }
                        }

                        if (!obstacleOrUnitFound)
                        {
                            lineClear = true;
                            break;
                        }
                    }

                    if (lineClear)
                    {
                        std::cout << "hello" << std::endl;
                        // Damage unit
                        // TODO: Implement the damage logic here
                    }
                }
                // do same for obstacles
            }
        }
    }
}