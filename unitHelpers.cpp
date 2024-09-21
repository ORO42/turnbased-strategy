#include "include/unitHelpers.h"

void createUnit(
    UnitType unitType,
    Position pos,
    SharedPointer<Player> &player,
    Teams team,
    VectorSharedPointer<Unit> &allUnits,
    Texture2D &unitTex)
{
    SharedPointer<Unit> newUnit = std::make_shared<Unit>();
    newUnit->name = "";
    newUnit->unitType = unitType;
    newUnit->uuid = generateUUID();
    newUnit->pos = pos;
    newUnit->facingAngle = -90;
    newUnit->visionTrapezoid = {};
    newUnit->health = 100.0f;
    newUnit->xp = 0.0f;
    newUnit->turnsAliveCt = 0;
    newUnit->team = team;
    newUnit->isVisibleToOppositeTeam = false;
    // newUnit.visionTrapezoid = (IsoscelesTrapezoid){(Position){}, (Position){}, (Position){}, (Position){}};

    SharedPointer<Ability> rifleAbility = std::make_shared<Ability>();
    rifleAbility->uuid = generateUUID();
    rifleAbility->abilityType = AbilityTypes::RIFLE;
    rifleAbility->displayTitle = "Rifle";
    rifleAbility->isPlayerAbility = false;
    rifleAbility->playerApCost = 0.0f;
    rifleAbility->playerXpRequirement = 0.0f;
    rifleAbility->unitApCost = 1.0f;
    rifleAbility->unitXpRequirement = 0.0f;
    rifleAbility->damage = 25.0f;
    rifleAbility->turnCooldown = 0;
    rifleAbility->lastTurnUsed = 0;
    rifleAbility->useQty = false;
    rifleAbility->qty = 0;
    rifleAbility->isQtyReplenishable = false;
    rifleAbility->reachRadius = 10;
    rifleAbility->accuracyRadius = 1; // NOTE 0 means just a single tile
    rifleAbility->placesUnit = UnitType::NA;

    SharedPointer<Ability> rotateAbility = std::make_shared<Ability>();
    rotateAbility->uuid = generateUUID();
    rotateAbility->abilityType = AbilityTypes::ROTATE;
    rotateAbility->displayTitle = "Rotate";
    rotateAbility->isPlayerAbility = false;
    rotateAbility->playerApCost = 0.0f;
    rotateAbility->playerXpRequirement = 0.0f;
    rotateAbility->unitApCost = 1.0f;
    rotateAbility->unitXpRequirement = 0.0f;
    rotateAbility->damage = 0.0f;
    rotateAbility->turnCooldown = 0;
    rotateAbility->lastTurnUsed = 0;
    rotateAbility->useQty = false;
    rotateAbility->qty = 0;
    rotateAbility->isQtyReplenishable = false;
    rotateAbility->reachRadius = -1;
    rotateAbility->accuracyRadius = -1; // NOTE 0 means just a single tile
    rotateAbility->placesUnit = UnitType::NA;

    SharedPointer<Ability> moveAbility = std::make_shared<Ability>();
    moveAbility->uuid = generateUUID();
    moveAbility->abilityType = AbilityTypes::MOVE;
    moveAbility->displayTitle = "Move";
    moveAbility->isPlayerAbility = false;
    moveAbility->playerApCost = 0.0f;
    moveAbility->playerXpRequirement = 0.0f;
    moveAbility->unitApCost = 1.0f;
    moveAbility->unitXpRequirement = 0.0f;
    moveAbility->damage = 0.0f;
    moveAbility->turnCooldown = 0;
    moveAbility->lastTurnUsed = 0;
    moveAbility->useQty = false;
    moveAbility->qty = 0;
    moveAbility->isQtyReplenishable = false;
    moveAbility->reachRadius = 5;
    moveAbility->accuracyRadius = -1; // NOTE 0 means just a single tile
    moveAbility->placesUnit = UnitType::NA;

    SharedPointer<Ability> placeRiflemanAbility = std::make_shared<Ability>();
    placeRiflemanAbility->uuid = generateUUID();
    placeRiflemanAbility->abilityType = AbilityTypes::PLACE_RIFLEMAN;
    placeRiflemanAbility->displayTitle = "Place Rifleman";
    placeRiflemanAbility->isPlayerAbility = false;
    placeRiflemanAbility->playerApCost = 0.0f;
    placeRiflemanAbility->playerXpRequirement = 0.0f;
    placeRiflemanAbility->unitApCost = 5.0f;
    placeRiflemanAbility->unitXpRequirement = 0.0f;
    placeRiflemanAbility->damage = 0.0f;
    placeRiflemanAbility->turnCooldown = 0;
    placeRiflemanAbility->lastTurnUsed = 0;
    placeRiflemanAbility->useQty = false;
    placeRiflemanAbility->qty = 0;
    placeRiflemanAbility->isQtyReplenishable = false;
    placeRiflemanAbility->reachRadius = 1;
    placeRiflemanAbility->accuracyRadius = -1; // NOTE 0 means just a single tile
    placeRiflemanAbility->placesUnit = UnitType::RIFLEMAN;

    if (unitType == UnitType::RIFLEMAN)
    {
        newUnit->attack = 25.0f;
        newUnit->accuracy = 100.0f;
        newUnit->maxAp = 100.0f;
        newUnit->ap = newUnit->maxAp;
        newUnit->abilities = VectorSharedPointer<Ability>{moveAbility, rotateAbility, rifleAbility, placeRiflemanAbility};

        newUnit->tex = unitTex;

        newUnit->viewDistance = 3;
        newUnit->viewWidth = 6; // NOTE: should be multiples of 2

        newUnit->movePoint = {-1.0f, -1.0f};
        newUnit->projectileStoppage = ProjectileStoppage::SOMETIMES;
    }

    allUnits.push_back(newUnit);
}

