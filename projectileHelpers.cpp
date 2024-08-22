#include "include/projectileHelpers.h"

SharedPointer<Projectile> createProjectile(
    VectorSharedPointer<Projectile> &allProjectiles,
    Texture2D tex,
    Position originPos,
    // Position &destinationPos,
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

            projectile->currentPos.x += moveX;
            projectile->currentPos.y += moveY;
        }
        else
        {
            // handle destruction and damage
        }
    }
}

void precomputeProjectileBehavior(SharedPointer<Projectile> &projectile, SharedPointer<Unit> &selectedUnit, SharedPointer<Ability> &selectedAbility, SharedPointer<Tile> &hoveredTile, SharedPointer<Player> &player, VectorSharedPointer<Unit> &allUnits, VectorSharedPointer<Obstacle> &allObstacles, Vector2 &worldMousePos, VectorSharedPointer<GridSubdivision> &allGridSubdivisions, VectorSharedPointer<Projectile> &allProjectiles, Texture2D projectileTex)
{
    Rectangle originRect = {selectedUnit->pos.x, selectedUnit->pos.y, static_cast<float>(selectedUnit->tex.width), static_cast<float>(selectedUnit->tex.height)};
    Position selectedUnitCenter = getRectCenter(originRect);
    Rectangle targetRect = {hoveredTile->pos.x, hoveredTile->pos.y, static_cast<float>(hoveredTile->tex.width), static_cast<float>(hoveredTile->tex.height)};
    Position hoveredTileCenter = getRectCenter({hoveredTile->pos.x, hoveredTile->pos.y, static_cast<float>(hoveredTile->tex.width), static_cast<float>(hoveredTile->tex.height)});
    float angleToRotationTarget = getAngleBetweenPoints(selectedUnitCenter, hoveredTileCenter);

    // 1. Accuracy Roll. If accuracy fails, the target rect will not be hit. Either it will miss the target or it will hit a blocker
    // 2. Draw lines between every corner of both rects
    // 3. Iterate over all units and obstacles, ignore selectedUnitCenter and unit/obstacle at targetRect
    // 4. Add all blocking units/obstacles to a vector
    // 5. If unit

    //////////////////////////////////////////////////////////////////////
    /////////////////// PRECOMPUTE PROJECTILE BEHAVIOR ///////////////////
    /////////////////////////////////////////////////////////////////////

    // establish accuracy radius
    Rectangle accuracyRadiusRect = createRectAroundRect((Rectangle){hoveredTile->pos.x, hoveredTile->pos.y, static_cast<float>(hoveredTile->tex.width), static_cast<float>(hoveredTile->tex.height)}, selectedAbility->accuracyRadius);

    std::vector<Line> hoveredTileCornerToCornerRays;

    VectorSharedPointer<Obstacle> obstaclesInDamageRadius;
    VectorSharedPointer<Unit> unitsInDamageRadius;
    VectorSharedPointer<Obstacle> obstaclesInDamageRadiusToModify; // NOTE derived from inAccuracyRadius vectors, passed to projectile, since derived from vector contains pointers to refs, modifying contents in this vector should modify original
    VectorSharedPointer<Unit> unitsInDamageRadiusToModify;         // NOTE derived from inAccuracyRadius vectors, passed to projectile, since derived from vector contains pointers to refs, modifying contents in this vector should modify original

    // // detect collisions en route to destination
    VectorSharedPointer<Obstacle> blockingObstacles;
    VectorSharedPointer<Unit> blockingUnits;

    std::vector<Line> linesFromOriginRectToEndRect;

    VectorSharedPointer<Line> obstacleCornerLines;
    VectorSharedPointer<Line> unitCornerLines;

    SharedPointer<Obstacle> obstacleAtStart = nullptr;
    SharedPointer<Obstacle> obstacleAtEnd = nullptr;
    SharedPointer<Unit> unitAtEnd = nullptr;

    // Step 1: choose a rectangle inside the accuracy radius to be the destination
    // Step 2: Check if any obstacle or unit is in between the origin and destination and if those obstacles are passable, considering if the projectile collides en route or not
    // Step 3: If the projectile is able to reach the destination, check if there exists and obstacle or unit at that destination, and consider it's stoppage type
    // Step 4: Damage non-blocked objects in the damage radius

    targetRect = getRandomItemFromVector(deduceInnerRectangles(accuracyRadiusRect));

    linesFromOriginRectToEndRect = getCornerToCornerLines(originRect, targetRect);

    bool projectileHitsTarget = calculateChance(projectile->accuracy);

    std::vector<std::string> seenIDs;

    if (!projectileHitsTarget)
    {

        // get obstacles at start and end and unit at end, if they exist
        for (auto &obstacle : allObstacles)
        {
            if (CheckCollisionRecs({obstacle->pos.x, obstacle->pos.y, static_cast<float>(obstacle->tex.width), static_cast<float>(obstacle->tex.height)}, originRect))
            {
                obstacleAtStart = obstacle;
            }
            if (CheckCollisionRecs({obstacle->pos.x, obstacle->pos.y, static_cast<float>(obstacle->tex.width), static_cast<float>(obstacle->tex.height)}, targetRect))
            {
                obstacleAtEnd = obstacle;
            }
        }
        for (auto &unit : allUnits)
        {
            if (CheckCollisionRecs({unit->pos.x, unit->pos.y, static_cast<float>(unit->tex.width), static_cast<float>(unit->tex.height)}, targetRect))
            {
                unitAtEnd = unit;
            }
        }

        // if () // account for case that start and end rect are same

        if (projectile->canCollideBeforeDestination)
        {
            for (auto &line : linesFromOriginRectToEndRect)
            {
                for (auto &obstacle : allObstacles)
                {
                    // obstacle at origin rect cannot be a blocker between origin and destination, unless cannot attack from this obstacle (e.g. a cave)
                    if ((obstacleAtStart && obstacle->uuid == obstacleAtStart->uuid) || obstacleAtEnd && obstacle->uuid == obstacleAtEnd->uuid)
                    {
                        continue;
                    }
                    if (!doesVectorContainString(seenIDs, obstacle->uuid))
                    {
                        seenIDs.push_back(obstacle->uuid);
                        blockingObstacles.push_back(obstacle);
                    }
                    // if line intersects, add this obstacle to blocking obstacles, if it is not already there
                }

                for (auto &unit : allUnits)
                {
                    // unit at origin rect cannot be a blocker between origin and destination
                }
            }

            // sort obstacles/units by closest to origin
            if (blockingObstacles.size() > 0)
            {
                // sort
                // get first blocking item, which becomes the end rect
                // ignore stoppage chance never
            }
            if (blockingUnits.size() > 0)
            {
                // sort
                // get first blocking item, which becomes the end rect
                // ignore stoppage chance never
            }
            // choose closest blocking obstacle vs unit
            // for each potentially blocking item, including one null rect (-1, -1) (probably use a "sailOff" flag on the projectile instead) choose randomly to become end rect
            // if null rect, the projectile should move along the angle until it leaves the map, at which point it gets destroyed (in the movement function)
        }
    }
    else
    {
        projectile->endRect = targetRect;
    }

    // for obstacles/units in damage radius, compute damages based on where the projectile lands (if it lands on always blocking, only that item gets damaged, and blockers between end rect and other obstacles/units may or may not be damaged based on stoppage of blockers)
}

