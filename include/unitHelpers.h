#pragma once

#include <iostream>
#include "components.h"
#include "uuid.h"
#include "geometryHelpers.h"

void createUnit(
    UnitType unitType,
    Position pos,
    Player &player,
    Teams team,
    std::vector<Unit> &allUnits,
    Texture2D &unitTex);
void sPositionVisionTrapezoids(std::vector<Unit> &allUnits);
void sVisibility(std::vector<Unit> &allUnits, Player &player);
void DEBUGsHoveredTileOverlappingTrap(Tile *&hoveredTile, std::vector<Unit> &allUnits);
bool shouldRenderUnitDueToVisibility(Unit &unit, Player &player);