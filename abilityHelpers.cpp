#include "include/abilityHelpers.h"

void sSelectAbility(SharedPointer<Unit> &selectedUnit, SharedPointer<Ability> &selectedAbility, Vector2 &worldMousePos)
{
    static int currentAbilityIdx = -1; // Holds the index of the currently selected ability
    int requestedAbilityIdx = -1;

    if (selectedUnit)
    {
        // Handle key inputs for ability selection
        if (IsKeyPressed(KEY_ONE))
            requestedAbilityIdx = 0;
        else if (IsKeyPressed(KEY_TWO))
            requestedAbilityIdx = 1;
        else if (IsKeyPressed(KEY_THREE))
            requestedAbilityIdx = 2;
        else if (IsKeyPressed(KEY_FOUR))
            requestedAbilityIdx = 3;
        else if (IsKeyPressed(KEY_FIVE))
            requestedAbilityIdx = 4;
        else if (IsKeyPressed(KEY_SIX))
            requestedAbilityIdx = 5;
        else if (IsKeyPressed(KEY_SEVEN))
            requestedAbilityIdx = 6;
        else if (IsKeyPressed(KEY_EIGHT))
            requestedAbilityIdx = 7;
        else if (IsKeyPressed(KEY_NINE))
            requestedAbilityIdx = 8;
        else if (IsKeyDown(KEY_ONE) && IsKeyDown(KEY_LEFT_SHIFT))
            requestedAbilityIdx = 9;
        else if (IsKeyDown(KEY_TWO) && IsKeyDown(KEY_LEFT_SHIFT))
            requestedAbilityIdx = 10;
    }
    else
    {
        selectedAbility = nullptr;
        currentAbilityIdx = -1;
        requestedAbilityIdx = -1;
    }

    if (selectedUnit && requestedAbilityIdx != -1 && requestedAbilityIdx < static_cast<int>(selectedUnit->abilities.size()))
    {
        if (requestedAbilityIdx == currentAbilityIdx)
        {
            selectedAbility = nullptr; // Deselect the current ability
            currentAbilityIdx = -1;
        }
        else
        {
            SharedPointer<Ability> ability = selectedUnit->abilities[requestedAbilityIdx]; // Use shared_ptr to access the ability
            if (selectedUnit->ap >= ability->unitApCost && selectedUnit->xp >= ability->unitXpRequirement)
            {
                selectedAbility = ability; // Assign the shared_ptr
                currentAbilityIdx = requestedAbilityIdx;
            }
        }
    }
    else if (requestedAbilityIdx != -1)
    {
        selectedAbility = nullptr;
        currentAbilityIdx = -1;
    }
}

// void sUseAbility(Unit *&selectedUnit, Ability *&selectedAbility, Tile *&hoveredTile, Player &player, std::vector<Unit> &allUnits, std::vector<Obstacle> &allObstacles, Vector2 &worldMousePos)
// {
//     std::vector<Unit *> unitsToAffect = {};
//     std::vector<Obstacle *> obstaclesToAffect = {};

//     bool useAbility = false;

//     if (selectedAbility)
//     {
//         if (selectedAbility->canAffectUnit || selectedAbility->canAffectObstacle || selectedAbility->canAffectSelf)
//         {
//             Rectangle accuracyRadiusRect = createRectAroundRect((Rectangle){hoveredTile->pos.x, hoveredTile->pos.y, static_cast<float>(hoveredTile->tex.width), static_cast<float>(hoveredTile->tex.height)}, selectedAbility->accuracyRadius);
//             if (selectedAbility->canAffectUnit)
//             {
//                 for (auto &unit : allUnits)
//                 {
//                     if (CheckCollisionRecs(accuracyRadiusRect, (Rectangle){unit.pos.x, unit.pos.y, static_cast<float>(unit.tex.width), static_cast<float>(unit.tex.height)}))
//                     {
//                         unitsToAffect.push_back(&unit); // TODO does this create a copy or reference the object in the main vector?
//                     }
//                 }
//             }

//             if (selectedAbility->canAffectObstacle)
//             {
//                 for (auto &obstacle : allObstacles)
//                 {
//                     if (CheckCollisionRecs(accuracyRadiusRect, (Rectangle){obstacle.pos.x, obstacle.pos.y, static_cast<float>(obstacle.tex.width), static_cast<float>(obstacle.tex.height)}))
//                     {
//                         obstaclesToAffect.push_back(&obstacle);
//                     }
//                 }
//             }

