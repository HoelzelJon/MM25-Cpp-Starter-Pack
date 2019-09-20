#pragma once

#include <string>
#include <vector>

#include "Resources/json.hpp"

using json = nlohmann::json;

namespace MechMania {


// -------------------- constants, enums, and structs ----------------------
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
enum Direction { UP, DOWN, LEFT, RIGHT, STAY };
/**
 * Possible types of terrain.
 */
enum TileType { BLANK, DESTRUCTABLE, INDESTRUCTABLE };

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
  bool operator==(const Unit &other) const {
    return id == other.id;
  }
};

/**
 * A Tile on the board, which keeps its HP, as well as any Units that happen to
 * be on it.
 */
struct Tile {
  int id;
  int hp;
  TileType type;
  bool operator==(const Tile &other) const {
    return id == other.id;
  }
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

// --------------------------- Game class ----------------------------------
/**
 * Class that holds the information for one MM25 game.
 */
class Game {
protected:
  /**
   * the 2d vector of tiles in the game, updated every turn
   */
  std::vector<std::vector<Tile>> tiles_;

  /**
   * the std::vector of units, updated every turn
   */
  std::vector<Unit> units_;

  /**
   * id of the current player, set in the beginning and used throughout
   * to make sure same player's units are being gathered every time.
   */
  int playerId_;


public:
  /**
   * default constructor for a Game, sets gameJson to a blank json, playerId
   * to 0 and gameId to a blank string.
   */
  Game() : playerId_(0) {};
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
   * Any clean up or analysis that needs to be done
   */
  virtual void onGameOver(std::string result) = 0;

  /**
   * Called by server to update the game json every turn.
   */
  void updateGame(std::string);
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
  /**
   * Gives the position and the attack damage of a unit at each position in its
   * attack pattern if the unit was attacking in the direction given.
   */
  std::vector<std::pair<Position, int>> getPositionsOfAttackPattern(int unitId, Direction dir);
  /**
   * Position after moving from start position and following a list of
   * directions.
   */
  Position getPositionAfterMovement(Position, std::vector<Direction>);
  /**
   * Get a unit given a unit id. (Remember, if you are player 1, then your
   * units are units 1, 2, and 3. If you are player 2, then your units are
   * units 4, 5, 6)
   */
  Unit getUnit(int unitId);

private:
  /**
   * Rotate a 2d vector 90 degrees to the left.
   */
  std::vector<std::vector<int>>
  rotateAttackPattern(std::vector<std::vector<int>>);
};


// ---------- nlohmann::json serialization/deserialization methods ---------
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

} // namespace MechMania


// -------------------------------- operator overloads --------------------
/**
 * operator<< overload to convert UnitSetup to a readable format.
 */
std::ostream &operator<<(std::ostream &os, const MechMania::UnitSetup &s);

/**
 * operator<< overload to convert UnitDecision to a readable format.
 */
std::ostream &operator<<(std::ostream &os, const MechMania::UnitDecision &s);
