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
void sDrawUI(Player &player, Unit *&selectedUnit, std::vector<Unit> &allUnits, Ability *&selectedAbility, Tile *&hoveredTile);
void sDrawSelectedUnitIndicator(Unit *&selectedUnit);
void sDrawHoveredTileIndicator(Tile *&hoveredTile, Ability *&selectedAbility);
void sDrawEffectRadius(Unit *&selectedUnit, Ability *&selectedAbility, Tile *&hoveredTile);
void sDrawAllTextures(std::vector<Unit> &allUnits, std::vector<Tile> &allTiles, std::vector<Projectile> &allProjectiles, Camera2D &camera, Player &player);
void sDrawRotationChevron(std::vector<Unit> &allUnits, Camera2D &camera, Texture2D &chevronTex, Player &player);
void sDrawVisionTrapezoids(std::vector<Unit> &allUnits, Camera2D &camera, Player &player);
void DEBUGsDrawAngleToHoveredTile(Unit *&selectedUnit, Tile *&hoveredTile);
void sDrawFacingAngleIndicator(std::vector<Unit> &allUnits, Player &player);
void sDrawDistanceIndicators(Ability *&selectedAbility, Unit *&selectedUnit, Tile *&hoveredTile, std::vector<Obstacle> &allObstacles, std::vector<Unit> &allUnits, Vector2 &worldMousePos);
void sDrawReachRadiusRect(Ability *&selectedAbility, Unit *&selectedUnit);