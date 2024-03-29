#include "../api.h"
#include "json.hpp"
#include "../strategy.h"

#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <string>

using json = nlohmann::json;
using std::pair;
using std::queue;
using std::set;
using std::string;
using std::vector;
using namespace MechMania;

Game::Game(string gameJson, int playerId)
    : playerId_(playerId) {
  auto parsedJson = json::parse(gameJson);
  playerId_ = parsedJson["playerNum"].get<int>();
}

Game::Game(const Game &other)
    : tiles_(other.tiles_), units_(other.units_), playerId_(other.playerId_) {
}

Game &Game::operator=(const Game &other) {
  tiles_ = other.tiles_;
  units_ = other.units_;
  playerId_ = other.playerId_;
  return *this;
}

void Game::updateGame(string setGameJson) {
  auto gameJson = json::parse(setGameJson);
  tiles_ = gameJson["tiles"].get<vector<vector<Tile>>>();
  units_ = gameJson["units"].get<vector<Unit>>();
  playerId_ = gameJson["playerNum"].get<int>();
}

vector<Unit> Game::getMyUnits() {
  if (playerId_ == 1) {
    return { getUnit(1), getUnit(2), getUnit(3) };
  } else {
    return { getUnit(4), getUnit(5), getUnit(6) };
  }
}

vector<Unit> Game::getEnemyUnits() {
  if (playerId_ == 2) {
    return { getUnit(1), getUnit(2), getUnit(3) };
  } else {
    return { getUnit(4), getUnit(5), getUnit(6) };
  }
}

Tile Game::getTile(Position p) {
  return tiles_[p.x][p.y];
}

Unit Game::getUnitAt(Position p) {
  for (Unit unit : units_) {
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
  queue<pair<Position, vector<Direction>>> q;
  q.push(std::make_pair(start, vector<Direction>()));

  vector<vector<bool>> visited(tiles_.size(), vector<bool>(tiles_.size(), false));

  while (!q.empty()) {
    pair<Position, vector<Direction>> p = q.front();
    q.pop();
    Position pos = p.first;
    vector<Direction> dirs = p.second;

    if (pos.x < 0 || pos.y < 0) {
      continue;
    }

    if (visited[pos.x][pos.y]) {
      continue;
    } else {
      visited[pos.x][pos.y] = true;
    }

    if (pos == end) {
      return dirs;
    }

    Position left = {pos.x - 1, pos.y};
    if (!(left.x < 0 ||
          std::find(tilesToAvoid.begin(), tilesToAvoid.end(), left) !=
              tilesToAvoid.end() ||
          getTile(left).type != TileType::BLANK ||
          visited[left.x][left.y])) {
      vector<Direction> leftDirs(dirs);
      leftDirs.push_back(Direction::LEFT);
      q.push(std::make_pair(left, leftDirs));
    }

    Position right = {pos.x + 1, pos.y};
    if (!(right.x >= (int)tiles_.size() ||
          std::find(tilesToAvoid.begin(), tilesToAvoid.end(), right) !=
              tilesToAvoid.end() ||
          getTile(right).type != TileType::BLANK ||
          visited[right.x][right.y])) {
      vector<Direction> rightDirs(dirs);
      rightDirs.push_back(Direction::RIGHT);
      q.push(std::make_pair(right, rightDirs));
    }

    Position up = {pos.x, pos.y + 1};
    if (!(up.y >= (int)tiles_.size() ||
          std::find(tilesToAvoid.begin(), tilesToAvoid.end(), up) !=
              tilesToAvoid.end() ||
          getTile(up).type != TileType::BLANK ||
          visited[up.x][up.y])) {
      vector<Direction> upDirs(dirs);
      upDirs.push_back(Direction::UP);
      q.push(std::make_pair(up, upDirs));
    }

    Position down = {pos.x, pos.y - 1};
    if (!(down.y < 0 ||
          std::find(tilesToAvoid.begin(), tilesToAvoid.end(), down) !=
              tilesToAvoid.end() ||
          getTile(down).type != TileType::BLANK ||
          visited[down.x][down.y])) {
      vector<Direction> downDirs(dirs);
      downDirs.push_back(Direction::DOWN);
      q.push(std::make_pair(down, downDirs));
    }
  }

  return vector<Direction>(1, Direction::STAY);
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
      if (xCoord >= 0 && xCoord < (int)tiles_.size() && yCoord >= 0 &&
          yCoord < (int)tiles_.size()) {
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
    default:
      break;
    }
  }

  return Position(init.x + dx, init.y + dy);
}

Unit Game::getUnit(int unitId) {
  for (Unit &unit : units_) {
    if (unit.id == unitId) {
      return unit;
    }
  }
  return Unit();
}
