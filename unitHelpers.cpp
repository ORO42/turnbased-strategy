#include "include/unitHelpers.h"

void createUnit(
    UnitType unitType,
    Position pos,
    Player &player,
    Teams team,
    std::vector<Unit> &allUnits,
    Texture2D &unitTex)
{
    Unit newUnit;
    newUnit.name = "";
    newUnit.unitType = unitType;
    newUnit.uuid = generateUUID();
    newUnit.pos = pos;
    newUnit.facingAngle = -90;
    newUnit.visionTrapezoid = {};
    newUnit.health = 100.0f;
    newUnit.xp = 0.0f;
    newUnit.turnsAliveCt = 0;
    newUnit.team = team;
    newUnit.isVisibleToOppositeTeam = false;
    // newUnit.visionTrapezoid = (IsoscelesTrapezoid){(Position){}, (Position){}, (Position){}, (Position){}};

    Ability attackAbility;
    attackAbility.uuid = generateUUID();
    attackAbility.abilityType = AbilityTypes::ATTACK;
    attackAbility.displayTitle = "Attack";
    attackAbility.playerApCost = 0.0f;
    attackAbility.playerXpRequirement = 0.0f;
    attackAbility.unitApCost = 1.0f;
    attackAbility.unitXpRequirement = 0.0f;
    attackAbility.damage = 25.0f;
    attackAbility.turnCooldown = 0;
    attackAbility.lastTurnUsed = 0;
    attackAbility.useQty = false;
    attackAbility.qty = 0;
    attackAbility.isQtyReplenishable = false;
    attackAbility.reachRadius = 10;
    attackAbility.effectRadius = 0; // NOTE 0 means just a single tile
    attackAbility.canAffectUnit = true;
    attackAbility.canAffectObstacle = false;
    attackAbility.canAffectTile = false;
    attackAbility.canAffectSelf = false;
    attackAbility.onlyAffectsSelf = false;
    attackAbility.placesUnit = UnitType::NA;

    Ability rotateAbility;
    rotateAbility.uuid = generateUUID();
    rotateAbility.abilityType = AbilityTypes::ROTATE;
    rotateAbility.displayTitle = "Rotate";
    rotateAbility.playerApCost = 0.0f;
    rotateAbility.playerXpRequirement = 0.0f;
    rotateAbility.unitApCost = 1.0f;
    rotateAbility.unitXpRequirement = 0.0f;
    rotateAbility.damage = 0.0f;
    rotateAbility.turnCooldown = 0;
    rotateAbility.lastTurnUsed = 0;
    rotateAbility.useQty = false;
    rotateAbility.qty = 0;
    rotateAbility.isQtyReplenishable = false;
    rotateAbility.reachRadius = 99999;
    rotateAbility.effectRadius = 0; // NOTE 0 means just a single tile
    rotateAbility.canAffectUnit = false;
    rotateAbility.canAffectObstacle = false;
    rotateAbility.canAffectTile = false;
    rotateAbility.canAffectSelf = false;
    rotateAbility.onlyAffectsSelf = false;
    rotateAbility.placesUnit = UnitType::NA;

    Ability moveAbility;
    moveAbility.uuid = generateUUID();
    moveAbility.abilityType = AbilityTypes::MOVE;
    moveAbility.displayTitle = "Move";
    moveAbility.playerApCost = 0.0f;
    moveAbility.playerXpRequirement = 0.0f;
    moveAbility.unitApCost = 1.0f;
    moveAbility.unitXpRequirement = 0.0f;
    moveAbility.damage = 0.0f;
    moveAbility.turnCooldown = 0;
    moveAbility.lastTurnUsed = 0;
    moveAbility.useQty = false;
    moveAbility.qty = 0;
    moveAbility.isQtyReplenishable = false;
    moveAbility.reachRadius = 5;
    moveAbility.effectRadius = 0; // NOTE 0 means just a single tile
    moveAbility.canAffectUnit = false;
    moveAbility.canAffectObstacle = false;
    moveAbility.canAffectTile = false;
    moveAbility.canAffectSelf = false;
    moveAbility.onlyAffectsSelf = false;
    moveAbility.placesUnit = UnitType::NA;

    if (unitType == UnitType::RIFLEMAN)
    {
        newUnit.attack = 25.0f;
        newUnit.accuracy = 1.0f;
        newUnit.maxAp = 10.0f;
        newUnit.ap = 10.0f;
        newUnit.abilities = std::vector<Ability>{moveAbility, rotateAbility, attackAbility};

        newUnit.tex = unitTex;

        newUnit.viewDistance = 3;
        newUnit.viewWidth = 6; // NOTE: should be multiples of 2

        newUnit.movePoint = {-1.0f, -1.0f};
    }

    allUnits.push_back(newUnit);
}

