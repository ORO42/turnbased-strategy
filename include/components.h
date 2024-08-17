#pragma once

// CONCEPT: to create a unit, the building must have enough AP to cover the cost, supply trucks transport AP to buildings and units
// how supply works: certain buildings generate supplies per turn. Supply trucks move into range of a supply generating building and subtract from its supplies into its supplies. The same process happens between trucks and people

#include "raylib.h"
#include <vector>
#include <string>
#include <memory>

template <typename T>
using SharedPointer = std::shared_ptr<T>;

template <typename T>
using VectorSharedPointer = std::vector<std::shared_ptr<T>>;

enum struct AbilityTypes
{
    RIFLE,
    MOVE,
    ROTATE,
    MORTAR_VOLLEY,
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

enum struct ProjectileStoppage
{
    ALWAYS,
    SOMETIMES,
    NEVER,
};

struct Player
{
    float ap;
    float xp;
    Teams team;
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
    float startX;
    float startY;
    float endX;
    float endY;
};

struct PointsRect
{
    Position tl;
    Position tr;
    Position br;
    Position bl;
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
    bool isPlayerAbility;
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
    UnitType placesUnit;
};

struct Unit
{
    std::string name;
    UnitType unitType;
    std::string uuid;
    Position pos;
    float facingAngle;
    IsoscelesTrapezoid visionTrapezoid;
    float health;
    float attack;
    float accuracy;
    float maxAp;
    float ap;
    float xp;
    int viewDistance;
    int viewWidth;

    VectorSharedPointer<Ability> abilities;

    int turnsAliveCt;
    Texture2D tex;
    Position movePoint = {-1.0f, -1.0f};
    Teams team;
    bool isVisibleToOppositeTeam;
    ProjectileStoppage projectileStoppage;
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
    std::string uuid;
    Position pos;
    Texture2D tex;
    bool isNavigable;
    float cover; // counts against spot chance
    bool unitCanBeUnder;
    bool visibleInProne;    // units in this stance behind the obstacle will not be visible to other units between the obstacle and the unit
    bool visibleInStanding; // units in this stance behind the obstacle will not be visible to other units between the obstacle and the unit
    float health = 0.0;
    ProjectileStoppage projectileStoppage;
};

struct GridSubdivision
{
    Position pos;
    float w;
    float h;
    VectorSharedPointer<Tile> tilesInSubdivision; // TODO this needs to be a vector of pointers to references
    // after creating all tiles, create quads
    // for each quad, push_back a reference to all tiles intersecting the quad into the quad
    // to check tile collisions externally, get all the quads that are colliding, and iterate each quad's contained tiles
};

struct Projectile
{
    Texture2D tex;
    Position originPos; // TODO maybe these need to be rects
    // Position destinationPos = {-1.0, -1.0}; // TODO maybe these need to be rects
    Rectangle targetRect;
    Position currentPos;
    float facingAngle;
    std::string originUnitUuid;
    Teams team;
    float speed;
    bool canCollideBeforeDestination; // determined by an initial accuracy roll, or manually when creating the projectile
    bool shouldDestroy;               // if this projectile needs to be removed
    bool causesExplosion;
    int effectRadius;
    float damage;
    float accuracy; // the chance this projectile causes damage to each unit/obstacle
    std::vector<std::string> idsToIgnoreInFlight;
    // Position endPos;                             // the point at which the projectile needs to be destroyed (used to get overlapping obstacles, units, tiles)
    Rectangle endRect = {-1.0f, -1.0f};
};