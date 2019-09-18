#pragma once
#include "api.h"

class Strategy2 : public MechMania::Game {
public:
  Strategy2() : Game(){};
  Strategy2(const Strategy2 &other) : Game(other){};
  Strategy2(std::string gameId, int playerId) : Game(gameId, playerId){};
  std::vector<MechMania::UnitSetup> getSetup();
  std::vector<MechMania::UnitDecision> doTurn();
};
