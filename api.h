#pragma once

#include <string>
#include <vector>

#include "Resources/json.hpp"

using json = nlohmann::json;

namespace MechMania {

/**
 * The number of bots per player.
 */
static constexpr int NUM_BOTS = 3;

/**
 * Possible directions that a Unit can go
 */
enum Direction { UP, DOWN, LEFT, RIGHT };
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
};

/**
 * A single unit within the game.
 */
struct Unit {
  int hp;
  int speed;
  std::vector<std::vector<int>> attack;
  bool isAlive;
  int id;
  Position pos;
};

/**
 * A Tile on the board, which keeps its HP, as well as any Units that happen to
 * be on it.
 */
struct Tile {
  int id;
  int hp;
  Unit unit;
  TileType type;
};

/**
 * A struct to hold the information regarding the initial setup of a Unit.
 */
struct UnitSetup {
  std::vector<std::vector<int>> attackPattern;
  int health;
  int speed;
};

/**
 * A struct to hold the information regarding the Decision that the player will
 * make each turn using its own Units
 */
struct Decision {
  std::vector<int> priorities;
  std::vector<std::vector<Direction>> movements;
  std::vector<Direction> attacks;
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
 * Serialize Decision into json.
 */
void to_json(json &j, const Decision &d);
/**
 * Deserialize json into Decision.
 */
void from_json(const json &j, Decision &d);

/**
 * Class that holds the information for one MM25 game.
 */
class Game {
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

  /**
   * Checks if a Decision is valid.
   */
  static bool isDecisionValid(Decision);

  /**
   * Checks if a UnitSetup is valid.
   */
  static bool isUnitSetupValid(UnitSetup);

public:
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
   * Gets the setups for each unit (UnitSetup). Default behavior will
   * initialize three Units with 0 attack anywhere within the grid, 5 health,
   * and 4 speed.
   */
  virtual std::vector<UnitSetup> getSetup();
  /**
   * Gets the Decision that the player makes for that turn.
   * Default behavior:
   * Unit 1: UP, UP, UP, UP, attack DOWN
   * Unit 2: DOWN, DOWN, DOWN, DOWN, attack UP
   * Unit 3: LEFT, LEFT, LEFT, LEFT, attack LEFT
   */
  virtual Decision doTurn();

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
  std::vector<Direction> pathTo(Position, Position, std::vector<Tile>);
};

} // namespace MechMania

/**
 * operator<< overload to convert UnitSetup to a readable format.
 */
std::ostream &operator<<(std::ostream &os, const MechMania::UnitSetup &s);

/**
 * operator<< overload to convert Decision to a readable format.
 */
std::ostream &operator<<(std::ostream &os, const MechMania::Decision &s);
