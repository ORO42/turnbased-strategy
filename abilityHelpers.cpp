#include "include/abilityHelpers.h"

void sSelectAbility(Unit *&selectedUnit, Ability *&selectedAbility, Vector2 &worldMousePos)
{
    static int currentAbilityIdx = -1; // Holds the index of the currently selected ability
    int requestedAbilityIdx = -1;

    if (selectedUnit)
    {
        if (IsKeyPressed(KEY_ONE))
        {
            requestedAbilityIdx = 0;
        }
        else if (IsKeyPressed(KEY_TWO))
        {
            requestedAbilityIdx = 1;
        }
        else if (IsKeyPressed(KEY_THREE))
        {
            requestedAbilityIdx = 2;
        }
        else if (IsKeyPressed(KEY_FOUR))
        {
            requestedAbilityIdx = 3;
        }
        else if (IsKeyPressed(KEY_FIVE))
        {
            requestedAbilityIdx = 4;
        }
        else if (IsKeyPressed(KEY_SIX))
        {
            requestedAbilityIdx = 5;
        }
        else if (IsKeyPressed(KEY_SEVEN))
        {
            requestedAbilityIdx = 6;
        }
        else if (IsKeyPressed(KEY_EIGHT))
        {
            requestedAbilityIdx = 7;
        }
        else if (IsKeyPressed(KEY_NINE))
        {
            requestedAbilityIdx = 8;
        }
        else if (IsKeyDown(KEY_ONE) && IsKeyDown(KEY_LEFT_SHIFT))
        {
            requestedAbilityIdx = 9;
        }
        else if (IsKeyDown(KEY_TWO) && IsKeyDown(KEY_LEFT_SHIFT))
        {
            requestedAbilityIdx = 10;
        }
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
            selectedAbility = nullptr;
            currentAbilityIdx = -1;
        }
        else
        {
            Ability &ability = selectedUnit->abilities[requestedAbilityIdx];
            if (selectedUnit->ap >= ability.unitApCost && selectedUnit->xp >= ability.unitXpRequirement)
            {
                selectedAbility = &ability;
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

void sUseAbility(Unit *&selectedUnit, Ability *&selectedAbility, Tile *&hoveredTile, Player &player, std::vector<Unit> &allUnits, std::vector<Obstacle> &allObstacles)
{
    std::vector<Unit *> unitsToAffect = {};
    std::vector<Obstacle *> obstaclesToAffect = {};

    bool useAbility = false;

    if (selectedAbility)
    {
        if (selectedAbility->canAffectUnit || selectedAbility->canAffectObstacle || selectedAbility->canAffectSelf)
        {
            Rectangle effectRadiusRect = createRectAroundRect((Rectangle){hoveredTile->pos.x, hoveredTile->pos.y, static_cast<float>(hoveredTile->tex.width), static_cast<float>(hoveredTile->tex.height)}, selectedAbility->effectRadius);
            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            {

                if (selectedAbility->canAffectUnit)
                {
                    for (auto &unit : allUnits)
                    {
                        if (CheckCollisionRecs(effectRadiusRect, (Rectangle){unit.pos.x, unit.pos.y, static_cast<float>(unit.tex.width), static_cast<float>(unit.tex.height)}))
                        {
                            unitsToAffect.push_back(&unit); // TODO does this create a copy or reference the object in the main vector?
                        }
                    }
                }

                if (selectedAbility->canAffectObstacle)
                {
                    for (auto &obstacle : allObstacles)
                    {
                        if (CheckCollisionRecs(effectRadiusRect, (Rectangle){obstacle.pos.x, obstacle.pos.y, static_cast<float>(obstacle.tex.width), static_cast<float>(obstacle.tex.height)}))
                        {
                            obstaclesToAffect.push_back(&obstacle);
                        }
                    }
                }

                if (selectedAbility->canAffectSelf)
                {
                    if (CheckCollisionRecs(effectRadiusRect, (Rectangle){selectedUnit->pos.x, selectedUnit->pos.y, static_cast<float>(selectedUnit->tex.width), static_cast<float>(selectedUnit->tex.height)}))
                    {
                        unitsToAffect.push_back(selectedUnit);
                    }
                }
                useAbility = true;
            }
        }
        if (selectedAbility->onlyAffectsSelf)
        {
            selectedAbility = nullptr;
        }
        if (selectedAbility->abilityType == AbilityTypes::ROTATE)
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            {
                Position selectedUnitCenter = getRectCenter((Rectangle){selectedUnit->pos.x, selectedUnit->pos.y, static_cast<float>(selectedUnit->tex.width), static_cast<float>(selectedUnit->tex.height)});
                Position hoveredTileCenter = getRectCenter((Rectangle){hoveredTile->pos.x, hoveredTile->pos.y, static_cast<float>(hoveredTile->tex.width), static_cast<float>(hoveredTile->tex.height)});
                float angleToRotationTarget = getAngleBetweenPoints(selectedUnitCenter, hoveredTileCenter);
                selectedUnit->facingAngle = angleToRotationTarget;
                useAbility = true;
            }
        }

        if (useAbility)
        {
            useAbility = false;
            for (auto *unit : unitsToAffect)
            {
                unit->health -= selectedAbility->damage;
            }
            for (auto *obstacle : obstaclesToAffect)
            {
                obstacle->health -= selectedAbility->damage;
            }

            selectedUnit->ap -= selectedAbility->unitApCost;
            player.ap -= selectedAbility->playerApCost;
        }
    }
}

void sAutoDeselectAbility(Unit *&selectedUnit, Ability *&selectedAbility)
{
    if (selectedUnit && selectedAbility)
    {
        if (selectedUnit->ap < selectedAbility->unitApCost || (selectedAbility->useQty && selectedAbility->qty == 0))
        {
            selectedAbility = nullptr;
        }
    }
}