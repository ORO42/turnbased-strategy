#pragma once

#include "components.h"
#include "selectionHelpers.h"
#include "gridHelpers.h"
#include "geometryHelpers.h"
#include "sig.h"

void sSelectAbility(Unit *&selectedUnit, Ability *&selectedAbility, Vector2 &worldMousePos);
void sUseAbility(Unit *&selectedUnit, Ability *&selectedAbility, Tile *&hoveredTile, Player &player, std::vector<Unit> &allUnits, std::vector<Obstacle> &allObstacles, Signal<PrepareTrapezoidRotationPayload> &prepareTrapezoidRotationSignal);
void sAutoDeselectAbility(Unit *&selectedUnit, Ability *&selectedAbility);