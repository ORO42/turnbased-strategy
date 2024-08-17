#include "include/selectionHelpers.h"

SharedPointer<Unit> getUnitById(std::string uuid, VectorSharedPointer<Unit> &allUnits)
{
    SharedPointer<Unit> returnUnit = nullptr;
    for (auto &unit : allUnits)
    {
        if (unit->uuid == uuid)
        {
            returnUnit = unit;
        }
    }
    return returnUnit;
}

// Tile getTileById(std::string uuid, std::vector<Tile> &allTiles)
// {
// }

// Ability getAbilityById(std::string uuid, std::vector<Ability> &allAbilities)
// {
// }

void sSelectUnit(SharedPointer<Unit> &selectedUnit, SharedPointer<Ability> &selectedAbility, VectorSharedPointer<Unit> &allUnits, Vector2 &worldMousePos, SharedPointer<Player> &player)
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        selectedUnit = nullptr; // Reset to nullptr before the loop

        for (auto &unit : allUnits)
        {
            if (shouldRenderUnitDueToVisibility(unit, player) && unit->team == player->team)
            {
                if (CheckCollisionPointRec(worldMousePos, {unit->pos.x, unit->pos.y, static_cast<float>(unit->tex.width), static_cast<float>(unit->tex.height)}))
                {
                    // TODO only can select units that belong to your team
                    selectedUnit = unit;
                    if (selectedUnit->team == Teams::BLUETEAM)
                    {
                        std::cout << "BLUETEAM" << std::endl;
                    }
                    if (selectedUnit->team == Teams::REDTEAM)
                    {
                        std::cout << "REDTEAM" << std::endl;
                    }
                    break; // Exit the loop once a unit is selected
                }
            }
        }
    }
}

void sClearStates(SharedPointer<Unit> &selectedUnit, SharedPointer<Ability> &selectedAbility)
{
    if (IsKeyPressed(KEY_C))
    {
        selectedUnit = nullptr;
        selectedAbility = nullptr;
    }
}