// void sMoveProjectiles(VectorSharedPointer<Projectile> &allProjectiles, VectorSharedPointer<Unit> &allUnits, VectorSharedPointer<Obstacle> &allObstacles, float deltaTime)
// {
//     for (auto &projectile : allProjectiles)
//     {
//         if (!projectile->shouldDestroy)
//         {
//             // Calculate movement in the x and y directions based on the angle
//             float moveStep = projectile->speed * deltaTime;
//             float angleInRadians = projectile->facingAngle * (PI / 180.0f);
//             float moveX = moveStep * std::cos(angleInRadians);
//             float moveY = moveStep * std::sin(angleInRadians);

//             if (CheckCollisionRecs({projectile->currentPos.x, projectile->currentPos.y, static_cast<float>(projectile->tex.width), static_cast<float>(projectile->tex.height)}, projectile->targetRect))
//             {
//                 projectile->endRect = projectile->targetRect;
//                 projectile->shouldDestroy = true;
//             }

//             for (auto &unit : allUnits)
//             {
//                 if (projectile->originUnitUuid != unit->uuid)
//                 {
//                     if (CheckCollisionRecs({projectile->currentPos.x, projectile->currentPos.y, static_cast<float>(projectile->tex.width), static_cast<float>(projectile->tex.height)}, {unit->pos.x, unit->pos.y, static_cast<float>(unit->tex.width), static_cast<float>(unit->tex.height)}))
//                     {
//                         if (unit->projectileStoppage == ProjectileStoppage::ALWAYS)
//                         {
//                             if (projectile->canCollideBeforeDestination)
//                             {
//                                 projectile->endRect = {unit->pos.x, unit->pos.y, static_cast<float>(unit->tex.width), static_cast<float>(unit->tex.height)};
//                                 projectile->shouldDestroy = true;
//                             }
//                         }
//                         if (unit->projectileStoppage == ProjectileStoppage::SOMETIMES)
//                         {
//                             if (projectile->canCollideBeforeDestination)
//                             {
//                                 if (!doesVectorContainString(projectile->idsToIgnoreInFlight, unit->uuid))
//                                 {
//                                     if (!calculateChance(projectile->accuracy))
//                                     {
//                                         projectile->endRect = {unit->pos.x, unit->pos.y, static_cast<float>(unit->tex.width), static_cast<float>(unit->tex.height)};
//                                         projectile->shouldDestroy = true;
//                                     }
//                                     else
//                                     {
//                                         projectile->idsToIgnoreInFlight.push_back(unit->uuid);
//                                     }
//                                 }
//                             }
//                         }
//                     }
//                 }
//             }

