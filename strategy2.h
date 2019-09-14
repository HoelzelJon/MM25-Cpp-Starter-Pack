#pragma once
#include "Resources/api.h"

class StrategyTwo : public MechMania::Game {
public:
  StrategyTwo() : Game(){};
  StrategyTwo(const StrategyTwo &other) : Game(other){};
  StrategyTwo(std::string gameId, int playerId) : Game(gameId, playerId){};
  std::vector<MechMania::UnitSetup> getSetup();
  MechMania::Decision doTurn();
};
