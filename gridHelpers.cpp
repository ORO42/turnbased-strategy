#include "include/gridHelpers.h"
#include "include/PerlinNoise.hpp"

void setupRectangularGrid(int width, int height, VectorSharedPointer<Tile> &allTiles, Texture2D &tex)
{
    const siv::PerlinNoise::seed_type seed = 123456u;
    const siv::PerlinNoise perlin{seed};

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            float tlx = x * 32.0f;
            float tly = y * 32.0f;
            const double frequency = 0.1; // 0.01 - 0.64
            const double noise = perlin.octave2D_01((x * frequency), (y * frequency), 8);
            SharedPointer<Tile> newTile = std::make_shared<Tile>(Tile{tlx, tly, tex, 0.0, TerrainType::NORMAL, floor(noise * 10)});
            allTiles.push_back(newTile);
        }
    }

    // draw high ground
    for (auto &tile : allTiles)
    {
    }

    // draw low ground
    // draw villages
    // draw trees
    // draw rocks
}

void sDrawRectangularGrid(VectorSharedPointer<Tile> &allTiles)
{
    for (auto &tile : allTiles)
    {
        DrawTexture(tile->tex, tile->pos.x, tile->pos.y, WHITE);
        // TODO for performance, only draw the visual grid for tiles that are in the viewport
        // DrawRectangleLinesEx((Rectangle){tile.pos.x, tile.pos.y, 32.0f, 32.0f}, 0.5f, GRAY);
    };
}

void sTileHover(VectorSharedPointer<Tile> &allTiles, SharedPointer<Tile> &hoveredTile, Vector2 &worldMousePos)
{
    for (auto &tile : allTiles)
    {
        if (CheckCollisionPointRec(worldMousePos, (Rectangle){tile->pos.x, tile->pos.y, 32.0f, 32.0f}))
        {
            hoveredTile = tile;
        }
        // else
        // {
        //     hoveredTile = nullptr;
        // }
    };
}

int chebyshevTileDistance(const Vector2 pos1, const Vector2 pos2)
{
    // Calculate the tile distance between two tiles using Chebyshev distance
    int dx = std::abs(pos1.x - pos2.x);
    int dy = std::abs(pos1.y - pos2.y);

    // Calculate the tile distance by dividing the maximum of dx and dy by the tile size
    int tileDistance = std::max(dx, dy) / 32;

    // Return the tile distance
    return tileDistance;
}

SharedPointer<Tile> getTileForPoint(const Vector2 pos, VectorSharedPointer<Tile> &allTiles)
{
    for (auto &tile : allTiles)
    {
        if (CheckCollisionPointRec(pos, {tile->pos.x, tile->pos.y, 32.0f, 32.0f}))
        {
            return tile;
        }
    }
    return {};
}

Rectangle createRectAroundRect(const Rectangle innerRect, const int radius)
{
    Vector2 centerPoint = {innerRect.x + innerRect.width / 2,
                           innerRect.y + innerRect.height / 2};

    DrawCircle(centerPoint.x, centerPoint.y, 3.0f, ORANGE); // TODO this is drawing two extra points under the tile map

    // Calculate the position and size of the outer rectangle
    float outerX = innerRect.x - radius * 32.0f;
    float outerY = innerRect.y - radius * 32.0f;
    float outerWidth = innerRect.width + 2 * radius * 32.0f;
    float outerHeight = innerRect.height + 2 * radius * 32.0f;

    // Create the outer rectangle
    Rectangle outerRect = {outerX, outerY, outerWidth, outerHeight};

    // DrawRectangleLines(outerRect.x, outerRect.y, outerRect.width, outerRect.height, GREEN);
    return outerRect;
}

std::vector<Rectangle> deduceInnerRectangles(const Rectangle outerRect)
{
    std::vector<Rectangle> rectangles;

    // Define the size of the small rectangles
    float rectWidth = 32.0f;
    float rectHeight = 32.0f;

    // Calculate the number of rectangles that fit along the width and height
    int numRectsX = static_cast<int>(outerRect.width / rectWidth);
    int numRectsY = static_cast<int>(outerRect.height / rectHeight);

    // Iterate over the area of the outer rectangle
    for (int i = 0; i < numRectsX; i++)
    {
        for (int j = 0; j < numRectsY; j++)
        {
            float x = outerRect.x + i * rectWidth;
            float y = outerRect.y + j * rectHeight;

            Rectangle smallRect = {x, y, rectWidth, rectHeight};
            rectangles.push_back(smallRect);
        }
    }

    return rectangles;
}

