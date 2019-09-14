#pragma once
#include "Resources/api.h"

namespace MechMania {

class StrategyOne : public Game {
public:
  std::vector<UnitSetup> getSetup();
  Decision doTurn();
};

} // namespace MechMania
