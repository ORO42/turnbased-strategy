#pragma once

#include "components.h"
#include "geometryHelpers.h"
#include "probabilityHelpers.h"
#include "gridHelpers.h"
#include "generalUtils.h"
#include "raymath.h"
#include <cmath>
#include <unordered_map>

SharedPointer<Projectile> createProjectile(
    VectorSharedPointer<Projectile> &allProjectiles,
    Texture2D tex,
    Position originPos,
    Rectangle targetRect,
    std::string originUnitUuid,
    Teams team,
    float facingAngle,
    float speed,
    bool canCollideBeforeDestination,
    bool causesExplosion,
    int projectileAccuracyRadius,
    int projectileDamageRadius,
    float damage,
    float accuracy);

void sMoveProjectiles(VectorSharedPointer<Projectile> &allProjectiles, VectorSharedPointer<Unit> &allUnits, VectorSharedPointer<Obstacle> &allObstacles, float deltaTime);