#include "strategy.h"
#include "api.h"

#include <iostream>
#include <vector>

using std::vector;

using namespace MechMania;

vector<UnitSetup> Strategy::getSetup() {
  vector<UnitSetup> units;
  int unitId = playerId_ == 1 ? 1 : 4;

  for (int i = 0; i < NUM_UNITS; i++) {
    vector<vector<int>> attackPattern(7, vector<int>(7, 0));
    vector<vector<bool>> terrainCreation(7, vector<bool>(7, false));
    attackPattern[3][4] = 1;
    int health = 5;
    int speed = 4;
    UnitSetup unitSetup = {attackPattern, terrainCreation, health, speed, unitId++};
    units.push_back(unitSetup);
  }

  return units;
}

vector<UnitDecision> Strategy::doTurn() {
  return vector<UnitDecision>();
}
