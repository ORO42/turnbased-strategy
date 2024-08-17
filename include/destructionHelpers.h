#pragma once

#include "components.h"

void sDestroyUnits(VectorSharedPointer<Unit> &allUnits, SharedPointer<Unit> &selectedUnit, SharedPointer<Ability> &selectedAbility);
void sDestroyObstacles(VectorSharedPointer<Obstacle> &allObstacles);
void sDestroyProjectiles(VectorSharedPointer<Projectile> &allProjectiles);