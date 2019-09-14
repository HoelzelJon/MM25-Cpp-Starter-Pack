#include "api.h"
#include "json.hpp"

using json = nlohmann::json;
using namespace MechMania;

namespace MechMania {

NLOHMANN_JSON_SERIALIZE_ENUM(
    Direction, {{UP, "UP"}, {DOWN, "DOWN"}, {LEFT, "LEFT"}, {RIGHT, "RIGHT"}})

NLOHMANN_JSON_SERIALIZE_ENUM(TileType, {
                                           {BLANK, "BLANK"},
                                           {DESTRUCTABLE, "DESTRUCTABLE"},
                                           {INDESTRUCTABLE, "INDESTRUCTABLE"},
                                       })

void to_json(json &j, const Position &p) { j = json{{"x", p.x}, {"y", p.y}}; }

void from_json(const json &j, Position &p) {
  j.at("x").get_to(p.x);
  j.at("y").get_to(p.y);
}

void to_json(json &j, const Unit &u) {
  j = json{{"hp", u.hp},           {"speed", u.speed}, {"attack", u.attack},
           {"isAlive", u.isAlive}, {"id", u.id},       {"pos", u.pos}};
}

void from_json(const json &j, Unit &u) {
  j.at("hp").get_to(u.hp);
  j.at("speed").get_to(u.speed);
  j.at("attack").get_to(u.attack);
  j.at("isAlive").get_to(u.isAlive);
  j.at("id").get_to(u.id);
  j.at("pos").get_to(u.pos);
}

void to_json(json &j, const Tile &t) {
  j = json{{"id", t.id}, {"hp", t.hp}, {"unit", t.unit}, {"type", t.type}};
}

void from_json(const json &j, Tile &t) {
  j.at("id").get_to(t.id);
  j.at("hp").get_to(t.hp);
  j.at("unit").get_to(t.unit);
  j.at("type").get_to(t.type);
}

void to_json(json &j, const UnitSetup &s) {
  j = json{{"attackPattern", s.attackPattern},
           {"health", s.health},
           {"speed", s.speed}};
}

void from_json(const json &j, UnitSetup &s) {
  j.at("attackPattern").get_to(s.attackPattern);
  j.at("health").get_to(s.health);
  j.at("speed").get_to(s.speed);
}

void to_json(json &j, const Decision &d) {
  j = json{{"priorities", d.priorities},
           {"movements", d.movements},
           {"attacks", d.attacks}};
}

void from_json(const json &j, Decision &d) {
  j.at("priorities").get_to(d.priorities);
  j.at("movements").get_to(d.movements);
  j.at("attacks").get_to(d.attacks);
}

} // namespace MechMania

std::ostream &operator<<(std::ostream &os, const MechMania::UnitSetup &s) {
  os << "UnitSetup(attackPattern=[";
  for (int i = 0; i < s.attackPattern.size(); i++) {
    os << "[";
    for (int j = 0; j < s.attackPattern[i].size(); j++) {
      os << s.attackPattern[i][j];
      if (j != s.attackPattern[i].size() - 1) {
        os << ",";
      } else {
        os << "]";
      }
    }
    if (i != s.attackPattern.size() - 1) {
      os << ",";
    } else {
      os << "]";
    }
  }
  os << ",health=" << s.health << ",speed=" << s.speed << ")";
  return os;
}
