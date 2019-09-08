#include "api.h"
#include <string>

namespace MechMania {

Game::Game(gameJson, playerId) {
  // TODO: use gameJson to set gameId_ field
}

Position::Position(positionJson) {
  // TODO: parse Json to find Position information
}

Unit::Unit(unitJson) {
  // TODO: parse Json to find Unit information
}

Tile::Tile(tileJson) {
  // TODO: parse Json to find Tile information
}

// updates the game json. Called every turn
void Game::updateGame(setGameJson) { gameJson = setGameJson; }

Unit[] Game::convertJsonToUnits(std::string unitsJson) {
  Unit units[3];
  for (int i = 0; i < 3; i++) {
    units[i] = Unit(unitsJson[i]);
  }
  return units;
}

Unit[] Game::getMyUnits() {
  if (playerId == 1) {
    // TODO: get p1Units
    return convertJsonToUnits(p1Units);
  } else {
    // TODO: get p2Units
    return convertJsonToUnits(p2Units);
  }
}

Unit[] Game::getEnemyUnits() {
  if (playerId == 1) {
    // TODO: get p2Units
    return convertJsonToUnits(p2Units);
  } else {
    // TODO: get p1Units
    return convertJsonToUnits(p1Units);
  }
}

Tile Game::getTile(Position p) {
  // TODO: get tile using position
  return Tile(tileJson);
}

Unit Game::getUnitAt(Position p) {
  Tile tile = getTile(p);
  return tile.unit;
}

std::vector<std::string> Game::pathTo(Position start, Position end,
                                      std::vector<Tile> tilesToAvoid) {
  // TODO: copy python version here
}

} // namespace MechMania
