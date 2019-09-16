#include "../api.h"
#include "json.hpp"

#include <iostream>
#include <string>

using json = nlohmann::json;
using std::string;
using std::vector;
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
                               vector<Tile> tilesToAvoid) {
  queue<pair<Position, vector<Direction>>> q();
  q.push(pair < Position, vector<Direction>(Position(), vector<Direction>()));

  vector<vector<Tile>> tiles =
      gameJson_["map"]["tiles"].get<vector<vector<Tile>>>();
  vector<vector<bool>> visited(tiles.size(), vector<bool>(tiles.size()));

  while (!q.empty()) {
    pair<Position, vector<Direction>> p = q.pop();
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
      leftDirs.append(Direction::LEFT);
      q.push(pair<Position, vector<Direction>>(left, leftDirs));
    }

    Position right = {pos.x + 1, pos.y};
    if (!(right.x > tiles.size() ||
          std::find(tilesToAvoid.begin(), tilesToAvoid.end(), right) !=
              tilesToAvoid.end() ||
          getTile(right).type != TileType::BLANK)) {
      vector<Direction> rightDirs(dirs);
      rightDirs.append(Direction::LEFT);
      q.push(pair<Position, vector<Direction>>(right, rightDirs));
    }

    Position up = {pos.x, pos.y + 1};
    if (!(up.y > tiles.size() ||
          std::find(tilesToAvoid.begin(), tilesToAvoid.end(), up) !=
              tilesToAvoid.end() ||
          getTile(up).type != TileType::BLANK)) {
      vector<Direction> upDirs(dirs);
      upDirs.append(Direction::LEFT);
      q.push(pair<Position, vector<Direction>>(up, upDirs));
    }

    Position down = {pos.x, pos.y - 1};
    if (!(down.y < 0 ||
          std::find(tilesToAvoid.begin(), tilesToAvoid.end(), down) !=
              tilesToAvoid.end() ||
          getTile(down).type != TileType::BLANK)) {
      vector<Direction> downDirs(dirs);
      downDirs.append(Direction::LEFT);
      q.push(pair<Position, vector<Direction>>(down, downDirs));
    }
  }

  return vector<Direction>();
}

static bool isDecisionValid(Decision d) {
  // TODO: implement
  return false;
}

static bool isUnitSetupValid(UnitSetup s) {
  // TODO: implement
  return false;
}

vector<pair<Position, int>>
getPositionsOfAttackPattern(vector<vector<int>> attackPattern, Direction dir) {
  Unit unit = getUnit(unitId);
  vector<vector<int>> &attackPattern = unit.attackPattern;
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

  vector<pair<Position, int>> attacks;
  for (int row = 0; row < attackPattern.size(); row++) {
    for (int col = 0; col < attackPattern[row].size(); col++) {
      int attack = attackPattern[row][col];
      if (attack == 0)
        continue;

      xPos = unit.pos.x;
      yPos = unit.pos.y;
      xCoord = xPos + col - 3;
      yCoord = yPos + row - 3;
      if (xCoord >= 0 && xCoord < tiles.size() && yCoord >= 0 &&
          yCoord < tiles.size()) {
        attacks.append(pair<Position, int>({xCoord, yCoord}, attack));
      }
    }
  }

  return attacks;
}

vector<vector<int>> rotateAttackPattern(vector<vector<int>> attackPattern) {
  // TODO: implement
}

Position getPositionAfterMovement(Position init,
                                  std::vector<Direction> movementSteps) {
  // TODO: implement
}

std::vector<std::vector<int>> basicAttackPattern(AttackPatternType attackType) {
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