//             for (auto &obstacle : allObstacles)
//             {
//                 if (projectile->originUnitUuid != obstacle->uuid)
//                 {
//                     if (CheckCollisionRecs({projectile->currentPos.x, projectile->currentPos.y, static_cast<float>(projectile->tex.width), static_cast<float>(projectile->tex.height)}, {obstacle->pos.x, obstacle->pos.y, static_cast<float>(obstacle->tex.width), static_cast<float>(obstacle->tex.height)}))
//                     {
//                         if (obstacle->projectileStoppage == ProjectileStoppage::ALWAYS)
//                         {
//                             if (projectile->canCollideBeforeDestination)
//                             {
//                                 projectile->endRect = {obstacle->pos.x, obstacle->pos.y, static_cast<float>(obstacle->tex.width), static_cast<float>(obstacle->tex.height)};
//                                 projectile->shouldDestroy = true;
//                             }
//                         }
//                         if (obstacle->projectileStoppage == ProjectileStoppage::SOMETIMES)
//                         {
//                             if (projectile->canCollideBeforeDestination)
//                             {
//                                 if (!doesVectorContainString(projectile->idsToIgnoreInFlight, obstacle->uuid))
//                                 {
//                                     if (!calculateChance(projectile->accuracy))
//                                     {
//                                         projectile->endRect = {obstacle->pos.x, obstacle->pos.y, static_cast<float>(obstacle->tex.width), static_cast<float>(obstacle->tex.height)};
//                                         projectile->shouldDestroy = true;
//                                     }
//                                     else
//                                     {
//                                         projectile->idsToIgnoreInFlight.push_back(obstacle->uuid);
//                                     }
//                                 }
//                             }
//                         }
//                     }
//                 }
//             }

//             if (!projectile->shouldDestroy)
//             {
//                 projectile->currentPos.x += moveX;
//                 projectile->currentPos.y += moveY;
//             }