void sPositionVisionTrapezoids(VectorSharedPointer<Unit> &allUnits)
{
    for (auto &unit : allUnits)
    {
        // In the beginning, assume orientation is N
        IsoscelesTrapezoid trapezoid;
        trapezoid.originPos = getRectCenter({unit->pos.x, unit->pos.y, static_cast<float>(unit->tex.width), static_cast<float>(unit->tex.height)});
        trapezoid.p1 = {trapezoid.originPos.x + ((unit->viewWidth / 2) * 32.0f), trapezoid.originPos.y};
        trapezoid.p4 = {trapezoid.originPos.x - ((unit->viewWidth / 2) * 32.0f), trapezoid.originPos.y};
        trapezoid.p2 = {trapezoid.p1.x + (unit->viewDistance * 32.0f), trapezoid.p1.y - (unit->viewDistance * 32.0f)};
        trapezoid.p3 = {trapezoid.p4.x - (unit->viewDistance * 32.0f), trapezoid.p2.y};

        float northAngle = -90.0f;

        float angleDelta = angleDifference(northAngle, unit->facingAngle);

        rotateTrapezoid(trapezoid, angleDelta);
        unit->visionTrapezoid = trapezoid;
    }
}

void sVisibility(VectorSharedPointer<Unit> &allUnits, SharedPointer<Player> &player, VectorSharedPointer<Obstacle> &allObstacles)
{
    for (auto &unit : allUnits)
    {
        IsoscelesTrapezoid trap1 = unit->visionTrapezoid;

        for (auto &unit2 : allUnits)
        {
            if (unit2->uuid != unit->uuid)
            {
                if (unit->team != unit2->team)
                {
                    if (checkTrapRectOverlap(trap1, {unit2->pos.x, unit2->pos.y, static_cast<float>(unit2->tex.width), static_cast<float>(unit2->tex.height)}))
                    {
                        // cast rays to each unit, check if overlap any obstacles
                        std::vector<Line> cornerLines = getCornerToCornerLines({unit->pos.x, unit->pos.y, static_cast<float>(unit->tex.width), static_cast<float>(unit->tex.height)}, {unit2->pos.x, unit2->pos.y, static_cast<float>(unit2->tex.width), static_cast<float>(unit2->tex.height)});
                        int blockedCt = 0;

                        for (auto &line : cornerLines)
                        {
                            for (auto &obstacle : allObstacles)
                            {
                                // if all lines are intersecting an obstacle, the unit will not be visible
                                // TODO ignore origin and end obstacles
                                if (lineRectOverlap({obstacle->pos.x, obstacle->pos.y, static_cast<float>(obstacle->tex.width), static_cast<float>(obstacle->tex.height)}, line))
                                {
                                    blockedCt += 1;
                                    // need to break out and check next line
                                    break;
                                }
                            }
                        }
                        if (blockedCt < 16) // 16 lines are created from getCornerToCornerLines()
                        {
                            unit2->isVisibleToOppositeTeam = true;
                        }
                    }
                    else
                    {
                        unit2->isVisibleToOppositeTeam = false;
                    }
                }
            }
        }
    }
}

