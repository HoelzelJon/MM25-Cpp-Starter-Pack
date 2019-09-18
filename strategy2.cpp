#include "strategy2.h"
#include "api.h"

#include <iostream>
#include <stdlib.h>
#include <vector>

using std::vector;

using namespace MechMania;

vector<UnitSetup> Strategy2::getSetup() {
  vector<UnitSetup> units;
  int unitId = playerId_ == 1 ? 1 : 4;

  for (int i = 0; i < NUM_UNITS; i++) {
    vector<vector<int>> attackPattern = Game::basicAttackPattern(AttackPatternType::MELEE);
    int health = 5;
    int speed = 2;
    vector<vector<bool>> terrainCreation = vector<vector<bool>>(7, vector<bool>(7, false));
    UnitSetup unitSetup = {attackPattern, terrainCreation, health, speed, unitId++};
    units.push_back(unitSetup);
  }

  return units;
  // if (Game::isUnitSetupValid(units)) {
  //   return units;
  // }
  // std::cout << "unit setup was not valid" << std::endl;
  // return vector<UnitSetup>();
}

vector<UnitDecision> Strategy2::doTurn() {
  vector<UnitDecision> decisions;
  int priority = 1;
  for (Unit unit : Game::getMyUnits()) {
    // choose a random opponent
    std::cout << "at line " << __LINE__ << std::endl;
    vector<Unit> opponentUnits = Game::getEnemyUnits();
    int randIndex = std::rand() % opponentUnits.size() + 1;
    std::cout << "at line " << __LINE__ << std::endl;
    Unit oppUnit = Game::getUnit(randIndex);
    Position end = oppUnit.pos;

    // go in that direction
    std::cout << "at line " << __LINE__ << std::endl;
    vector<Direction> pathToOpp = Game::pathTo(unit.pos, end);
    pathToOpp.resize(unit.speed);  // will be invalid if moves > speed

    // figure out direction to attack in
    std::cout << "at line " << __LINE__ << std::endl;
    Position posAfterMovement = Game::getPositionAfterMovement(unit.pos, pathToOpp);
    Direction direction;
    if (oppUnit.pos.x > posAfterMovement.x) {
      if (oppUnit.pos.y > posAfterMovement.y) {
        direction = Direction::UP;
      } else if (oppUnit.pos.y < posAfterMovement.y) {
        direction = Direction::DOWN;
      } else {
        direction = Direction::RIGHT;
      }
    } else {
      if (oppUnit.pos.y > posAfterMovement.y) {
        direction = Direction::UP;
      } else if (oppUnit.pos.y < posAfterMovement.y) {
        direction = Direction::DOWN;
      } else {
        direction = Direction::LEFT;
      }
    }

    UnitDecision decision = {priority++, pathToOpp, direction, unit.id};
    decisions.push_back(decision);
  }
  return decisions;
}
