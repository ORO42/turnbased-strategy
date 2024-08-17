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
    std::vector<Projectile> &allProjectiles,
    Texture2D tex,
    Position &originPos,
    // Position &destinationPos,
    Rectangle &targetRect,
    std::string &originUnitUuid,
    Teams &team,
    float &facingAngle,
    float speed,
    bool canCollideBeforeDestination,
    bool causesExplosion,
    int projectileEffectRadius,
    float damage,
    float &accuracy);

void sMoveProjectiles(std::vector<Projectile> &allProjectiles, std::vector<Unit> &allUnits, std::vector<Obstacle> &allObstacles, float &deltaTime, Rectangle &debug_endRect);
void sProjectileDamage(std::vector<Projectile> &allProjectiles, std::vector<Unit> &allUnits, std::vector<Obstacle> &allObstacles);