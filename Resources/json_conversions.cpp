#include "api.h"
#include "json.hpp"

using json = nlohmann::json;

namespace MechMania {

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
}


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
           {"id", u.id},       {"pos", u.pos}};
}

/**
 * Deserialize json into Unit.
 */
void MechMania::from_json(const json &j, Unit &u) {
  j.at("hp").get_to(u.hp);
  j.at("speed").get_to(u.speed);
  j.at("attack").get_to(u.attack);
  j.at("id").get_to(u.id);
  j.at("pos").get_to(u.pos);
}

/**
 * Serialize Tile into json.
 */
void MechMania::to_json(json &j, const Tile &t) {
  j = json{{"id", t.id}, {"hp", t.hp}, {"type", t.type}};
}

/**
 * Deserialize json to Tile.
 */
void MechMania::from_json(const json &j, Tile &t) {
  j.at("id").get_to(t.id);
  j.at("hp").get_to(t.hp);
  j.at("type").get_to(t.type);
}

/**
 * Serialize json into UnitSetup.
 */
void MechMania::to_json(json &j, const UnitSetup &s) {
  j = json{{"attackPattern", s.attackPattern},
           {"terrainPattern", s.terrainCreation},
           {"health", s.health},
           {"speed", s.speed},
           {"unitId", s.unitId}};
}

/**
 * Deserialize json to UnitSetup.
 */
void MechMania::from_json(const json &j, UnitSetup &s) {
  j.at("attackPattern").get_to(s.attackPattern);
  j.at("terrainPattern").get_to(s.terrainCreation);
  j.at("health").get_to(s.health);
  j.at("speed").get_to(s.speed);
  j.at("unitId").get_to(s.unitId);
}

/**
 * Serialize UnitDecision into json.
 */
void MechMania::to_json(json &j, const UnitDecision &d) {
  j = json{{"priority", d.priority},
           {"movement", d.movement},
           {"attack", d.attack},
           {"unitId", d.unitId}};
}

/**
 * Deserialize json to UnitDecision.
 */
void MechMania::from_json(const json &j, UnitDecision &d) {
  j.at("priority").get_to(d.priority);
  j.at("movement").get_to(d.movement);
  j.at("attack").get_to(d.attack);
  j.at("unitId").get_to(d.unitId);
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
