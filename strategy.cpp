#include "api.h"

#include <vector>

using std::vector;

namespace MechMania {

vector<UnitSetup> Game::getSetup() {
  vector<UnitSetup> units;
  for (int i = 0; i < units.size(); i++) {
    vector<vector<int>> attackPattern;
    for (int x = 0; x < 7; x++) {
      attackPattern.push_back(vector<int>());
      for (int y = 0; y < 7; y++) {
        attackPattern[x].push_back(0);
      }
    }
    int health = 5;
    int speed = 4;
    UnitSetup unitSetup = {attackPattern, health, speed};
    units.push_back(unitSetup);
  }

  return units;
}

Decision Game::doTurn() {
  vector<int> priorities = {1, 2, 3};
  vector<vector<Direction>> movements = {
      {UP, DOWN, LEFT, DOWN}, {UP, DOWN, LEFT, RIGHT}, {DOWN, UP, LEFT, RIGHT}};
  vector<Direction> attacks = {DOWN, UP, LEFT};

  Decision decision = {priorities, movements, attacks};
  return decision;
}

} // namespace MechMania
