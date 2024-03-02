#include "GameEngine.h"
/*
#include "EntityManager.h"
#include <iostream>
*/
int main(int argc, char** argv) {
  GameEngine g;
  g.run();
  /*EntityManager em;
  Entity e = em.addEntity(ETILE);
  std::cout << "Entity - isAlive: " << e.isAlive() << " id: " << e.id() << " tag: " << e.tag() << "\n";
  e.addComponent<CTransform>(Vec2(10, 10), Vec2(-1, -1), 12);
  std::cout << "Entity - isAlive: " << e.isAlive() << " id: " << e.id() << " tag: " << e.tag() << " pos: " << e.getComponent<CTransform>().pos.x << "\n";*/
  return 0;
}
