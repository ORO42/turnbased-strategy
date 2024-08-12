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

void sSelectAbility(Unit *&selectedUnit, Ability *&selectedAbility, Vector2 &worldMousePos);
void sUseAbility(Unit *&selectedUnit, Ability *&selectedAbility, Tile *&hoveredTile, Unit *&hoveredUnit, Obstacle *&hoveredObstacle, Player &player, std::vector<Unit> &allUnits, std::vector<Obstacle> &allObstacles, Vector2 &worldMousePos, std::vector<GridSubdivision> &allGridSubdivisions, std::vector<Projectile> &allProjectiles, Texture2D projectileTex);
void sAutoDeselectAbility(Unit *&selectedUnit, Ability *&selectedAbility);