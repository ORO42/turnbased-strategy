#pragma once

#include <memory>
#include "components.h"
#include "iostream"

void setupRectangularGrid(int width, int height, VectorSharedPointer<Tile> &allTiles, Texture2D &tex);
void sDrawRectangularGrid(VectorSharedPointer<Tile> &allTiles);
void sTileHover(VectorSharedPointer<Tile> &allTiles, SharedPointer<Tile> &hoveredTile, Vector2 &worldMousePos);
int chebyshevTileDistance(const Vector2 pos1, const Vector2 pos2);
SharedPointer<Tile> getTileForPoint(const Vector2 pos, VectorSharedPointer<Tile> &allTiles);
Rectangle createRectAroundRect(const Rectangle innerRect, const int radius);
std::vector<Rectangle> deduceInnerRectangles(const Rectangle outerRect);
VectorSharedPointer<Tile> getTilesInRect(const Rectangle rect, const int radius, VectorSharedPointer<Tile> &allTiles);
void createGridSubdivisions(VectorSharedPointer<GridSubdivision> &allGridSubdivisions, const VectorSharedPointer<Tile> &allTiles, int width, int height, int numSegmentsX, int numSegmentsY);
VectorSharedPointer<Tile> getAllTilesSubdivRectCollision(VectorSharedPointer<GridSubdivision> &allGridSubdivisions, Rectangle rect);
SharedPointer<Tile> getTileForPointFromSubdivs(Position p, VectorSharedPointer<GridSubdivision> &allGridSubdivisions);
