#pragma once

#include "components.h"
#include "selectionHelpers.h"
#include "gridHelpers.h"
#include "geometryHelpers.h"
#include "raymath.h"
#include "sig.h"
#include "obstacleHelpers.h"
#include "probabilityHelpers.h"
#include "projectileHelpers.h"
#include <algorithm>
#include <random>

void sSelectAbility(SharedPointer<Unit> &selectedUnit, SharedPointer<Ability> &selectedAbility, Vector2 &worldMousePos);
void sUseAbility(SharedPointer<Unit> &selectedUnit, SharedPointer<Ability> &selectedAbility, SharedPointer<Tile> &hoveredTile, SharedPointer<Unit> &hoveredUnit, SharedPointer<Obstacle> &hoveredObstacle, SharedPointer<Player> &player, VectorSharedPointer<Unit> &allUnits, VectorSharedPointer<Obstacle> &allObstacles, Vector2 &worldMousePos, VectorSharedPointer<GridSubdivision> &allGridSubdivisions, VectorSharedPointer<Projectile> &allProjectiles, Texture2D projectileTex);
void sAutoDeselectAbility(SharedPointer<Unit> &selectedUnit, SharedPointer<Ability> &selectedAbility);