// NOTE it seems there is overlap on the top corners of the trapezoid even though the trap doesn't extend into that tile
void DEBUGsHoveredTileOverlappingTrap(SharedPointer<Tile> &hoveredTile, VectorSharedPointer<Unit> &allUnits)
{
    for (auto &unit : allUnits)
    {
        if (checkTrapRectOverlap(unit->visionTrapezoid, {hoveredTile->pos.x, hoveredTile->pos.y, 32.0f, 32.0f}))
        {
            std::cout << "OVERLAP" << std::endl;
        }
        else
        {
            std::cout << "NO OVERLAP" << std::endl;
        }
    }
}

bool shouldRenderUnitDueToVisibility(SharedPointer<Unit> &unit, SharedPointer<Player> &player)
{
    return (unit->team != player->team && unit->isVisibleToOppositeTeam) || unit->team == player->team;
}

// Function to move units smoothly
void sMoveUnits(VectorSharedPointer<Unit> &allUnits, float deltaTime)
{
    for (auto &unit : allUnits)
    {
        // TODO make generic move function
        if (unit->movePoint.x != -1.0f || unit->movePoint.y != -1.0)
        {
            // Calculate direction to move towards
            Vector2 direction = {unit->movePoint.x - unit->pos.x, unit->movePoint.y - unit->pos.y};
            float distance = Vector2Distance({unit->pos.x, unit->pos.y}, {unit->movePoint.x, unit->movePoint.y});

            if (distance > 0.0f)
            {
                // Normalize the direction
                direction.x /= distance;
                direction.y /= distance;

                // Move the unit
                float moveStep = (32.0f * 20) * deltaTime;
                if (moveStep >= distance)
                {
                    // If we can reach the move point this frame
                    unit->pos = unit->movePoint;
                    unit->movePoint = {-1.0f, -1.0f}; // Reset move point
                }
                else
                {
                    // Move towards the move point
                    unit->pos.x += direction.x * moveStep;
                    unit->pos.y += direction.y * moveStep;
                }
            }
        }
    }
}

bool isMouseOverAnyUnit(VectorSharedPointer<Unit> &allUnits, Vector2 &worldMousePos)
{
    for (auto &unit : allUnits)
    {
        if (CheckCollisionPointRec(worldMousePos, {unit->pos.x, unit->pos.y, static_cast<float>(unit->tex.width), static_cast<float>(unit->tex.height)}))
        {
            return true;
        }
    }
    return false;
}

void sUnitHover(VectorSharedPointer<Unit> &allUnits, SharedPointer<Unit> &hoveredUnit, Vector2 &worldMousePos)
{
    for (auto &unit : allUnits)
    {
        if (CheckCollisionPointRec(worldMousePos, {unit->pos.x, unit->pos.y, static_cast<float>(unit->tex.width), static_cast<float>(unit->tex.height)}))
        {
            hoveredUnit = unit;
        }
        // else
        // {
        //     hoveredUnit = nullptr;
        // }
    }
}

