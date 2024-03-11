#include "Ring.h"

Ring::Ring()
  : damage(0)
  , maxHp(0)
  , currentHp(0) {}


Ring::Ring(uint16_t aDamage, uint16_t aHp)
  : damage(aDamage)
  , maxHp(aHp)
  , currentHp(aHp) {}


std::string Ring::getPrintableInfo() {
  return "ATK: " + std::to_string(damage) + " HP: " + std::to_string(currentHp) +
    "/" + std::to_string(maxHp);
}
