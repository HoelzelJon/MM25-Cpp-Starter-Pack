#pragma once
#include "api.h"

class Strategy : public MechMania::Game {
public:
  Strategy() : Game(){};
  Strategy(const Strategy &other) : Game(other){};
  Strategy(std::string gameId, int playerId) : Game(gameId, playerId){};
  std::vector<MechMania::UnitSetup> getSetup();
  std::vector<MechMania::UnitDecision> doTurn();
};
