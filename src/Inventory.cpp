#include "Inventory.h"

Inventory::Inventory() {}


Ring& Inventory::getEquipped() {
  return equipped;
}

Ring& Inventory::getInventoryAtIx(int ix) {
  if (ix < 0 || ix > INVENTORY_SIZE-1) {
    ix = 0;
  }
  return inventory[ix];
}


void Inventory::giveIntoInventory(Ring aNewRing) {
  if (equipped.maxHp == 0 && equipped.damage == 0) {
    equipped = aNewRing;
    return;
  }
  int ix = getEmptySpaceIx();
  if (ix == -1) {
    std::cout << "Wrong usage of inventory! Check for empty space with getEmptySpaceIx\n";
    return;
  }
  inventory[ix] = aNewRing;
}

int Inventory::getEmptySpaceIx() {
  int ix = -1;
  for (int i = 0; i < INVENTORY_SIZE; i++) {
    if (inventory[i].maxHp == 0 && inventory[i].damage == 0) {
      ix = i;
      break;
    }
  }
  return ix;
}

void Inventory::equip(int ix) {
  if (ix < 0 || ix > INVENTORY_SIZE - 1) {
    ix = 0;
  }
  Ring tmp = equipped;
  equipped = inventory[ix];
  inventory[ix] = tmp;
}
