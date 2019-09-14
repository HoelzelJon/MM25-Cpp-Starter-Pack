#include "../api.h"
#include "json.hpp"

using json = nlohmann::json;

// Method to tell json to convert Direction enum into string.
NLOHMANN_JSON_SERIALIZE_ENUM(MechMania::Direction,
                             {{MechMania::Direction::UP, "UP"},
                              {MechMania::Direction::DOWN, "DOWN"},
                              {MechMania::Direction::LEFT, "LEFT"},
                              {MechMania::Direction::RIGHT, "RIGHT"}})

// Method to tell json to convert TileType enum into string.
NLOHMANN_JSON_SERIALIZE_ENUM(
    MechMania::TileType,
    {
        {MechMania::TileType::BLANK, "BLANK"},
        {MechMania::TileType::DESTRUCTABLE, "DESTRUCTABLE"},
        {MechMania::TileType::INDESTRUCTABLE, "INDESTRUCTABLE"},
    })

void MechMania::to_json(json &j, const Position &p) {
  j = json{{"x", p.x}, {"y", p.y}};
}

/**
 * Deserialize json into Position.
 */
void MechMania::from_json(const json &j, Position &p) {
  j.at("x").get_to(p.x);
  j.at("y").get_to(p.y);
}

/**
 * Serialize Unit into json.
 */
void MechMania::to_json(json &j, const Unit &u) {
  j = json{{"hp", u.hp},           {"speed", u.speed}, {"attack", u.attack},
           {"isAlive", u.isAlive}, {"id", u.id},       {"pos", u.pos}};
}

/**
 * Deserialize json into Unit.
 */
void MechMania::from_json(const json &j, Unit &u) {
  j.at("hp").get_to(u.hp);
  j.at("speed").get_to(u.speed);
  j.at("attack").get_to(u.attack);
  j.at("isAlive").get_to(u.isAlive);
  j.at("id").get_to(u.id);
  j.at("pos").get_to(u.pos);
}

/**
 * Serialize Tile into json.
 */
void MechMania::to_json(json &j, const Tile &t) {
  j = json{{"id", t.id}, {"hp", t.hp}, {"unit", t.unit}, {"type", t.type}};
}

/**
 * Deserialize json to Tile.
 */
void MechMania::from_json(const json &j, Tile &t) {
  j.at("id").get_to(t.id);
  j.at("hp").get_to(t.hp);
  j.at("unit").get_to(t.unit);
  j.at("type").get_to(t.type);
}

/**
 * Serialize json into UnitSetup.
 */
void MechMania::to_json(json &j, const UnitSetup &s) {
  j = json{{"attackPattern", s.attackPattern},
           {"health", s.health},
           {"speed", s.speed}};
}

/**
 * Deserialize json to UnitSetup.
 */
void MechMania::from_json(const json &j, UnitSetup &s) {
  j.at("attackPattern").get_to(s.attackPattern);
  j.at("health").get_to(s.health);
  j.at("speed").get_to(s.speed);
}

/**
 * Serialize Decision into json.
 */
void MechMania::to_json(json &j, const Decision &d) {
  j = json{{"priorities", d.priorities},
           {"movements", d.movements},
           {"attacks", d.attacks}};
}

/**
 * Deserialize json to Decision.
 */
void MechMania::from_json(const json &j, Decision &d) {
  j.at("priorities").get_to(d.priorities);
  j.at("movements").get_to(d.movements);
  j.at("attacks").get_to(d.attacks);
}

/**
 * operator<< overload to convert UnitSetup object to string.
 */
std::ostream &operator<<(std::ostream &os, const MechMania::UnitSetup &s) {
  os << "UnitSetup(attackPattern=[";
  for (int i = 0; i < (int)s.attackPattern.size(); i++) {
    os << "[";
    for (int j = 0; j < (int)s.attackPattern[i].size(); j++) {
      os << s.attackPattern[i][j];
      if (j != (int)s.attackPattern[i].size() - 1) {
        os << ",";
      } else {
        os << "]";
      }
    }
    if (i != (int)s.attackPattern.size() - 1) {
      os << ",";
    } else {
      os << "]";
    }
  }
  os << ",health=" << s.health << ",speed=" << s.speed << ")";
  return os;
}
