#include "strategy.h"
#include "api.h"

#include <iostream>
#include <set>
#include <string>
#include <vector>

using std::set;
using std::string;
using std::vector;

using namespace MechMania;

vector<UnitSetup> Strategy::getSetup() {
  vector<UnitSetup> units;
  int unitId = playerId_ == 1 ? 1 : 4;

  for (int i = 0; i < NUM_UNITS; i++) {
    vector<vector<int>> attackPattern = vector<vector<int>>(ATTACK_PATTERN_SIZE, vector<int>(ATTACK_PATTERN_SIZE, 0));
    // four immediately surrounding bot
    attackPattern[3][4] = 2;
    attackPattern[4][3] = 2;
    attackPattern[5][4] = 2;
    attackPattern[4][5] = 2;
    int health = 5;
    int speed = 2;
    vector<vector<bool>> terrainCreation =
        vector<vector<bool>>(7, vector<bool>(7, false));
    UnitSetup unitSetup = {attackPattern, terrainCreation, health, speed,
                           unitId++};
    units.push_back(unitSetup);
  }

  return units;
}

vector<UnitDecision> Strategy::doTurn() {
  vector<UnitDecision> decisions;
  int priority = 1;
  for (Unit unit : Game::getMyUnits()) {
    if (unit.id == -1) {
      // bot is dead
      continue;
    }
    // choose a random opponent
    vector<Unit> opponentUnits = Game::getEnemyUnits();
    int randIndex = std::rand() % opponentUnits.size();
    Unit oppUnit = opponentUnits[randIndex];
    Position end = oppUnit.pos;

    // std::cout << "unit " << unit.id << " moving towards unit " << oppUnit.id
    // << " at " << oppUnit.pos.x << ", " << oppUnit.pos.y << std::endl;

    // go in that direction
    vector<Direction> pathToOpp = Game::pathTo(unit.pos, end);
    pathToOpp.resize(unit.speed); // will be invalid if moves > speed
    // std::cout << "unit " << unit.id << " moves ";
    // for (Direction d : pathToOpp)
    //   std::cout << d << " ";
    // std::cout << std::endl;

    // figure out direction to attack in
    Position posAfterMovement =
        Game::getPositionAfterMovement(unit.pos, pathToOpp);
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

void Strategy::onGameOver(string result) {
  if (result == "TIE") {
    // on game tie
    std::cout << "the game ended in a tie" << std::endl;
  } else if (result == "WIN") {
    // you won
    std::cout << "you won!" << std::endl;
  } else {
    // you lost
    std::cout << "you lost :(" << std::endl;
  }
}
