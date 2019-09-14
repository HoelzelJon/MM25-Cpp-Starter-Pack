#pragma once
#include "Resources/api.h"

class Strategy : public MechMania::Game {
public:
  Strategy() : Game(){};
  Strategy(const StrategyOne &other) : Game(other){};
  Strategy(std::string gameId, int playerId) : Game(gameId, playerId){};
  std::vector<MechMania::UnitSetup> getSetup();
  MechMania::Decision doTurn();
};