VectorSharedPointer<Tile> getTilesInRect(const Rectangle rect, const int radius, VectorSharedPointer<Tile> &allTiles)
{
    VectorSharedPointer<Tile> overlappingTiles;

    // get all the tiles overlapping the rect
    for (auto &tile : allTiles)
    {
        if (CheckCollisionRecs(rect, {tile->pos.x, tile->pos.y, 32.0f, 32.0f}))
        {
            overlappingTiles.push_back(tile);
        }
    }

    return overlappingTiles;
}

// std::vector<Tile> getTilesInRect(const Vector2 topLeftPos, const float &width, const float &height, const int &radius, std::vector<Tile> &allTiles)
// {
//     std::vector<Tile> overlappingTiles;

//     // construct rectangle with center point as provided pos
//     Vector2 centerPoint = {topLeftPos.x + width / 2,
//                            topLeftPos.y + height / 2};

//     // get all the tiles overlapping the rect
//     for (auto &tile : allTiles)
//     {
//         if (CheckCollisionRecs({centerPoint.x - (width / 2) * radius, centerPoint.x - (height / 2) * radius, width * radius, height * radius}, {tile.pos.x, tile.pos.y, 32.0f, 32.0f}))
//         {
//             overlappingTiles.push_back(tile);
//         }
//     }
//     DrawRectangleLines(centerPoint.x - (width / 2) * radius, centerPoint.x - (height / 2) * radius, width * radius, height * radius, GREEN);

//     return overlappingTiles;
// }

void createGridSubdivisions(VectorSharedPointer<GridSubdivision> &allGridSubdivisions, const VectorSharedPointer<Tile> &allTiles, int width, int height, int numSegmentsX, int numSegmentsY)
{
    // Calculate the size of each segment
    float segmentWidth = static_cast<float>(width) / numSegmentsX;
    float segmentHeight = static_cast<float>(height) / numSegmentsY;

    // Iterate over each segment
    for (float y = 0; y < height; y += segmentHeight)
    {
        for (float x = 0; x < width; x += segmentWidth)
        {
            // Create a new GridSubdivision for the segment
            SharedPointer<GridSubdivision> subdivision = std::make_shared<GridSubdivision>();
            subdivision->pos = {x, y};
            subdivision->w = segmentWidth;
            subdivision->h = segmentHeight;

            // Iterate over each tile and check if it belongs to this segment
            for (auto &tile : allTiles)
            {
                if (tile->pos.x >= x && tile->pos.x < x + segmentWidth &&
                    tile->pos.y >= y && tile->pos.y < y + segmentHeight)
                {
                    subdivision->tilesInSubdivision.push_back(tile);
                }
            }

            // Add the GridSubdivision to the vector
            allGridSubdivisions.push_back(subdivision);
        }
    }
}

VectorSharedPointer<Tile> getAllTilesSubdivRectCollision(VectorSharedPointer<GridSubdivision> &allGridSubdivisions, Rectangle rect)
{
    VectorSharedPointer<Tile> allTiles;
    for (auto &subdiv : allGridSubdivisions)
    {
        if (CheckCollisionRecs({subdiv->pos.x, subdiv->pos.y, subdiv->w, subdiv->h}, rect))
        {
            for (auto &tile : subdiv->tilesInSubdivision)
            {
                allTiles.push_back(tile);
            }
        }
    }
    return allTiles;
}

SharedPointer<Tile> getTileForPointFromSubdivs(Position p, VectorSharedPointer<GridSubdivision> &allGridSubdivisions)
{
    SharedPointer<Tile> foundTile = {};

    for (auto gSubdiv : allGridSubdivisions)
    {
        if (CheckCollisionPointRec({p.x, p.y}, {gSubdiv->pos.x, gSubdiv->pos.y, gSubdiv->w, gSubdiv->h}))
        {
            for (auto tile : gSubdiv->tilesInSubdivision)
            {
                if (CheckCollisionPointRec({p.x, p.y}, {tile->pos.x, tile->pos.y, static_cast<float>(tile->tex.width), static_cast<float>(tile->tex.height)}))
                {
                    foundTile = tile;
                }
            }
        }
    }

    return foundTile;
}