#pragma once

#include "Ring.h"
#define INVENTORY_SIZE 4

class Inventory {
  float bootsspd;
  Ring equipped;
  Ring inventory[INVENTORY_SIZE];

public:
  Inventory();

  Ring& getEquipped();
  Ring& getInventoryAtIx(int ix);

  int getBootsLvl();
  float getBootsSpd();
  void addBoots(float val);

  void giveIntoInventory(Ring aNewRing);
  int getEmptySpaceIx();
  void equip(int ix);
};
