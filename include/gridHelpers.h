#pragma once

#include "components.h"
#include "iostream"

void setupRectangularGrid(int width, int height, std::vector<Tile> &allTiles, Texture2D &tex);
void sDrawRectangularGrid(std::vector<Tile> &allTiles);
void sTileHover(std::vector<Tile> &allTiles, Tile *&hoveredTile, Vector2 &worldMousePos);
int chebyshevTileDistance(const Vector2 &pos1, const Vector2 &pos2);
Tile getTileForPoint(const Vector2 &pos, std::vector<Tile> &allTiles);
Rectangle createRectAroundRect(const Rectangle &innerRect, const int &radius);
std::vector<Tile> getTilesInRect(const Rectangle &rect, const int &radius, std::vector<Tile> &allTiles);
void createGridSubdivisions(std::vector<GridSubdivision> &allGridSubdivisions, const std::vector<Tile> &allTiles, int width, int height, int numSegmentsX, int numSegmentsY);
std::vector<Tile> getAllTilesSubdivRectCollision(std::vector<GridSubdivision> &allGridSubdivisions, Rectangle rect);