//             if (selectedAbility->canAffectSelf)
//             {
//                 if (CheckCollisionRecs(accuracyRadiusRect, (Rectangle){selectedUnit->pos.x, selectedUnit->pos.y, static_cast<float>(selectedUnit->tex.width), static_cast<float>(selectedUnit->tex.height)}))
//                 {
//                     unitsToAffect.push_back(selectedUnit);
//                 }
//             }
//             if (!selectedUnit) // this means its a player ability
//             {
//                 useAbility = true;
//             }
//             else
//             {
//                 Rectangle reachRadiusRect = createRectAroundRect({selectedUnit->pos.x, selectedUnit->pos.y, static_cast<float>(selectedUnit->tex.width), static_cast<float>(selectedUnit->tex.height)}, selectedAbility->reachRadius);
//                 if (CheckCollisionRecs(reachRadiusRect, {hoveredTile->pos.x, hoveredTile->pos.y, static_cast<float>(hoveredTile->tex.width), static_cast<float>(hoveredTile->tex.height)}))
//                 {
//                     if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
//                     {
//                         useAbility = true;
//                     }
//                 }
//             }
//         }
//         if (selectedAbility->onlyAffectsSelf)
//         {
//             // logic here
//             selectedAbility = nullptr;
//         }
//         if (selectedAbility->abilityType == AbilityTypes::ROTATE)
//         {
//             if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
//             {
//                 Position selectedUnitCenter = getRectCenter((Rectangle){selectedUnit->pos.x, selectedUnit->pos.y, static_cast<float>(selectedUnit->tex.width), static_cast<float>(selectedUnit->tex.height)});
//                 Position hoveredTileCenter = getRectCenter((Rectangle){hoveredTile->pos.x, hoveredTile->pos.y, static_cast<float>(hoveredTile->tex.width), static_cast<float>(hoveredTile->tex.height)});
//                 float angleToRotationTarget = getAngleBetweenPoints(selectedUnitCenter, hoveredTileCenter);
//                 selectedUnit->facingAngle = angleToRotationTarget;
//                 useAbility = true;
//             }
//         }
//         if (selectedAbility->abilityType == AbilityTypes::MOVE)
//         {
//             if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
//             {
//                 Rectangle reachRadiusRect = createRectAroundRect({selectedUnit->pos.x, selectedUnit->pos.y, static_cast<float>(selectedUnit->tex.width), static_cast<float>(selectedUnit->tex.height)}, selectedAbility->reachRadius);
//                 if (CheckCollisionRecs(reachRadiusRect, {hoveredTile->pos.x, hoveredTile->pos.y, static_cast<float>(hoveredTile->tex.width), static_cast<float>(hoveredTile->tex.height)}) && !isMouseOverAnyObstacle(allObstacles, worldMousePos) && !isMouseOverAnyUnit(allUnits, worldMousePos))
//                 {
//                     selectedUnit->movePoint = {hoveredTile->pos.x, hoveredTile->pos.y};
//                     useAbility = true;
//                 }
//             }
//         }

//         if (useAbility)
//         {
//             useAbility = false;
//             for (auto *unit : unitsToAffect)
//             {
//                 unit->health -= selectedAbility->damage;
//             }
//             for (auto *obstacle : obstaclesToAffect)
//             {
//                 obstacle->health -= selectedAbility->damage;
//             }

//             selectedUnit->ap -= selectedAbility->unitApCost;
//             player.ap -= selectedAbility->playerApCost;
//         }
//     }
// }

