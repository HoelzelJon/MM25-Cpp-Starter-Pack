#pragma once

#include <string>
#include <vector>

#include "json.hpp"

using json = nlohmann::json;

namespace MechMania {

enum Direction { UP, DOWN, LEFT, RIGHT };
enum TileType { BLANK, DESTRUCTABLE, INDESTRUCTABLE };

struct Position {
  int x;
  int y;
};

struct Unit {
  int hp;
  int speed;
  std::vector<std::vector<int>> attack;
  bool isAlive;
  int id;
  Position pos;
};

struct Tile {
  int id;
  int hp;
  Unit unit;
  TileType type;
};

struct UnitSetup {
  std::vector<std::vector<int>> attackPattern;
  int health;
  int speed;
};

struct Decision {
  std::vector<int> priorities;
  std::vector<std::vector<Direction>> movements;
  std::vector<Direction> attacks;
};

void to_json(json &j, const Position &p);
void from_json(const json &j, Position &p);
void to_json(json &j, const Unit &u);
void from_json(const json &j, Unit &u);
void to_json(json &j, const Tile &t);
void from_json(const json &j, Tile &t);
void to_json(json &j, const UnitSetup &s);
void from_json(const json &j, UnitSetup &s);
void to_json(json &j, const Decision &d);
void from_json(const json &j, Decision &d);

class Game {
  nlohmann::json gameJson_;
  int playerId_;
  int gameId_;

public:
  Game(std::string setGameJson, int setPlayerId);
  std::vector<UnitSetup> getSetup();
  Decision doTurn();

  void updateGame(std::string gameJson);
  std::vector<Unit> convertJsonToUnits(std::string unitsJson);
  std::vector<Unit> getMyUnits();
  std::vector<Unit> getEnemyUnits();
  Tile getTile(Position p);
  Unit getUnitAt(Position p);
  std::vector<Direction> pathTo(Position start, Position end,
                                std::vector<Tile> tilesToAvoid);
};

} // namespace MechMania
