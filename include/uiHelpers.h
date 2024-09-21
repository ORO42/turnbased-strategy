#pragma once

#include "components.h"
#include "sig.h"
#include "gridHelpers.h"
#include "raylib.h"
#include "selectionHelpers.h"
#include "geometryHelpers.h"
#include "raymath.h"
#include "unitHelpers.h"
#include "obstacleHelpers.h"

bool isRectangleInViewport(const Rectangle &rect, const Camera2D &camera);
void sDrawUI(SharedPointer<Player> &player, SharedPointer<Unit> &selectedUnit, VectorSharedPointer<Unit> &allUnits, SharedPointer<Ability> &selectedAbility, SharedPointer<Tile> &hoveredTile);
void sDrawSelectedUnitIndicator(SharedPointer<Unit> &selectedUnit);
void sDrawHoveredTileIndicator(SharedPointer<Tile> &hoveredTile, SharedPointer<Ability> &selectedAbility);
void sDrawaccuracyRadius(SharedPointer<Unit> &selectedUnit, SharedPointer<Ability> &selectedAbility, SharedPointer<Tile> &hoveredTile);
void sDrawAllTextures(VectorSharedPointer<Unit> &allUnits, VectorSharedPointer<Tile> &allTiles, VectorSharedPointer<Obstacle> &allObstacles, VectorSharedPointer<Projectile> &allProjectiles, Camera2D &camera, SharedPointer<Player> &player);
void sDrawRotationChevron(VectorSharedPointer<Unit> &allUnits, Camera2D &camera, Texture2D &chevronTex, SharedPointer<Player> &player);
void sDrawVisionTrapezoids(VectorSharedPointer<Unit> &allUnits, Camera2D &camera, SharedPointer<Player> &player);
void DEBUGsDrawAngleToHoveredTile(SharedPointer<Unit> &selectedUnit, SharedPointer<Tile> &hoveredTile);
void sDrawFacingAngleIndicator(VectorSharedPointer<Unit> &allUnits, SharedPointer<Player> &player);
void sDrawDistanceIndicators(SharedPointer<Ability> &selectedAbility, SharedPointer<Unit> &selectedUnit, SharedPointer<Tile> &hoveredTile, VectorSharedPointer<Obstacle> &allObstacles, VectorSharedPointer<Unit> &allUnits, Vector2 &worldMousePos);
void sDrawReachRadiusRect(SharedPointer<Ability> &selectedAbility, SharedPointer<Unit> &selectedUnit);
void sDrawHealthValues(VectorSharedPointer<Unit> &allUnits, VectorSharedPointer<Obstacle> &allObstacles);
void sDrawUnitToPlace(SharedPointer<Ability> &selectedAbility, UnitType &unitToPlaceType, Texture2D &unitToPlaceTex, Vector2 &worldMousePos);