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
                float angleToTarget = getAngleBetweenPoints(selectedUnitCenter, hoveredTileCenter);

                bool useAngleToTarget = calculateChance(selectedUnit->accuracy);
                bool doesStopAtEndRect = true;

                std::vector<Rectangle> innerRects;

                // TODO get corner to corner lines, and choose angle out of those

                // if (selectedAbility->accuracyRadius > 0)
                // {
                //     innerRects = deduceInnerRectangles(createRectAroundRect(targetRect, selectedAbility->accuracyRadius)); // TODO something wrong with this function
                //     // choose a random rect
                //     targetRect = getRandomItemFromVector(innerRects);
                //     // update angle to target
                //     angleToTarget = getAngleBetweenPoints(selectedUnitCenter, getRectCenter({targetRect.x, targetRect.height, targetRect.x, targetRect.y}));
                // }

                if (!useAngleToTarget)
                {
                    angleToTarget = getRandomAngleWithinRange(angleToTarget, 25.0f);
                }

                createProjectile(allProjectiles, projectileTex, selectedUnitCenter, targetRect, selectedUnit->uuid, selectedUnit->team, angleToTarget, 100.0f, false, false, selectedAbility->accuracyRadius, 0, 25.0f, selectedUnit->accuracy);

                useAbility = true;
            }
            break;

        case AbilityTypes::MORTAR_VOLLEY:
            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            {
                useAbility = true;
            }
            break;

        case AbilityTypes::PLACE_RIFLEMAN:
            break;
        }
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