//             // TODO handle non colliding mode projectiles
//         }
//     }
// }

// void sProjectileDamage(std::vector<Projectile> &allProjectiles, std::vector<Unit> &allUnits, std::vector<Obstacle> &allObstacles)
// {
//     // NOTE if poor performance even with position based item chunking, could break steps up across frames
//     for (auto &projectile : allProjectiles)
//     {
//         if (projectile.shouldDestroy)
//         {
//             Rectangle endRect = projectile.endRect;
//             Rectangle accuracyRadiusRect = createRectAroundRect(endRect, projectile.accuracyRadius);
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
//                 if (CheckCollisionRecs(unitRect, accuracyRadiusRect))
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
//                 if (CheckCollisionRecs(obstacleRect, accuracyRadiusRect))
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

// TODO handle AOE damage
// void sProjectileDamage(VectorSharedPointer<Projectile> &allProjectiles, VectorSharedPointer<Unit> &allUnits, VectorSharedPointer<Obstacle> &allObstacles)
// {
//     for (auto &projectile : allProjectiles)
//     {
//         if (projectile->shouldDestroy)
//         {
//             Rectangle endRect = projectile->endRect;
//             Rectangle accuracyRadiusRect = createRectAroundRect(endRect, projectile->accuracyRadius);
//             SharedPointer<Unit> unitInCenter = nullptr;
//             SharedPointer<Obstacle> obstacleInCenter = nullptr;

//             VectorSharedPointer<Unit> unitsInEffectRect;
//             VectorSharedPointer<Obstacle> obstaclesInEffectRect;

//             bool continueDueToImpassableCenter = false;

//             for (auto &unit : allUnits)
//             {
//                 Rectangle unitRect = {unit->pos.x, unit->pos.y, static_cast<float>(unit->tex.width), static_cast<float>(unit->tex.height)};
//                 if (CheckCollisionRecs(unitRect, endRect))
//                 {
//                     unitInCenter = unit;
//                 }
//                 if (CheckCollisionRecs(unitRect, accuracyRadiusRect))
//                 {
//                     unitsInEffectRect.push_back(unit);
//                 }
//             }

//             for (auto &obstacle : allObstacles)
//             {
//                 Rectangle obstacleRect = {obstacle->pos.x, obstacle->pos.y, static_cast<float>(obstacle->tex.width), static_cast<float>(obstacle->tex.height)};
//                 if (CheckCollisionRecs(obstacleRect, endRect))
//                 {
//                     obstacleInCenter = obstacle;
//                 }
//                 if (CheckCollisionRecs(obstacleRect, accuracyRadiusRect))
//                 {
//                     obstaclesInEffectRect.push_back(obstacle);
//                 }
//             }

//             if (projectile->accuracyRadius == 0 && !unitInCenter && !obstacleInCenter)
//             {
//                 continue;
//             }

//             bool shouldContinue = false;
//             if (projectile->accuracyRadius == 0 && unitInCenter)
//             {
//                 std::cout << "reached" << std::endl;
//                 unitInCenter->health -= projectile->damage;
//                 shouldContinue = true;
//             }

//             if (projectile->accuracyRadius == 0 && obstacleInCenter)
//             {
//                 obstacleInCenter->health -= projectile->damage;
//                 shouldContinue = true;
//             }

//             if (shouldContinue)
//             {
//                 continue;
//             }

//             if (unitInCenter)
//             {
//                 unitInCenter->health -= projectile->damage;
//                 if (unitInCenter->projectileStoppage == ProjectileStoppage::ALWAYS)
//                 {
//                     continueDueToImpassableCenter = true;
//                 }
//                 if (unitInCenter->projectileStoppage == ProjectileStoppage::SOMETIMES)
//                 {
//                     if (!calculateChance(projectile->accuracy))
//                     {
//                         continueDueToImpassableCenter = true;
//                     }
//                 }
//                 if (unitsInEffectRect.size() == 1 && obstaclesInEffectRect.size() == 0)
//                 {
//                     continue;
//                 }
//             }

