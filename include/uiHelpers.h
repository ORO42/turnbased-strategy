#pragma once

#include "components.h"
#include "sig.h"
#include "gridHelpers.h"
#include "raylib.h"
#include "selectionHelpers.h"

bool isRectangleInViewport(const Rectangle &rect, const Camera2D &camera);
void sDrawUI(Player &player, Unit *&selectedUnit, std::vector<Unit> &allUnits, Ability *&selectedAbility, Tile *&hoveredTile);
void sDrawSelectedUnitIndicator(Unit *&selectedUnit);
void sDrawHoveredTileIndicator(Tile *&hoveredTile, Ability *&selectedAbility);
void sDrawEffectRadius(Unit *&selectedUnit, Ability *&selectedAbility, Tile *&hoveredTile);
void sDrawAllTextures(std::vector<Unit> &allUnits, std::vector<Tile> &allTiles, Camera2D &camera);
void sDrawRotationChevron(std::vector<Unit> &allUnits, Camera2D &camera, Texture2D &chevronTex);
void sDrawVisionTrapezoids(std::vector<Unit> &allUnits, Camera2D &camera);