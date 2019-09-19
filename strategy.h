#pragma once
#include "api.h"

class Strategy : public MechMania::Game {
  static bool isValidUnitSetup(std::vector<MechMania::UnitSetup>);
  static bool isValidUnitDecision(std::vector<MechMania::UnitDecision>);
public:
  Strategy() : Game(){};
  Strategy(const Strategy &other) : Game(other){};
  Strategy(std::string gameId, int playerId) : Game(gameId, playerId){};
  std::vector<MechMania::UnitSetup> getSetup();
  std::vector<MechMania::UnitDecision> doTurn();
  void onGameOver(int winner);
};