//             if (obstacleInCenter)
//             {
//                 obstacleInCenter->health -= projectile->damage;
//                 if (obstacleInCenter->projectileStoppage == ProjectileStoppage::ALWAYS)
//                 {
//                     continueDueToImpassableCenter = true;
//                 }
//                 if (obstacleInCenter->projectileStoppage == ProjectileStoppage::SOMETIMES)
//                 {
//                     if (!calculateChance(projectile->accuracy))
//                     {
//                         continueDueToImpassableCenter = true;
//                     }
//                 }
//                 if (obstaclesInEffectRect.size() == 1 && unitsInEffectRect.size() == 0)
//                 {
//                     continue;
//                 }
//             }

//             // if made it to this point, means all directly adjacent items can be damaged
//             if (projectile->accuracyRadius == 1)
//             {
//                 for (auto &unit : unitsInEffectRect)
//                 {
//                     unit->health -= projectile->damage;
//                 }
//                 for (auto &obstacle : obstaclesInEffectRect)
//                 {
//                     obstacle->health -= projectile->damage;
//                 }
//                 continue;
//             }

//             // facts:
//             // if accuracyRadius == 0 and no center unit and no center obstacle, continue;
//             // if only 1 unit and no obstacles in effect rect, damage that unit, continue;
//             // if only 1 obstacle and no units in effect rect, damage that obstacle, continue;
//             // if unitInCenter and always stops projectile, damage and continue;
//             // if obstacleInCenter and always stops projectile, damage and continue;
//             // if unitInCenter and sometimes stops projectile, roll, if !, damage and continue;
//             // if obstacleInCenter and sometimes stops projectile, roll, if !, damage and continue;

//             if (continueDueToImpassableCenter)
//             {
//                 continue;
//             }

//             if (unitsInEffectRect.size() > 0 || obstaclesInEffectRect.size() > 0)
//             {
//                 // NOTE at this point, if center unit or obstacle exist, they were already damaged

//                 std::vector<Line> lines = createLinesWithinRect(endRect, accuracyRadiusRect, 5);
//                 for (auto &line : lines)
//                 {
//                     for (auto &unit : unitsInEffectRect)
//                     {
//                         if (unitInCenter && unitInCenter->uuid == unit->uuid)
//                         {
//                             continue;
//                         }
//                         if (lineRectOverlap({unit->pos.x, unit->pos.y, static_cast<float>(unit->tex.width), static_cast<float>(unit->tex.height)}, line))
//                         {
//                         }
//                     }
//                     for (auto &obstacle : obstaclesInEffectRect)
//                     {
//                         if (obstacleInCenter && obstacleInCenter->uuid == obstacle->uuid)
//                         {
//                             continue;
//                         }
//                         if (lineRectOverlap({obstacle->pos.x, obstacle->pos.y, static_cast<float>(obstacle->tex.width), static_cast<float>(obstacle->tex.height)}, line))
//                         {
//                         }
//                     }
//                 }

//                 // check if there is a unit in between this unit and center
//                 // if false or if true and unit never stops projectile, damage unit
//                 // if true, if unit always stops projectile, continue
//                 // if true, if unit sometimes stops projectile, roll accuracy chance to damage unit

//                 // check if there is an obstacle in between this obstacle and center
//                 // if false or if true and obstacle never stops projectile, damage obstacle
//                 // if true, if obstacle always stops projectile, continue
//                 // if true, if obstacle sometimes stops projectile, roll accuracy chance to damage obstacle

//                 for (auto &unit : unitsInEffectRect)
//                 {
//                     if (unit->uuid == unitInCenter->uuid)
//                     {
//                         continue;
//                     }

//                     bool lineClear = false;

//                     for (auto &line : getCornerToCornerLines({unit->pos.x, unit->pos.y, static_cast<float>(unit->tex.width), static_cast<float>(unit->tex.height)}, endRect))
//                     {
//                         bool obstacleOrUnitFound = false;

