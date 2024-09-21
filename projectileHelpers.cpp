#include "include/projectileHelpers.h"

SharedPointer<Projectile> createProjectile(
    VectorSharedPointer<Projectile> &allProjectiles,
    Texture2D tex,
    Rectangle originUnitRect,
    Rectangle targetRect,
    std::string originUnitUuid,
    Position currentPos,
    Teams team,
    float facingAngle,
    float speed,
    bool canCollideBeforeDestination,
    bool causesExplosion,
    int projectileAccuracyRadius,
    int projectileDamageRadius,
    float damage,
    float accuracy,
    int reachRadius)
{
    SharedPointer<Projectile> newProjectile = std::make_shared<Projectile>();
    newProjectile->tex = tex;
    newProjectile->originUnitRect = originUnitRect;
    // newProjectile.destinationPos = destinationPos;
    newProjectile->targetRect = targetRect;
    newProjectile->currentPos = currentPos; // TODO make this center of origin rect
    newProjectile->originUnitUuid = originUnitUuid;
    newProjectile->team = team;
    newProjectile->facingAngle = facingAngle;
    newProjectile->speed = speed;
    newProjectile->canCollideBeforeDestination = canCollideBeforeDestination;
    newProjectile->shouldDestroy = false;
    newProjectile->causesExplosion = causesExplosion;
    newProjectile->damageRadius = projectileDamageRadius;
    newProjectile->accuracyRadius = projectileAccuracyRadius;
    newProjectile->damage = damage;
    newProjectile->accuracy = accuracy;
    newProjectile->reachRadius = reachRadius;

    allProjectiles.push_back(newProjectile);

    return newProjectile;
}

void sMoveProjectiles(VectorSharedPointer<Projectile> &allProjectiles, VectorSharedPointer<Unit> &allUnits, VectorSharedPointer<Obstacle> &allObstacles, float deltaTime)
{
    for (auto &projectile : allProjectiles)
    {

        // Calculate movement in the x and y directions based on the angle
        float moveStep = projectile->speed * deltaTime;
        float angleInRadians = projectile->facingAngle * (PI / 180.0f);
        float moveX = moveStep * std::cos(angleInRadians);
        float moveY = moveStep * std::sin(angleInRadians);

        Rectangle projectileRect = {projectile->currentPos.x, projectile->currentPos.y, static_cast<float>(projectile->tex.width), static_cast<float>(projectile->tex.height)};

        // projectile should not travel beyond either map border or projectile reach radius (TBD)
        // if (!CheckCollisionRecs(projectileRect, createRectAroundRect(projectile->originUnitRect, projectile->reachRadius)))
        // {
        //     projectile->shouldDestroy = true;
        // }
        if (projectile->canCollideBeforeDestination)
        {
            for (auto &unit : allUnits)
            {
                if (unit->uuid != projectile->originUnitUuid)
                {
                    Rectangle unitRect = {unit->pos.x, unit->pos.y, static_cast<float>(unit->tex.width), static_cast<float>(unit->tex.height)};
                    if (CheckCollisionRecs(projectileRect, unitRect))
                    {
                        bool shouldHitOriginalTarget = calculateChance(projectile->accuracy);
                        std::cout << unit->uuid << std::endl;
                        if (unit->projectileStoppage == ProjectileStoppage::ALWAYS || (unit->projectileStoppage == ProjectileStoppage::SOMETIMES && !shouldHitOriginalTarget) || (shouldHitOriginalTarget && CheckCollisionRecs(projectile->targetRect, unitRect)))
                        {
                            projectile->shouldDestroy = true;
                            projectile->targetRect = unitRect;
                            break;
                        }
                    }
                }
            }
            for (auto &obstacle : allObstacles)
            {
                // TODO don't damage the obstacle the unit is occupying, depending on stoppage type
                Rectangle obstacleRect = {obstacle->pos.x, obstacle->pos.y, static_cast<float>(obstacle->tex.width), static_cast<float>(obstacle->tex.height)};
                if (CheckCollisionRecs(projectileRect, obstacleRect))
                {
                    bool shouldHitOriginalTarget = calculateChance(projectile->accuracy);
                    if (obstacle->projectileStoppage == ProjectileStoppage::ALWAYS || (obstacle->projectileStoppage == ProjectileStoppage::SOMETIMES && !shouldHitOriginalTarget || (shouldHitOriginalTarget && CheckCollisionRecs(projectile->targetRect, obstacleRect))))
                    {
                        projectile->shouldDestroy = true;
                        projectile->targetRect = obstacleRect;
                        break;
                    }
                }
            }
        }

        if (projectile->shouldDestroy)
        {
            Rectangle radiusRect = createRectAroundRect(projectile->targetRect, projectile->damageRadius);
            for (auto &unit : allUnits)
            {
                Rectangle unitRect = {unit->pos.x, unit->pos.y, static_cast<float>(unit->tex.width), static_cast<float>(unit->tex.height)};
                if (CheckCollisionRecs(unitRect, radiusRect))
                {
                    unit->health -= projectile->damage;
                }
            }
            for (auto &obstacle : allObstacles)
            {
                Rectangle obstacleRect = {obstacle->pos.x, obstacle->pos.y, static_cast<float>(obstacle->tex.width), static_cast<float>(obstacle->tex.height)};
                if (CheckCollisionRecs(obstacleRect, radiusRect))
                {
                    obstacle->health -= projectile->damage;
                }
            }
        }
        else
        {
            projectile->currentPos.x += moveX;
            projectile->currentPos.y += moveY;
        }
    }
}