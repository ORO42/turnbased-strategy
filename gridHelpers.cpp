#include "include/gridHelpers.h"
#include "include/PerlinNoise.hpp"

void setupRectangularGrid(int width, int height, std::vector<Tile> &allTiles, Texture2D &tex)
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
            Tile newTile{tlx, tly, tex, 0.0, TerrainType::NORMAL, floor(noise * 10)};
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

void sDrawRectangularGrid(std::vector<Tile> &allTiles)
{
    for (auto &tile : allTiles)
    {
        DrawTexture(tile.tex, tile.pos.x, tile.pos.y, WHITE);
        // TODO for performance, only draw the visual grid for tiles that are in the viewport
        // DrawRectangleLinesEx((Rectangle){tile.pos.x, tile.pos.y, 32.0f, 32.0f}, 0.5f, GRAY);
    };
}

void sTileHover(std::vector<Tile> &allTiles, Tile *&hoveredTile, Vector2 &worldMousePos)
{
    for (auto &tile : allTiles)
    {
        if (CheckCollisionPointRec(worldMousePos, (Rectangle){tile.pos.x, tile.pos.y, 32.0f, 32.0f}))
        {
            hoveredTile = &tile;
        }
        // else
        // {
        //     hoveredTile = nullptr;
        // }
    };
}

int chebyshevTileDistance(const Vector2 &pos1, const Vector2 &pos2)
{
    // Calculate the tile distance between two tiles using Chebyshev distance
    int dx = std::abs(pos1.x - pos2.x);
    int dy = std::abs(pos1.y - pos2.y);

    // Calculate the tile distance by dividing the maximum of dx and dy by the tile size
    int tileDistance = std::max(dx, dy) / 32;

    // Return the tile distance
    return tileDistance;
}

Tile getTileForPoint(const Vector2 &pos, std::vector<Tile> &allTiles)
{
    for (auto &tile : allTiles)
    {
        if (CheckCollisionPointRec(pos, {tile.pos.x, tile.pos.y, 32.0f, 32.0f}))
        {
            return tile;
        }
        else
        {
            return {};
        }
    }
}

Rectangle createRectAroundRect(const Rectangle &innerRect, const int &radius)
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

std::vector<Tile> getTilesInRect(const Rectangle &rect, const int &radius, std::vector<Tile> &allTiles)
{
    std::vector<Tile> overlappingTiles;

    // get all the tiles overlapping the rect
    for (auto &tile : allTiles)
    {
        if (CheckCollisionRecs(rect, {tile.pos.x, tile.pos.y, 32.0f, 32.0f}))
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

void createGridSubdivisions(std::vector<GridSubdivision> &allGridSubdivisions, const std::vector<Tile> &allTiles, int width, int height, int numSegmentsX, int numSegmentsY)
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
            GridSubdivision subdivision;
            subdivision.pos = {x, y};
            subdivision.w = segmentWidth;
            subdivision.h = segmentHeight;

            // Iterate over each tile and check if it belongs to this segment
            for (const Tile &tile : allTiles)
            {
                if (tile.pos.x >= x && tile.pos.x < x + segmentWidth &&
                    tile.pos.y >= y && tile.pos.y < y + segmentHeight)
                {
                    subdivision.tilesInSubdivision.push_back(tile);
                }
            }

            // Add the GridSubdivision to the vector
            allGridSubdivisions.push_back(subdivision);
        }
    }
}

std::vector<Tile> getAllTilesSubdivRectCollision(std::vector<GridSubdivision> &allGridSubdivisions, Rectangle rect)
{
    std::vector<Tile> allTiles;
    for (auto &subdiv : allGridSubdivisions)
    {
        if (CheckCollisionRecs({subdiv.pos.x, subdiv.pos.y, subdiv.w, subdiv.h}, rect))
        {
            for (auto &tile : subdiv.tilesInSubdivision)
            {
                allTiles.push_back(tile);
            }
        }
    }
    return allTiles;
}

Tile getTileForPointFromSubdivs(Position &p, std::vector<GridSubdivision> &allGridSubdivisions)
{
    Tile foundTile = {};

    for (auto gSubdiv : allGridSubdivisions)
    {
        if (CheckCollisionPointRec({p.x, p.y}, {gSubdiv.pos.x, gSubdiv.pos.y, gSubdiv.w, gSubdiv.h}))
        {
            for (auto tile : gSubdiv.tilesInSubdivision)
            {
                if (CheckCollisionPointRec({p.x, p.y}, {tile.pos.x, tile.pos.y, static_cast<float>(tile.tex.width), static_cast<float>(tile.tex.height)}))
                {
                    foundTile = tile;
                }
            }
        }
    }

    return foundTile;
}