//                         for (auto &unit2 : unitsInEffectRect)
//                         {
//                             if (unit2->uuid == unit->uuid)
//                             {
//                                 continue;
//                             }

//                             if (lineRectOverlap({unit2->pos.x, unit2->pos.y, static_cast<float>(unit2->tex.width), static_cast<float>(unit2->tex.height)}, line))
//                             {
//                                 if (unitInCenter && unit2->uuid == unitInCenter->uuid)
//                                 {
//                                     continue;
//                                 }
//                                 // TODO: Account for projectile stoppage type and accuracy of blocking item
//                                 obstacleOrUnitFound = true;
//                                 break;
//                             }
//                         }

//                         if (!obstacleOrUnitFound)
//                         {
//                             for (auto &obstacle : obstaclesInEffectRect)
//                             {
//                                 if (lineRectOverlap({obstacle->pos.x, obstacle->pos.y, static_cast<float>(obstacle->tex.width), static_cast<float>(obstacle->tex.height)}, line))
//                                 {
//                                     if (obstacleInCenter && obstacle->uuid == obstacleInCenter->uuid)
//                                     {
//                                         continue;
//                                     }
//                                     obstacleOrUnitFound = true;
//                                     // TODO: Account for projectile stoppage type and accuracy of blocking item
//                                     break;
//                                 }
//                             }
//                         }

//                         if (!obstacleOrUnitFound)
//                         {
//                             lineClear = true;
//                             break;
//                         }
//                     }

//                     if (lineClear)
//                     {
//                         std::cout << "unit damaged" << std::endl;
//                         // Damage unit
//                         unit->health -= projectile->damage;
//                         // TODO: Implement the damage logic here
//                     }
//                 }

//                 for (auto &obstacle : obstaclesInEffectRect)
//                 {
//                     if (obstacle->uuid == obstacleInCenter->uuid)
//                     {
//                         continue;
//                     }

//                     bool lineClear = false;

//                     for (auto &line : getCornerToCornerLines({obstacle->pos.x, obstacle->pos.y, static_cast<float>(obstacle->tex.width), static_cast<float>(obstacle->tex.height)}, endRect))
//                     {
//                         bool obstacleOrUnitFound = false;

//                         for (auto &obstacle2 : obstaclesInEffectRect)
//                         {
//                             if (obstacle2->uuid == obstacle2->uuid)
//                             {
//                                 continue;
//                             }

//                             if (lineRectOverlap({obstacle2->pos.x, obstacle2->pos.y, static_cast<float>(obstacle2->tex.width), static_cast<float>(obstacle2->tex.height)}, line))
//                             {
//                                 if (obstacleInCenter && obstacle2->uuid == obstacleInCenter->uuid)
//                                 {
//                                     continue;
//                                 }
//                                 // TODO: Account for projectile stoppage type and accuracy of blocking item
//                                 obstacleOrUnitFound = true;
//                                 break;
//                             }
//                         }

//                         if (!obstacleOrUnitFound)
//                         {
//                             for (auto &unit : unitsInEffectRect)
//                             {
//                                 if (lineRectOverlap({unit->pos.x, unit->pos.y, static_cast<float>(unit->tex.width), static_cast<float>(unit->tex.height)}, line))
//                                 {
//                                     if (unitInCenter && unit->uuid == obstacleInCenter->uuid)
//                                     {
//                                         continue;
//                                     }
//                                     obstacleOrUnitFound = true;
//                                     // TODO: Account for projectile stoppage type and accuracy of blocking item
//                                     break;
//                                 }
//                             }
//                         }

//                         if (!obstacleOrUnitFound)
//                         {
//                             lineClear = true;
//                             break;
//                         }
//                     }

//                     if (lineClear)
//                     {
//                         std::cout << "hello" << std::endl;
//                         // Damage obstacle
//                         obstacle->health -= projectile->damage;
//                         // TODO: Implement the damage logic here
//                     }
//                 }
//             }
//         }
//     }
// }