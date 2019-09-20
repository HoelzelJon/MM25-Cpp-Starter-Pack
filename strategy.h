#pragma once
#include "api.h"

class Strategy : public MechMania::Game {
  // ---- helper functions to ensure you don't immediately lose a game ----
  static bool isValidUnitSetup(std::vector<MechMania::UnitSetup>);
  static bool isValidUnitDecision(std::vector<MechMania::UnitDecision>);
public:
  // -------------------------- Constructors ------------------------------
  Strategy() : Game(){};
  Strategy(const Strategy &other) : Game(other){};
  Strategy(std::string gameId, int playerId) : Game(gameId, playerId){};

  // --------------- functions from MechMania::Game to override -----------
  std::vector<MechMania::UnitSetup> getSetup();
  std::vector<MechMania::UnitDecision> doTurn();
  void onGameOver(std::string result);
};
