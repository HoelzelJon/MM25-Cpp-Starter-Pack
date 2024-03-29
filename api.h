#pragma once

#include <string>
#include <vector>

#include "Resources/json.hpp"

using json = nlohmann::json;

namespace MechMania {

  // -------------------- constants, enums, and structs ----------------------
static constexpr int ATTACK_PATTERN_SIZE = 7;
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
  std::vector<std::vector<bool>> terrain;
  int id;
  Position pos;
  Unit()
      : hp(0), speed(0),
        attack(std::vector<std::vector<int>>(7, std::vector<int>(7, 0))),
        terrain(std::vector<std::vector<bool>>(7, std::vector<bool>(7, false))),
        id(-1), pos(){};
  Unit(int hp_, int speed_, std::vector<std::vector<int>> attack_, std::vector<std::vector<bool>> terrain_,int id_,
       Position pos_)
      : hp(hp_), speed(speed_), attack(attack_), terrain(terrain_), id(id_), pos(pos_) {};
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
  UnitSetup(std::vector<std::vector<int>> attackPattern_,
            std::vector<std::vector<bool>> terrainCreation_,
            int health_, int speed_, int unitId_)
           : attackPattern(attackPattern_), terrainCreation(terrainCreation_),
             health(health_), speed(speed_), unitId(unitId_) {};
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
  UnitDecision(int priority_, std::vector<Direction> movement_,
               Direction attack_, int unitId_)
              : priority(priority_), movement(movement_), attack(attack_),
                unitId(unitId_) {};
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
   * params:
   *   Position start: A Position object for the start position
   *   Position end: A Position object for the end position
   *   std::vector<Position> tilesToAvoid: A vector of Position objects indicating which tiles to avoid
   * Returns a vector of Directions indicating the shortest path from the desired positions
   */
  std::vector<Direction> pathTo(Position, Position, std::vector<Position>);
  std::vector<Direction> pathTo(Position, Position);
  /**
   * Gives the position and the attack damage of a unit at each position in its
   * attack pattern if the unit was attacking in the direction given.
   * params:
   *   unitId: The Id of the desired unit
   *   dir: Direction to attack in
   * output:
   *   Returns a vector of <Position, int> pairs where the int is the damage at that Position
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
