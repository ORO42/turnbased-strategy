#include "include/destructionHelpers.h"

void sDestroyUnits(VectorSharedPointer<Unit> &allUnits, SharedPointer<Unit> &selectedUnit, SharedPointer<Ability> &selectedAbility)
{
    allUnits.erase(std::remove_if(allUnits.begin(), allUnits.end(), [&](const SharedPointer<Unit> &unitPtr)
                                  {
                                      if (unitPtr->health <= 0.0)
                                      {
                                          // Check if any of the unit's abilities match the selected ability
                                          for (const auto &ability : unitPtr->abilities)
                                          {
                                              if (selectedAbility && ability->uuid == selectedAbility->uuid)
                                              {
                                                  selectedAbility = nullptr;
                                              }
                                          }

                                          // Check if the selected unit matches the unit to be removed
                                          if (selectedUnit && selectedUnit->uuid == unitPtr->uuid)
                                          {
                                              selectedUnit = nullptr;
                                          }

                                          return true; // Remove this unit
                                      }
                                      return false; // Keep this unit
                                  }),
                   allUnits.end());
}

void sDestroyObstacles(VectorSharedPointer<Obstacle> &allObstacles)
{
    allObstacles.erase(std::remove_if(allObstacles.begin(), allObstacles.end(), [](const SharedPointer<Obstacle> &obsaclePtr)
                                      {
                                          if (obsaclePtr->health <= 0.0)
                                          {
                                              return true; // Remove this obstacle
                                          }
                                          return false; // Keep this obstacle
                                      }),
                       allObstacles.end());
}

void sDestroyProjectiles(VectorSharedPointer<Projectile> &allProjectiles)
{
    allProjectiles.erase(std::remove_if(allProjectiles.begin(), allProjectiles.end(), [](const SharedPointer<Projectile> &projectilePtr)
                                        {
                                            if (projectilePtr->shouldDestroy)
                                            {
                                                return true; // Remove this projectile
                                            }
                                            return false; // Keep this projectile
                                        }),
                         allProjectiles.end());
}