#include "strategy.h"
#include "Resources/api.h"

#include <iostream>
#include <vector>

using std::vector;

using namespace MechMania;

vector<UnitSetup> StrategyOne::getSetup() {
  vector<UnitSetup> units;
  for (int i = 0; i < NUM_BOTS; i++) {
    vector<vector<int>> attackPattern(7, vector<int>(7));
    attackPattern[3][4] = 1;
    int health = 5;
    int speed = 4;
    UnitSetup unitSetup = {attackPattern, health, speed};
    units.push_back(unitSetup);
  }

  return units;
}

Decision StrategyOne::doTurn() {
  vector<int> priorities = {1, 2, 3};
  vector<vector<Direction>> movements = {
      {UP, UP, UP, UP}, {DOWN, DOWN, DOWN, DOWN}, {LEFT, LEFT, LEFT}};
  vector<Direction> attacks = {DOWN, UP, LEFT};

  Decision decision = {priorities, movements, attacks};
  return decision;
}
