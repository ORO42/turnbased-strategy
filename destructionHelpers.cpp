#include "include/destructionHelpers.h"

void sDestroyUnits(std::vector<Unit> &allUnits, Unit *&selectedUnit, Ability *&selectedAbility)
{
    allUnits.erase(std::remove_if(allUnits.begin(), allUnits.end(), [&](Unit &unit)
                                  {
                                      if (unit.health <= 0.0)
                                      {
                                          for (auto &ability : unit.abilities)
                                          {
                                              if (selectedAbility && ability.uuid == selectedAbility->uuid)
                                              {
                                                  selectedAbility = nullptr;
                                              }
                                          }
                                          if (selectedUnit && selectedUnit->uuid == unit.uuid)
                                          {
                                              selectedUnit = nullptr;
                                          }
                                          return true; // should remove
                                      }
                                      return false; // Keep this unit
                                  }),
                   allUnits.end());
}

void sDestroyObstacles(std::vector<Obstacle> &allObstacles)
{
    allObstacles.erase(std::remove_if(allObstacles.begin(), allObstacles.end(), [&](Obstacle &obsacle)
                                      {
                                          if (obsacle.health <= 0.0)
                                          {
                                              return true; // should remove
                                          }
                                          return false; // Keep this unit
                                      }),
                       allObstacles.end());
}