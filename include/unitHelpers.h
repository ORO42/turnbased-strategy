#pragma once

#include <iostream>
#include <unordered_map>
#include "components.h"
#include "uuid.h"
#include "geometryHelpers.h"

void createUnit(
    UnitType unitType,
    Position pos,
    Player &player,
    std::vector<Unit> &allUnits,
    Texture2D &unitTex);
void sPositionVisionTrapezoids(std::vector<Unit> &allUnits);
void setVisionTrapezoidRotation(const PrepareTrapezoidRotationPayload &payload);