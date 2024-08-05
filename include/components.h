#pragma once

// CONCEPT: to create a unit, the building must have enough AP to cover the cost, supply trucks transport AP to buildings and units
// how supply works: certain buildings generate supplies per turn. Supply trucks move into range of a supply generating building and subtract from its supplies into its supplies. The same process happens between trucks and people

#include "raylib.h"
#include <vector>
#include <string>

enum struct AbilityTypes
{
    ATTACK,
    MOVE,
    ROTATE,
    NA,
};

enum struct UnitType
{
    // person
    RIFLEMAN,
    NA,
};

enum struct ObstacleType
{
    WALL,
};

enum struct TerrainType
{
    NORMAL,
    HIGHGROUND,
    LOWGROUND,
    MUD,
    FOREST,
    NA,
};

enum struct Teams
{
    REDTEAM,
    BLUETEAM,
    NA,
};

enum struct Stance
{
    STANDING,
    PRONE,
    NA
};

enum struct CardinalDirection
{
    N,
    S,
    E,
    W,
    NE,
    NW,
    SE,
    SW
};

struct Player
{
    float ap;
    float xp;
    Teams team = Teams::BLUETEAM;
    // float food;
    // float oil;
    // float ammo;
};

// struct CostSheet // used for specifying the cost of items in the costs unordered map
// {
//     float playerApCost;
//     float unitApCost;
//     float oilCost;
//     // etc...
// };

struct Position
{
    float x;
    float y;
};

struct Line
{
    int startX;
    int startY;
    int endX;
    int endY;
};

struct IsoscelesTrapezoid
{
    Position originPos; // center of base
    Position p1;
    Position p2;
    Position p3;
    Position p4;
};

struct Ability
{
    std::string uuid;
    AbilityTypes abilityType;
    std::string displayTitle;
    float playerApCost;
    float playerXpRequirement;
    float unitApCost;
    float unitXpRequirement;
    float damage;
    int turnCooldown;
    int lastTurnUsed;
    bool useQty;
    int qty;
    bool isQtyReplenishable;
    int reachRadius;
    int effectRadius;
    bool canAffectUnit;
    bool canAffectTile;
    bool canAffectObstacle;
    bool canAffectSelf;
    bool onlyAffectsSelf;
    UnitType placesUnit;
    // bool selected = false;
};

struct Unit
{
    std::string name;
    UnitType unitType;
    std::string uuid;
    Position pos;
    CardinalDirection facingDirection;
    IsoscelesTrapezoid visionTrapezoid;
    float health;
    float attack;
    float accuracy;
    float maxAp;
    float ap;
    float xp;
    int viewDistance;
    int viewWidth;

    std::vector<Ability> abilities;

    int turnsAliveCt;
    Texture2D tex;
    Position movePoint = {-1.0f, -1.0f};
    Teams team;
    bool isVisibleToOppositeTeam;
    // IsoscelesTrapezoid visionTrapezoid;
};

struct Tile
{
    Position pos;
    Texture2D tex;
    float moveCost;
    TerrainType terrainType;
    double perlinNoise;
};

struct Obstacle
{
    ObstacleType obstacleType;
    Position pos;
    Texture2D tex;
    bool isNavigable;
    float cover; // counts against spot chance
    bool unitCanBeUnder;
    std::vector<Stance> visibleInStance; // if a unit behind this obstacle and is in any of these stances, the unit can be seen
    float health = 0.0;
};

struct GridSubdivision
{
    Position pos;
    float w;
    float h;
    std::vector<Tile> tilesInSubdivision;
    // after creating all tiles, create quads
    // for each quad, push_back a reference to all tiles intersecting the quad into the quad
    // to check tile collisions externally, get all the quads that are colliding, and iterate each quad's contained tiles
};

struct Projectile
{
    Texture2D tex;
    Position originPos;
    Position destinationPos;
    int damage;
    // float maxDistanceFromOrigin;
    float speed;
};