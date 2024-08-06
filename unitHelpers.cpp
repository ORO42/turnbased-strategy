#include "include/unitHelpers.h"

void createUnit(
    UnitType unitType,
    Position pos,
    Player &player,
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
    newUnit.team = player.team;
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

    if (unitType == UnitType::RIFLEMAN)
    {
        newUnit.attack = 25.0f;
        newUnit.accuracy = 1.0f;
        newUnit.maxAp = 10.0f;
        newUnit.ap = 10.0f;
        newUnit.abilities = std::vector<Ability>{attackAbility, rotateAbility};

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
        std::cout << angleDelta << std::endl;

        rotateTrapezoid(trapezoid, angleDelta);
        unit.visionTrapezoid = trapezoid;
    }
}