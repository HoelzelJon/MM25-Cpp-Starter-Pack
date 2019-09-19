#include "strategy.h"
#include "api.h"

#include <iostream>
#include <set>
#include <vector>

using std::set;
using std::vector;

using namespace MechMania;

vector<UnitSetup> Strategy::getSetup() {
  vector<UnitSetup> units;
  int unitId = playerId_ == 1 ? 1 : 4;

  for (int i = 0; i < NUM_UNITS; i++) {
    vector<vector<int>> attackPattern =
        Game::basicAttackPattern(AttackPatternType::MELEE);
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
    int randIndex = std::rand() % opponentUnits.size() + 1;
    Unit oppUnit = Game::getUnit(randIndex);
    Position end = oppUnit.pos;

    // go in that direction
    vector<Direction> pathToOpp = Game::pathTo(unit.pos, end);
    pathToOpp.resize(unit.speed); // will be invalid if moves > speed

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


bool Strategy::isValidUnitDecision(vector<UnitDecision> decisions) {
  set<int> uniquePriorities;
  for (UnitDecision d : decisions) {
    if (uniquePriorities.find(d.priority) != uniquePriorities.end() ||
        (d.priority != 1 && d.priority != 2 && d.priority != 3)) {
      return false;
    }
    uniquePriorities.insert(d.priority);
  }

  return true;
}

bool Strategy::isValidUnitSetup(vector<UnitSetup> ss) {
  for (UnitSetup s : ss) {
    if (s.health < BASE_HEALTH || s.speed < BASE_SPEED ||
        (int)s.attackPattern.size() != ATTACK_PATTERN_SIZE ||
        (int)s.terrainCreation.size() != ATTACK_PATTERN_SIZE) {
      return false;
    }

    for (int i = 0; i < ATTACK_PATTERN_SIZE; i++) {
      if ((int)s.attackPattern[i].size() != ATTACK_PATTERN_SIZE ||
          (int)s.terrainCreation[i].size() != ATTACK_PATTERN_SIZE) {
        return false;
      }
    }

    int sum = 0;
    for (vector<int> &row : s.attackPattern) {
      for (int cell : row) {
        if (cell > 1) {
          if (cell >= MAX_DAMAGE) {
            sum = MAX_POINTS + 1;
            break;
          } else {
            sum += DAMAGE_SCALING[cell - 1];
          }

        } else if (cell < 0) {
          return false;
        }

        sum += cell;
      }
    }

    for (vector<bool> &row : s.terrainCreation) {
      for (bool creatingTerrain : row) {
        if (creatingTerrain) {
          sum += TERRAIN_COST;
        }
      }
    }

    if (sum > MAX_POINTS) {
      return false;

    } else if (s.health >= MAX_STAT || s.speed >= MAX_STAT) {
      return false;

    } else if (STAT_SCALING[s.health - 1] + STAT_SCALING[s.speed - 1] + sum >
               MAX_POINTS) {
      return false;
    }
  }

  return true;
}
