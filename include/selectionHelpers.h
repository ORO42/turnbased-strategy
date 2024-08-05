#pragma once

#include <iostream>
#include "components.h"

Unit getUnitById(std::string uuid, std::vector<Unit> &allUnits);
void sSelectUnit(Unit *&selectedUnit, Ability *&selectedAbility, std::vector<Unit> &allUnits, Vector2 &worldMousePos);
void sClearStates(Unit *&selectedUnit, Ability *&selectedAbility);