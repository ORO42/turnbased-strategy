#pragma once

#include "components.h"

void sDestroyUnits(std::vector<Unit> &allUnits, Unit *&selectedUnit, Ability *&selectedAbility);
void sDestroyObstacles(std::vector<Obstacle> &allObstacles);
void sDestroyProjectiles(std::vector<Projectile> &allProjectiles);