void sUnitOperations(
    VectorSharedPointer<Unit> &allUnits,
    SharedPointer<Player> &player,
    VectorSharedPointer<Obstacle> &allObstacles,
    SharedPointer<Tile> &hoveredTile,
    Vector2 &worldMousePos,
    SharedPointer<Unit> &hoveredUnit,
    float deltaTime)
{
    for (auto &unit : allUnits)
    {
        // sPositionVisionTrapezoids
        IsoscelesTrapezoid trapezoid;
        trapezoid.originPos = getRectCenter({unit->pos.x, unit->pos.y, static_cast<float>(unit->tex.width), static_cast<float>(unit->tex.height)});
        trapezoid.p1 = {trapezoid.originPos.x + ((unit->viewWidth / 2) * 32.0f), trapezoid.originPos.y};
        trapezoid.p4 = {trapezoid.originPos.x - ((unit->viewWidth / 2) * 32.0f), trapezoid.originPos.y};
        trapezoid.p2 = {trapezoid.p1.x + (unit->viewDistance * 32.0f), trapezoid.p1.y - (unit->viewDistance * 32.0f)};
        trapezoid.p3 = {trapezoid.p4.x - (unit->viewDistance * 32.0f), trapezoid.p2.y};

        float northAngle = -90.0f;
        float angleDelta = angleDifference(northAngle, unit->facingAngle);
        rotateTrapezoid(trapezoid, angleDelta);
        unit->visionTrapezoid = trapezoid;

        // sVisibility
        unit->isVisibleToOppositeTeam = false;
        for (auto &unit2 : allUnits)
        {
            if (unit2->uuid != unit->uuid && unit->team != unit2->team)
            {
                if (checkTrapRectOverlap(unit->visionTrapezoid, {unit2->pos.x, unit2->pos.y, static_cast<float>(unit2->tex.width), static_cast<float>(unit2->tex.height)}))
                {
                    std::vector<Line> cornerLines = getCornerToCornerLines(
                        {unit->pos.x, unit->pos.y, static_cast<float>(unit->tex.width), static_cast<float>(unit->tex.height)},
                        {unit2->pos.x, unit2->pos.y, static_cast<float>(unit2->tex.width), static_cast<float>(unit2->tex.height)});
                    int blockedCt = 0;

                    for (auto &line : cornerLines)
                    {
                        for (auto &obstacle : allObstacles)
                        {
                            if (lineRectOverlap({obstacle->pos.x, obstacle->pos.y, static_cast<float>(obstacle->tex.width), static_cast<float>(obstacle->tex.height)}, line))
                            {
                                blockedCt++;
                                break;
                            }
                        }
                    }
                    if (blockedCt < 16)
                    {
                        unit2->isVisibleToOppositeTeam = true;
                    }
                }
            }
        }

        // DEBUGsHoveredTileOverlappingTrap
        if (checkTrapRectOverlap(unit->visionTrapezoid, {hoveredTile->pos.x, hoveredTile->pos.y, 32.0f, 32.0f}))
        {
            std::cout << "OVERLAP" << std::endl;
        }
        else
        {
            std::cout << "NO OVERLAP" << std::endl;
        }

        // sMoveUnits
        if (unit->movePoint.x != -1.0f || unit->movePoint.y != -1.0)
        {
            Vector2 direction = {unit->movePoint.x - unit->pos.x, unit->movePoint.y - unit->pos.y};
            float distance = Vector2Distance({unit->pos.x, unit->pos.y}, {unit->movePoint.x, unit->movePoint.y});

            if (distance > 0.0f)
            {
                direction.x /= distance;
                direction.y /= distance;

                float moveStep = (32.0f * 20) * deltaTime;
                if (moveStep >= distance)
                {
                    unit->pos = unit->movePoint;
                    unit->movePoint = {-1.0f, -1.0f};
                }
                else
                {
                    unit->pos.x += direction.x * moveStep;
                    unit->pos.y += direction.y * moveStep;
                }
            }
        }

        // sUnitHover
        if (CheckCollisionPointRec(worldMousePos, {unit->pos.x, unit->pos.y, static_cast<float>(unit->tex.width), static_cast<float>(unit->tex.height)}))
        {
            hoveredUnit = unit;
        }
    }
}