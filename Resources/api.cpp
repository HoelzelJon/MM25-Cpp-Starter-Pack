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
  // TODO: copy python version here
  return vector<Direction>();
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
