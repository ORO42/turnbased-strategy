#pragma once

#include "components.h"
#include "geometryHelpers.h"
#include "probabilityHelpers.h"
#include "gridHelpers.h"
#include "raymath.h"
#include <cmath>

void createProjectile(
    std::vector<Projectile> &allProjectiles,
    Texture2D tex,
    Position &originPos,
    Position &destinationPos,
    std::string &originUnitUuid,
    Teams &team,
    bool isAerial,
    float &facingAngle,
    float speed,
    bool canCollideBeforeDestination,
    bool causesExplosion,
    int &effectRadius,
    float damage,
    float &accuracy);

void sMoveProjectiles(std::vector<Projectile> &allProjectiles, std::vector<Unit> &allUnits, std::vector<Obstacle> &allObstacles, float &deltaTime);