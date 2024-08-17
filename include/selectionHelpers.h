#pragma once

#include <iostream>
#include "components.h"
#include "unitHelpers.h"

SharedPointer<Unit> getUnitById(std::string uuid, VectorSharedPointer<Unit> &allUnits);
void sSelectUnit(SharedPointer<Unit> &selectedUnit, SharedPointer<Ability> &selectedAbility, VectorSharedPointer<Unit> &allUnits, Vector2 &worldMousePos, SharedPointer<Player> &player);
void sClearStates(SharedPointer<Unit> &selectedUnit, SharedPointer<Ability> &selectedAbility);
