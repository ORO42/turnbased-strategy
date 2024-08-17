#pragma once

#include <iostream>
#include "components.h"
#include "uuid.h"
#include "geometryHelpers.h"
#include "raymath.h"

void createUnit(
    UnitType unitType,
    Position pos,
    SharedPointer<Player> &player,
    Teams team,
    VectorSharedPointer<Unit> &allUnits,
    Texture2D &unitTex);
void sPositionVisionTrapezoids(VectorSharedPointer<Unit> &allUnits);
void sVisibility(VectorSharedPointer<Unit> &allUnits, SharedPointer<Player> &player);
void DEBUGsHoveredTileOverlappingTrap(SharedPointer<Tile> &hoveredTile, VectorSharedPointer<Unit> &allUnits);
bool shouldRenderUnitDueToVisibility(SharedPointer<Unit> &unit, SharedPointer<Player> &player);
void sMoveUnits(VectorSharedPointer<Unit> &allUnits, float deltaTime);
bool isMouseOverAnyUnit(VectorSharedPointer<Unit> &allUnits, Vector2 &worldMousePos);
void sUnitHover(VectorSharedPointer<Unit> &allUnits, SharedPointer<Unit> &hoveredUnit, Vector2 &worldMousePos);