void sPositionVisionTrapezoids(std::vector<Unit> &allUnits)
{
    for (auto &unit : allUnits)
    {
        // In the beginning, assume orientation is N
        IsoscelesTrapezoid trapezoid;
        trapezoid.originPos = getRectCenter({unit.pos.x, unit.pos.y, static_cast<float>(unit.tex.width), static_cast<float>(unit.tex.height)});
        trapezoid.p1 = {trapezoid.originPos.x + ((unit.viewWidth / 2) * 32.0f), trapezoid.originPos.y};
        trapezoid.p4 = {trapezoid.originPos.x - ((unit.viewWidth / 2) * 32.0f), trapezoid.originPos.y};
        trapezoid.p2 = {trapezoid.p1.x + (unit.viewDistance * 32.0f), trapezoid.p1.y - (unit.viewDistance * 32.0f)};
        trapezoid.p3 = {trapezoid.p4.x - (unit.viewDistance * 32.0f), trapezoid.p2.y};

        float northAngle = -90.0f;

        float angleDelta = angleDifference(northAngle, unit.facingAngle);

        rotateTrapezoid(trapezoid, angleDelta);
        unit.visionTrapezoid = trapezoid;
    }
}

void sVisibility(std::vector<Unit> &allUnits, Player &player)
{
    for (auto &unit : allUnits)
    {
        IsoscelesTrapezoid trap1 = unit.visionTrapezoid;

        for (auto &unit2 : allUnits)
        {
            if (unit2.uuid != unit.uuid)
            {
                if (unit.team != unit2.team)
                {
                    if (checkTrapRectOverlap(trap1, {unit2.pos.x, unit2.pos.y, static_cast<float>(unit2.tex.width), static_cast<float>(unit2.tex.height)}))
                    {
                        unit2.isVisibleToOppositeTeam = true;
                    }
                    else
                    {
                        unit2.isVisibleToOppositeTeam = false;
                    }
                }
            }
        }
    }
}

// NOTE it seems there is overlap on the top corners of the trapezoid even though the trap doesn't extend into that tile
void DEBUGsHoveredTileOverlappingTrap(Tile *&hoveredTile, std::vector<Unit> &allUnits)
{
    for (auto &unit : allUnits)
    {
        if (checkTrapRectOverlap(unit.visionTrapezoid, {hoveredTile->pos.x, hoveredTile->pos.y, 32.0f, 32.0f}))
        {
            std::cout << "OVERLAP" << std::endl;
        }
        else
        {
            std::cout << "NO OVERLAP" << std::endl;
        }
    }
}

bool shouldRenderUnitDueToVisibility(Unit &unit, Player &player)
{
    if ((unit.team != player.team && unit.isVisibleToOppositeTeam) || unit.team == player.team)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Function to move units smoothly
void sMoveUnits(std::vector<Unit> &allUnits, float deltaTime)
{
    for (auto &unit : allUnits)
    {
        if (unit.movePoint.x != -1.0f || unit.movePoint.y != -1.0)
        {
            // Calculate direction to move towards
            Vector2 direction = {unit.movePoint.x - unit.pos.x, unit.movePoint.y - unit.pos.y};
            float distance = Vector2Distance({unit.pos.x, unit.pos.y}, {unit.movePoint.x, unit.movePoint.y});

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
                    unit.pos = unit.movePoint;
                    unit.movePoint = {-1.0f, -1.0f}; // Reset move point
                }
                else
                {
                    // Move towards the move point
                    unit.pos.x += direction.x * moveStep;
                    unit.pos.y += direction.y * moveStep;
                }
            }
        }
    }
}