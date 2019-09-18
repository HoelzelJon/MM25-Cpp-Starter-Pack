#include "../api.h"
#include "json.hpp"

#include <iostream>
#include <string>
#include <queue>
#include <algorithm>

using json = nlohmann::json;
using std::string;
using std::vector;
using std::pair;
using std::queue;
using namespace MechMania;

Game::Game(string gameJson, int playerId)
    : gameJson_(gameJson), playerId_(playerId) {
  // std::cout << "in Game::Game(string, int)" << std::endl;
  auto parsedJson = json::parse(gameJson);
  gameId_ = parsedJson["gameId"].get<string>();
}

Game::Game(const Game &other)
    : gameJson_(other.gameJson_), playerId_(other.playerId_) {
  // std::cout << "in Game::Game(Game)" << std::endl;
}

Game &Game::operator=(const Game &other) {
  // std::cout << "in Game::operator=(Game)" << std::endl;
  gameJson_ = other.gameJson_;
  playerId_ = other.playerId_;
  gameId_ = other.gameId_;
  return *this;
}

// updates the game json. Called every turn
void Game::updateGame(string setGameJson) {
  gameJson_ = json::parse(setGameJson);
}

vector<Unit> Game::convertJsonToUnits(string unitsJson) {
  auto unitJson = json::parse(unitsJson);
  return unitJson.get<vector<Unit>>();
}

vector<Unit> Game::getMyUnits() {
  if (playerId_ == 1) {
    return gameJson_["p1Units"].get<vector<Unit>>();
  } else {
    return gameJson_["p2Units"].get<vector<Unit>>();
  }
}

vector<Unit> Game::getEnemyUnits() {
  if (playerId_ == 1) {
    return gameJson_["p2Units"].get<vector<Unit>>();
  } else {
    return gameJson_["p1Units"].get<vector<Unit>>();
  }
}

Tile Game::getTile(Position p) {
  return gameJson_["map"]["tiles"].get<vector<vector<Tile>>>().at(p.x).at(p.y);
}

Unit Game::getUnitAt(Position p) {
  Tile tile = getTile(p);
  return tile.unit;
}

vector<Direction> Game::pathTo(Position start, Position end,
                               vector<Position> tilesToAvoid) {
  queue<pair<Position, vector<Direction>>> q;
  q.push(std::make_pair(Position(-1, -1), vector<Direction>()));

  vector<vector<Tile>> tiles =
      gameJson_["map"]["tiles"].get<vector<vector<Tile>>>();
  vector<vector<bool>> visited(tiles.size(), vector<bool>(tiles.size()));

  while (!q.empty()) {
    pair<Position, vector<Direction>> p = q.front();
    q.pop();
    Position pos = p.first;
    vector<Direction> dirs = p.second;

    if (visited[pos.y][pos.x]) {
      continue;
    } else {
      visited[pos.y][pos.x] = true;
    }

    if (pos == end) {
      return dirs;
    }

    Position left = {pos.x - 1, pos.y};
    if (!(left.x < 0 ||
          std::find(tilesToAvoid.begin(), tilesToAvoid.end(), left) !=
              tilesToAvoid.end() ||
          getTile(left).type != TileType::BLANK)) {
      vector<Direction> leftDirs(dirs);
      leftDirs.push_back(Direction::LEFT);
      q.push(pair<Position, vector<Direction>>(left, leftDirs));
    }

    Position right = {pos.x + 1, pos.y};
    if (!(right.x > tiles.size() ||
          std::find(tilesToAvoid.begin(), tilesToAvoid.end(), right) !=
              tilesToAvoid.end() ||
          getTile(right).type != TileType::BLANK)) {
      vector<Direction> rightDirs(dirs);
      rightDirs.push_back(Direction::LEFT);
      q.push(pair<Position, vector<Direction>>(right, rightDirs));
    }

    Position up = {pos.x, pos.y + 1};
    if (!(up.y > tiles.size() ||
          std::find(tilesToAvoid.begin(), tilesToAvoid.end(), up) !=
              tilesToAvoid.end() ||
          getTile(up).type != TileType::BLANK)) {
      vector<Direction> upDirs(dirs);
      upDirs.push_back(Direction::LEFT);
      q.push(pair<Position, vector<Direction>>(up, upDirs));
    }

    Position down = {pos.x, pos.y - 1};
    if (!(down.y < 0 ||
          std::find(tilesToAvoid.begin(), tilesToAvoid.end(), down) !=
              tilesToAvoid.end() ||
          getTile(down).type != TileType::BLANK)) {
      vector<Direction> downDirs(dirs);
      downDirs.push_back(Direction::LEFT);
      q.push(pair<Position, vector<Direction>>(down, downDirs));
    }
  }

  return vector<Direction>();
}

bool Game::isDecisionValid(Decision d) {
  // TODO: implement
  return false;
}

bool Game::isUnitSetupValid(UnitSetup s) {
  // TODO: implement
  return false;
}

vector<pair<Position, int>>
Game::getPositionsOfAttackPattern(int unitId, Direction dir) {
  Unit unit = getUnit(unitId);
  vector<vector<int>> &attackPattern = unit.attack;
  switch (dir) {
  case LEFT:
    attackPattern = rotateAttackPattern(attackPattern);
  case DOWN:
    attackPattern = rotateAttackPattern(attackPattern);
  case RIGHT:
    attackPattern = rotateAttackPattern(attackPattern);
  case UP:
    break;
  default:
    return vector<pair<Position, int>>();
  }
  vector<vector<Tile>> tiles =
      gameJson_["map"]["tiles"].get<vector<vector<Tile>>>();

  vector<pair<Position, int>> attacks;
  for (int row = 0; row < attackPattern.size(); row++) {
    for (int col = 0; col < attackPattern[row].size(); col++) {
      int attack = attackPattern[row][col];
      if (attack == 0)
        continue;

      int xPos = unit.pos.x;
      int yPos = unit.pos.y;
      int xCoord = xPos + col - 3;
      int yCoord = yPos + row - 3;
      if (xCoord >= 0 && xCoord < tiles.size() && yCoord >= 0 &&
          yCoord < tiles.size()) {
        attacks.push_back(std::make_pair(Position(xCoord, yCoord), attack));
      }
    }
  }

  return attacks;
}

vector<vector<int>> Game::rotateAttackPattern(vector<vector<int>> attackPattern) {
  // TODO: implement
}

Position Game::getPositionAfterMovement(Position init,
                                  std::vector<Direction> movementSteps) {
  // TODO: implement
}

std::vector<std::vector<int>> Game::basicAttackPattern(AttackPatternType attackType) {
  // TODO: implement
}

Unit Game::getUnit(int unitId) {
  // TODO: implement
}

vector<UnitSetup> Game::getSetup() {
  vector<UnitSetup> units;
  for (int i = 0; i < NUM_BOTS; i++) {
    vector<vector<int>> attackPattern(7, vector<int>(7));
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
      {UP, UP, UP, UP}, {DOWN, DOWN, DOWN, DOWN}, {LEFT, LEFT, LEFT, LEFT}};
  vector<Direction> attacks = {DOWN, UP, LEFT};

  Decision decision = {priorities, movements, attacks};
  return decision;
}
