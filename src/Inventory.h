#pragma once

#include "Ring.h"
#define INVENTORY_SIZE 4

class Inventory {
  Ring equipped;
  Ring inventory[INVENTORY_SIZE];

public:
  Inventory();

  Ring& getEquipped();
  Ring& getInventoryAtIx(int ix);

  void giveIntoInventory(Ring aNewRing);
  int getEmptySpaceIx();
  void equip(int ix);
};

