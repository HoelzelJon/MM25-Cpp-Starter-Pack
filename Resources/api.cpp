#include "../api.h"
#include "json.hpp"

#include <algorithm>
#include <iostream>
#include <deque>
#include <set>
#include <string>

using json = nlohmann::json;
using std::pair;
using std::deque;
using std::set;
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
    return { getUnit(1), getUnit(2), getUnit(3) };
  } else {
    return { getUnit(4), getUnit(5), getUnit(6) };
  }
}

vector<Unit> Game::getEnemyUnits() {
  if (playerId_ == 1) {
    return { getUnit(1), getUnit(2), getUnit(3) };
  } else {
    return { getUnit(4), getUnit(5), getUnit(6) };
  }
}

Tile Game::getTile(Position p) {
  return gameJson_["tiles"].get<vector<vector<Tile>>>()[p.y][p.x];
}

Unit Game::getUnitAt(Position p) {
  for (Unit unit : gameJson_["units"].get<vector<Unit>>()) {
    if (unit.pos == p) {
      return unit;
    }
  }
  return Unit();
}

std::vector<Direction> Game::pathTo(Position start, Position end) {
  return pathTo(start, end, vector<Position>());
}

vector<Direction> Game::pathTo(Position start, Position end,
                               vector<Position> tilesToAvoid) {
  deque<pair<Position, vector<Direction>>> q;
  q.push_front(std::make_pair(start, vector<Direction>()));

  vector<vector<Tile>> tiles =
      gameJson_["tiles"].get<vector<vector<Tile>>>();
  vector<vector<bool>> visited(tiles.size(), vector<bool>(tiles.size(), false));

  while (!q.empty()) {
    pair<Position, vector<Direction>> p = q.front();
    q.pop_front();
    Position pos = p.first;
    vector<Direction> dirs = p.second;

    if (pos.x < 0 || pos.y < 0) {
      continue;
    }

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
      q.push_front(std::make_pair(left, leftDirs));
    }

    Position right = {pos.x + 1, pos.y};
    if (!(right.x >= (int)tiles.size() ||
          std::find(tilesToAvoid.begin(), tilesToAvoid.end(), right) !=
              tilesToAvoid.end() ||
          getTile(right).type != TileType::BLANK)) {
      vector<Direction> rightDirs(dirs);
      rightDirs.push_back(Direction::LEFT);
      q.push_front(std::make_pair(right, rightDirs));
    }

    Position up = {pos.x, pos.y + 1};
    if (!(up.y >= (int)tiles.size() ||
          std::find(tilesToAvoid.begin(), tilesToAvoid.end(), up) !=
              tilesToAvoid.end() ||
          getTile(up).type != TileType::BLANK)) {
      vector<Direction> upDirs(dirs);
      upDirs.push_back(Direction::LEFT);
      q.push_front(std::make_pair(up, upDirs));
    }

    Position down = {pos.x, pos.y - 1};
    if (!(down.y < 0 ||
          std::find(tilesToAvoid.begin(), tilesToAvoid.end(), down) !=
              tilesToAvoid.end() ||
          getTile(down).type != TileType::BLANK)) {
      vector<Direction> downDirs(dirs);
      downDirs.push_back(Direction::LEFT);
      q.push_front(std::make_pair(down, downDirs));
    }
  }

  return vector<Direction>();
}

vector<pair<Position, int>> Game::getPositionsOfAttackPattern(int unitId,
                                                              Direction dir) {
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
      gameJson_["tiles"].get<vector<vector<Tile>>>();

  vector<pair<Position, int>> attacks;
  for (int row = 0; row < (int)attackPattern.size(); row++) {
    for (int col = 0; col < (int)attackPattern[row].size(); col++) {
      int attack = attackPattern[row][col];
      if (attack == 0)
        continue;

      int xPos = unit.pos.x;
      int yPos = unit.pos.y;
      int xCoord = xPos + col - 3;
      int yCoord = yPos + row - 3;
      if (xCoord >= 0 && xCoord < (int)tiles.size() && yCoord >= 0 &&
          yCoord < (int)tiles.size()) {
        attacks.push_back(std::make_pair(Position(xCoord, yCoord), attack));
      }
    }
  }

  return attacks;
}

vector<vector<int>>
Game::rotateAttackPattern(vector<vector<int>> attackPattern) {
  // https://stackoverflow.com/questions/39668537/rotate-2d-vector-90-degrees-clockwise
  int N = attackPattern.size();

  for (int x = 0; x < N / 2; x++) {
    for (int y = x; y < N - x - 1; y++) {
      // store current cell in attackPattern variable
      int tmp = attackPattern[x][y];

      // move values from right to top
      attackPattern[x][y] = attackPattern[y][N - 1 - x];

      // move values from bottom to right
      attackPattern[y][N - 1 - x] = attackPattern[N - 1 - x][N - 1 - y];

      // move values from left to bottom
      attackPattern[N - 1 - x][N - 1 - y] = attackPattern[N - 1 - y][x];

      // assign attackPattern to left
      attackPattern[N - 1 - y][x] = tmp;
    }
  }

  return attackPattern;
}

Position Game::getPositionAfterMovement(Position init,
                                        std::vector<Direction> movementSteps) {
  int dx = 0, dy = 0;
  for (Direction movement : movementSteps) {
    switch (movement) {
    case LEFT:
      dx -= 1;
      break;
    case RIGHT:
      dx += 1;
      break;
    case DOWN:
      dy -= 1;
      break;
    case UP:
      dy += 1;
      break;
    }
  }

  return Position(init.x + dx, init.y + dy);
}

vector<vector<int>> Game::basicAttackPattern(AttackPatternType attackType) {
  vector<vector<int>> defaultAttack = vector<vector<int>>(7, vector<int>(7, 0));
  switch (attackType) {
  case SNIPER:
    // four on each direction
    defaultAttack[4][0] = 2;
    defaultAttack[0][4] = 2;
    defaultAttack[4][6] = 2;
    defaultAttack[6][4] = 2;
    return defaultAttack;
  case AOE:
    // eight immediately surrounding bot
    defaultAttack[3][3] = 1;
    defaultAttack[5][5] = 1;
    defaultAttack[3][5] = 1;
    defaultAttack[5][3] = 1;
    return defaultAttack;
  case MELEE:
    // four immediately surrounding bot
    defaultAttack[3][4] = 2;
    defaultAttack[4][3] = 2;
    defaultAttack[5][4] = 2;
    defaultAttack[4][5] = 2;
    return defaultAttack;

  default:
    return vector<vector<int>>(7, vector<int>(7, 0));
  }
}

Unit Game::getUnit(int unitId) {
  for (Unit &unit : gameJson_["units"].get<vector<Unit>>()) {
    if (unit.id == unitId) {
      return unit;
    }
  }
  return Unit();
}
