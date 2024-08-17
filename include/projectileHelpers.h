#pragma once

#include "components.h"
#include "geometryHelpers.h"
#include "probabilityHelpers.h"
#include "gridHelpers.h"
#include "generalUtils.h"
#include "raymath.h"
#include <cmath>
#include <unordered_set>

void createProjectile(
    VectorSharedPointer<Projectile> &allProjectiles,
    Texture2D tex,
    Position originPos,
    // Position &destinationPos,
    Rectangle targetRect,
    std::string originUnitUuid,
    Teams team,
    float facingAngle,
    float speed,
    bool canCollideBeforeDestination,
    bool causesExplosion,
    int projectileEffectRadius,
    float damage,
    float accuracy);
;
void sMoveProjectiles(VectorSharedPointer<Projectile> &allProjectiles, VectorSharedPointer<Unit> &allUnits, VectorSharedPointer<Obstacle> &allObstacles, float deltaTime);
void sProjectileDamage(VectorSharedPointer<Projectile> &allProjectiles, VectorSharedPointer<Unit> &allUnits, VectorSharedPointer<Obstacle> &allObstacles);