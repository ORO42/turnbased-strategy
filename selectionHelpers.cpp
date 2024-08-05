#include "include/selectionHelpers.h"

Unit getUnitById(std::string uuid, std::vector<Unit> &allUnits)
{
    Unit returnUnit = {};
    for (auto &unit : allUnits)
    {
        if (unit.uuid == uuid)
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

void sSelectUnit(Unit *&selectedUnit, Ability *&selectedAbility, std::vector<Unit> &allUnits, Vector2 &worldMousePos)
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        selectedUnit = nullptr; // Reset to nullptr before the loop

        for (auto &unit : allUnits)
        {
            if (CheckCollisionPointRec(worldMousePos, {unit.pos.x, unit.pos.y, static_cast<float>(unit.tex.width), static_cast<float>(unit.tex.height)}))
            {
                // TODO only can select units that belong to your team
                selectedUnit = &unit;
                break; // Exit the loop once a unit is selected
            }
        }
    }
}

void sClearStates(Unit *&selectedUnit, Ability *&selectedAbility)
{
    if (IsKeyPressed(KEY_C))
    {
        selectedUnit = nullptr;
        selectedAbility = nullptr;
    }
}