void sUseAbility(SharedPointer<Unit> &selectedUnit, SharedPointer<Ability> &selectedAbility, SharedPointer<Tile> &hoveredTile, SharedPointer<Unit> &hoveredUnit, SharedPointer<Obstacle> &hoveredObstacle, SharedPointer<Player> &player, VectorSharedPointer<Unit> &allUnits, VectorSharedPointer<Obstacle> &allObstacles, Vector2 &worldMousePos, VectorSharedPointer<GridSubdivision> &allGridSubdivisions, VectorSharedPointer<Projectile> &allProjectiles, Texture2D projectileTex)
{
    VectorSharedPointer<Unit> unitsToAffect = {};
    VectorSharedPointer<Obstacle> obstaclesToAffect = {};
    bool useAbility = false;

    if (selectedAbility && selectedUnit) // means it's a unit ability
    {
        switch (selectedAbility->abilityType)
        {

        case AbilityTypes::MOVE:
            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            {
                Rectangle reachRadiusRect = createRectAroundRect({selectedUnit->pos.x, selectedUnit->pos.y, static_cast<float>(selectedUnit->tex.width), static_cast<float>(selectedUnit->tex.height)}, selectedAbility->reachRadius);
                if (CheckCollisionRecs(reachRadiusRect, {hoveredTile->pos.x, hoveredTile->pos.y, static_cast<float>(hoveredTile->tex.width), static_cast<float>(hoveredTile->tex.height)}) && !isMouseOverAnyObstacle(allObstacles, worldMousePos) && !isMouseOverAnyUnit(allUnits, worldMousePos))
                {
                    selectedUnit->movePoint = {hoveredTile->pos.x, hoveredTile->pos.y};
                    useAbility = true;
                }
            }
            break;

        case AbilityTypes::ROTATE:
            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            {
                Position selectedUnitCenter = getRectCenter((Rectangle){selectedUnit->pos.x, selectedUnit->pos.y, static_cast<float>(selectedUnit->tex.width), static_cast<float>(selectedUnit->tex.height)});
                Position hoveredTileCenter = getRectCenter((Rectangle){hoveredTile->pos.x, hoveredTile->pos.y, static_cast<float>(hoveredTile->tex.width), static_cast<float>(hoveredTile->tex.height)});
                float angleToRotationTarget = getAngleBetweenPoints(selectedUnitCenter, hoveredTileCenter);
                selectedUnit->facingAngle = angleToRotationTarget;
                useAbility = true;
            }
            break;

        case AbilityTypes::RIFLE:
            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            {
                Position selectedUnitCenter = getRectCenter({selectedUnit->pos.x, selectedUnit->pos.y, static_cast<float>(selectedUnit->tex.width), static_cast<float>(selectedUnit->tex.height)});
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

                // // establish effect radius
                Rectangle accuracyRadiusRect = createRectAroundRect((Rectangle){hoveredTile->pos.x, hoveredTile->pos.y, static_cast<float>(hoveredTile->tex.width), static_cast<float>(hoveredTile->tex.height)}, selectedAbility->accuracyRadius);

                std::vector<Line> hoveredTileCornerToCornerRays;

                VectorSharedPointer<Obstacle> obstaclesInDamageRadius;
                VectorSharedPointer<Unit> unitsInDamageRadius;
                VectorSharedPointer<Obstacle> obstaclesInDamageRadiusToModify; // NOTE derived from inAccuracyRadius vectors, passed to projectile, since derived from vector contains pointers to refs, modifying contents in this vector should modify original
                VectorSharedPointer<Unit> unitsInDamageRadiusToModify;         // NOTE derived from inAccuracyRadius vectors, passed to projectile, since derived from vector contains pointers to refs, modifying contents in this vector should modify original

                // // detect collisions en route to destination
                VectorSharedPointer<Obstacle> blockingObstacles;
                VectorSharedPointer<Unit> blockingUnits;

                std::vector<Rectangle> potentialEndRects;

                VectorSharedPointer<Rectangle> obstacleCornerLines;
                VectorSharedPointer<Rectangle> unitCornerLines;

                // Step 1: choose a rectangle inside the accuracy radius to be the destination
                // Step 2: Check if any obstacle or unit is in between the origin and destination and if those obstacles are passable, considering if the projectile collides en route or not
                // Step 3: If the projectile is able to reach the destination, check if there exists and obstacle or unit at that destination, and consider it's stoppage type
                // Step 4: Damage non-blocked objects in the damage radius

                targetRect = getRandomItemFromVector(deduceInnerRectangles(accuracyRadiusRect));

                // for (auto &obstacle : allObstacles)
                // {
                //     // find obstacles in desired accuracyRadiusRect
                //     if (CheckCollisionRecs((Rectangle){obstacle.pos.x, obstacle.pos.y, static_cast<float>(obstacle.tex.width), static_cast<float>(obstacle.tex.height)}, accuracyRadiusRect))
                //     {
                //             obstaclesInaccuracyRadius.push_back(&obstacle);
                //             TODO create lines
                //     }

                //     // for (auto &ray : hoveredObstacleCornerToCornerRays)
                //     for (auto &ray : hoveredTileCornerToCornerRays)
                //     {
                //         if (lineRectOverlap((Rectangle){obstacle.pos.x, obstacle.pos.y, static_cast<float>(obstacle.tex.width), static_cast<float>(obstacle.tex.height)}, ray))
                //         {
                //             // Only add if obstacle is not already in blockingObstacles
                //             if (std::find(blockingObstacles.begin(), blockingObstacles.end(), obstacle) == blockingObstacles.end())
                //             {
                //                 blockingObstacles.push_back(obstacle);
                //             }
                //         }
                //     }
                // }

                // for (auto &unit : allUnits)
                // {
                //     // find units in desired accuracyRadiusRect
                //     if (CheckCollisionRecs((Rectangle){unit.pos.x, unit.pos.y, static_cast<float>(unit.tex.width), static_cast<float>(unit.tex.height)}, accuracyRadiusRect))
                //     {
                //             unitsInaccuracyRadius.push_back(&unit);
                //             TODO create lines
                //     }

                //     // for (auto &ray : hoveredUnitCornerToCornerRays)
                //     for (auto &ray : hoveredTileCornerToCornerRays)
                //     {
                //         if (lineRectOverlap((Rectangle){unit.pos.x, unit.pos.y, static_cast<float>(unit.tex.width), static_cast<float>(unit.tex.height)}, ray))
                //         {
                //             if (std::find(blockingUnits.begin(), blockingUnits.end(), unit) == blockingUnits.end())
                //             {
                //                 blockingUnits.push_back(unit);
                //             }
                //         }
                //     }
                // }

                // if (blockingObstacles.size() > 0)
                // {
                //     // Sort blockingObstacles by closest distance to selectedUnitCenter
                //     std::sort(blockingObstacles.begin(), blockingObstacles.end(), [&selectedUnitCenter](const Unit &a, const Unit &b)
                //               { return chebyshevTileDistance({a.pos.x, a.pos.y}, {selectedUnitCenter.x, selectedUnitCenter.y}) < chebyshevTileDistance({b.pos.x, b.pos.y}, {selectedUnitCenter.x, selectedUnitCenter.y}); });

                //     // get first impassable obstacle
                //     auto it = std::find_if(blockingObstacles.begin(), blockingObstacles.end(), [](const Obstacle &o)
                //                            { return o.projectileStoppage == ProjectileStoppage::ALWAYS; });

                //     if (it != blockingObstacles.end())
                //     // means was found
                //     {
                //         destinationRect = {it->pos.x,
                //                            it->pos.y,
                //                            static_cast<float>(it->tex.width),
                //                            static_cast<float>(it->tex.height)};
                //         useOriginalDestinationRect = false;
                //     }
                // }

                // if (blockingUnits.size() > 0)
                // {
                //     // Sort blockingUnits by closest distance to selectedUnitCenter
                //     std::sort(blockingUnits.begin(), blockingUnits.end(), [&selectedUnitCenter](const Unit &a, const Unit &b)
                //               { return chebyshevTileDistance({a.pos.x, a.pos.y}, {selectedUnitCenter.x, selectedUnitCenter.y}) < chebyshevTileDistance({b.pos.x, b.pos.y}, {selectedUnitCenter.x, selectedUnitCenter.y}); });

                //     // get first impassable unit
                //     auto it = std::find_if(blockingUnits.begin(), blockingUnits.end(), [](const Obstacle &u)
                //                            { return u.projectileStoppage == ProjectileStoppage::ALWAYS; });

                //     if (it != blockingUnits.end())
                //     // means was found
                //     {
                //         if (!useOriginalDestinationRect) // if there is already an impassable obstacle, need to choose the closer obstacle/unit
                //         {
                //             Position centerObstacle = getRectCenter({destinationRect.x,
                //                                                      destinationRect.y,
                //                                                      destinationRect.width,
                //                                                      destinationRect.height});
                //             Position centerUnit = getRectCenter({it->pos.x,
                //                                                  it->pos.y,
                //                                                  static_cast<float>(it->tex.width),
                //                                                  static_cast<float>(it->tex.height)});
                //             float distanceBetweenObstacle = Vector2Distance({centerObstacle.x, centerObstacle.y}, {selectedUnitCenter.x, selectedUnitCenter.y});
                //             float distanceBetweenUnit = Vector2Distance({centerUnit.x, centerUnit.y}, {selectedUnitCenter.x, selectedUnitCenter.y});

                //             if (distanceBetweenUnit < distanceBetweenObstacle)
                //             {
                //                 destinationRect = {it->pos.x,
                //                                    it->pos.y,
                //                                    static_cast<float>(it->tex.width),
                //                                    static_cast<float>(it->tex.height)};
                //             }
                //         }
                //         else // else if there is no stopping obstacle, use stopping unit
                //         {
                //             destinationRect = {it->pos.x,
                //                                it->pos.y,
                //                                static_cast<float>(it->tex.width),
                //                                static_cast<float>(it->tex.height)};
                //         }
                //         useOriginalDestinationRect = false;
                //     }
                // }

                // // now calculate hits in effect radius
                // if (useOriginalDestinationRect) // means
                // {
                //     if (calculateChance(selectedUnit->accuracy))
                //     {
                //         // hovered tile is destination, already computed units/obstacles for that accuracyRadius
                //     }
                //     else
                //     {
                //         // choose a unit or obstacle to be the destination
                //     }
                // }
                // else // means need to find units and obstacles for new accuracyRadius around this destination
                // {
                //     obstaclesInaccuracyRadius = {};
                //     unitsInaccuracyRadius = {};
                //     for (auto &obstacle : allObstacles)
                //     {
                //         // find obstacles in desired accuracyRadiusRect
                //         if (CheckCollisionRecs((Rectangle){obstacle.pos.x, obstacle.pos.y, static_cast<float>(obstacle.tex.width), static_cast<float>(obstacle.tex.height)}, accuracyRadiusRect))
                //         {
                //             obstaclesInaccuracyRadius.push_back(&obstacle);
                //         }
                //     }
                //     for (auto &unit : allUnits)
                //     {
                //         // find units in accuracyRadiusRect in the same loop
                //         // find obstacles in desired accuracyRadiusRect
                //         if (CheckCollisionRecs((Rectangle){unit.pos.x, unit.pos.y, static_cast<float>(unit.tex.width), static_cast<float>(unit.tex.height)}, accuracyRadiusRect))
                //         {
                //             unitsInaccuracyRadius.push_back(&unit);
                //         }
                //     }
                // }

                //////////////////////////////////////////////////////////////////////
                /////////////////// PRECOMPUTE PROJECTILE BEHAVIOR ///////////////////
                /////////////////////////////////////////////////////////////////////

                // create projectile
                // createProjectile(allProjectiles, projectileTex, selectedUnitCenter, hoveredTileCenter, selectedUnit->uuid, selectedUnit->team, false, angleToRotationTarget, 100.0f, true, false, selectedAbility->accuracyRadius, selectedAbility->damage, selectedUnit->accuracy);
                createProjectile(allProjectiles, projectileTex, selectedUnitCenter, targetRect, selectedUnit->uuid, selectedUnit->team, angleToRotationTarget, 100.0f, true, false, 0, selectedAbility->damage, selectedUnit->accuracy);

                useAbility = true;
            }
            break;

        case AbilityTypes::MORTAR_VOLLEY:
            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            {
                useAbility = true;
            }
            break;
        }
        case AbilityTypes::
    }
    else if (selectedAbility && !selectedUnit) // means it's a player ability
    {
    }

    if (useAbility)
    {
        useAbility = false;
        selectedUnit->ap -= selectedAbility->unitApCost;
        player->ap -= selectedAbility->playerApCost;
    }
}

void sAutoDeselectAbility(SharedPointer<Unit> &selectedUnit, SharedPointer<Ability> &selectedAbility)
{
    if (selectedUnit && selectedAbility)
    {
        if (selectedUnit->ap < selectedAbility->unitApCost || (selectedAbility->useQty && selectedAbility->qty == 0))
        {
            selectedAbility = nullptr;
        }
    }
}