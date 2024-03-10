#pragma once

#include <iostream>

class Ring {
public:
  uint16_t damage = 1;
  uint16_t maxHp = 1;
  int16_t currentHp = 1;

  Ring();
  Ring(uint16_t aDamage, uint16_t aHp);
};

