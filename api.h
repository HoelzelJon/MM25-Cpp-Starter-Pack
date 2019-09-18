#pragma once

#include <string>
#include <vector>

#include "Resources/json.hpp"

using json = nlohmann::json;

namespace MechMania {

static constexpr int ATTACK_PATTERN_SIZE = 7;
static constexpr int BASE_HEALTH = 1;
static constexpr int BASE_SPEED = 1;
static constexpr int MAX_POINTS = 24;
static constexpr int TERRAIN_COST = 2;
static constexpr int MAX_DAMAGE = 7;
static constexpr int MAX_STAT = 9;
static constexpr int DAMAGE_SCALING[7] = {1, 3, 6, 10, 15, 21, 27};
static constexpr int STAT_SCALING[9] = {1, 2, 4, 6, 9, 12, 16, 20, 25};
static constexpr int NUM_UNITS = 3;

/**
 * Possible directions that a Unit can go
 */
enum Direction { UP, DOWN, LEFT, RIGHT };
/**
 * Possible types of terrain.
 */
enum TileType { BLANK, DESTRUCTABLE, INDESTRUCTABLE };

/**
 * Different possible attack pattern types
 */
enum AttackPatternType { SNIPER, AOE, MELEE };

/**
 * A Position struct that holds information for a x and y coordinate that can
 * be passed into other functions.
 */
struct Position {
  int x;
  int y;
  Position() : x(-1), y(-1){};
  Position(int x_, int y_) : x(x_), y(y_){};
  bool operator==(const Position &other) const {
    return x == other.x && y == other.y;
  }
};

/**
 * A single unit within the game.
 */
struct Unit {
  int hp;
  int speed;
  std::vector<std::vector<int>> attack;
  int id;
  Position pos;
  Unit()
      : hp(0), speed(0),
        attack(std::vector<std::vector<int>>(7, std::vector<int>(7, 0))),
        id(-1), pos(){};
};

/**
 * A Tile on the board, which keeps its HP, as well as any Units that happen to
 * be on it.
 */
struct Tile {
  int id;
  int hp;
  TileType type;
};

/**
 * A struct to hold the information regarding the initial setup of a Unit.
 */
struct UnitSetup {
  std::vector<std::vector<int>> attackPattern;
  std::vector<std::vector<bool>> terrainCreation;
  int health;
  int speed;
  int unitId;
};

/**
 * A struct to hold the information regarding the Decision that the player will
 * make each turn using its own Units
 */
struct UnitDecision {
  int priority;
  std::vector<Direction> movement;
  Direction attack;
  int unitId;
};

/**
 * Serialize Position into json.
 */
void to_json(json &j, const Position &p);
/**
 * Deserialize json into Position.
 */
void from_json(const json &j, Position &p);
/**
 * Serialize Unit into json.
 */
void to_json(json &j, const Unit &u);
/**
 * Deserialize json into Unit.
 */
void from_json(const json &j, Unit &u);
/**
 * Serialize Tile into json.
 */
void to_json(json &j, const Tile &t);
/**
 * Deserialize json into Tile.
 */
void from_json(const json &j, Tile &t);
/**
 * Serialize UnitSetup into json.
 */
void to_json(json &j, const UnitSetup &s);
/**
 * Deserialize json into UnitSetup.
 */
void from_json(const json &j, UnitSetup &s);
/**
 * Serialize UnitDecision into json.
 */
void to_json(json &j, const UnitDecision &d);
/**
 * Deserialize json into UnitDecision.
 */
void from_json(const json &j, UnitDecision &d);

/**
 * Class that holds the information for one MM25 game.
 */
class Game {
protected:
  /**
   * Holds the json for the game, updated every round to replace.
   */
  nlohmann::json gameJson_;
  /**
   * id of the current player, set in the beginning and used throughout
   * to make sure same player's units are being gathered every time.
   */
  int playerId_;
  /**
   * ID of the game, as a string. This uniquely identifies each game, so
   * multiple games can be played using one server.
   */
  std::string gameId_;


public:
  /**
   * Checks if a UnitDecision is valid.
   */
  static bool isUnitDecisionValid(std::vector<UnitDecision>);

  /**
   * Checks if a UnitSetup is valid.
   */
  static bool isUnitSetupValid(std::vector<UnitSetup>);

  /**
   * Gets a basic attack pattern given a enum type
   */
  static std::vector<std::vector<int>> basicAttackPattern(AttackPatternType);
  /**
   * default constructor for a Game, sets gameJson to a blank json, playerId
   * to 0 and gameId to a blank string.
   */
  Game() : gameJson_({}), playerId_(0), gameId_(""){};
  /**
   * Game constructor that uses a gameId and a playerId as inputs.
   */
  Game(std::string, int);
  /**
   * Game copy constructor.
   */
  Game(const Game &);
  /**
   * Game operator=
   */
  Game &operator=(const Game &);
  /**
   * Gets the setups for each unit (UnitSetup).
   */
  virtual std::vector<UnitSetup> getSetup() = 0;
  /**
   * Gets the UnitDecision that the player makes for that turn.
   */
  virtual std::vector<UnitDecision> doTurn() = 0;

  /**
   * private function called by server to update the game json every turn.
   */
  void updateGame(std::string);
  /**
   * Converts a json given as a string to proper unit objects. Mostly used
   * internally.
   */
  std::vector<Unit> convertJsonToUnits(std::string);
  /**
   * Gets a vector of Unit objects that represent the information of the
   * player's Units (playerId specified by the private variable.
   */
  std::vector<Unit> getMyUnits();
  /**
   * Gets a vector of Unit objects that represent the opponent Units.
   */
  std::vector<Unit> getEnemyUnits();
  /**
   * Get a tile at a specific Position, given by a Position object.
   */
  Tile getTile(Position);
  /**
   * Get a Unit at a position, if a Unit exists at that position. Else returns
   * null.
   */
  Unit getUnitAt(Position);
  /**
   * Gives a path from a Position to another Position, while avoiding Tiles,
   * given by tilesToAvoid
   */
  std::vector<Direction> pathTo(Position, Position, std::vector<Position>);
  std::vector<Direction> pathTo(Position, Position);

  std::vector<std::pair<Position, int>>
  getPositionsOfAttackPattern(int unitId, Direction dir);

  Position getPositionAfterMovement(Position, std::vector<Direction>);

  Unit getUnit(int unitId);

private:
  std::vector<std::vector<int>>
  rotateAttackPattern(std::vector<std::vector<int>>);
};

} // namespace MechMania

/**
 * operator<< overload to convert UnitSetup to a readable format.
 */
std::ostream &operator<<(std::ostream &os, const MechMania::UnitSetup &s);

/**
 * operator<< overload to convert UnitDecision to a readable format.
 */
std::ostream &operator<<(std::ostream &os, const MechMania::UnitDecision &s);
