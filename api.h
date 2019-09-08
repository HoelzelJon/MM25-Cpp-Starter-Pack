#include <string>
#include <vector>

namespace MechMania {

class Game {
  std::string gameJson_;
  int playerId_;
  int gameId_;

public:
  Game(std::string setGameJson, int setPlayerId)
      : gameJson_(setGameJson), playerId_(setPlayerId);
  UnitSetup[] getSetup();
  Decision doTurn();

  void updateGame(gameJson);
  Unit[] convertJsonToUnits(unitsJson);
  Unit[] getMyUnits();
  Unit[] getEnemyUnits();
  Tile getTile(Position p);
  Unit getUnitAt(Position p);
  Direction pathTo(Position start, Position end, vector<Tile> tilesToAvoid);
};

enum Direction { UP, DOWN, LEFT, RIGHT };

struct UnitSetup {
  int attackPattern[7][7];
  int health;
  int speed;

  UnitSetup(int setAttackPattern[][], int setHealth, int setSpeed)
      : attackPattern(setAttackPattern), health(setHealth), speed(setSpeed);
};

struct Decision {
  int priorities[3];
  std::vector<Direction> movements[3];
  Direction attacks[3];

  Decision(int setPriorities[], std::vector<Direction> setMovements[],
           Direction setAttacks[])
      : priorities(setPriorities), movements(setMovements), attacks(setAttacks);
};

struct Position {
  int x;
  int y;

  Position(positionJson);
};

struct Unit {
  int hp;
  int speed;
  int attack;
  bool isAlive;
  int id;
  Position pos;
  Unit unit;

  Unit(unitJson);
};

struct Tile {
  int id;
  int hp;
  Unit unit;
  std::string type;

  Tile(tileJson);
};

} // namespace MechMania
