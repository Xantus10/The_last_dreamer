#include "Ring.h"

Ring::Ring()
  : damage(0)
  , maxHp(0)
  , currentHp(0) {}


Ring::Ring(uint16_t aDamage, uint16_t aHp)
  : damage(aDamage)
  , maxHp(aHp)
  , currentHp(aHp) {}
