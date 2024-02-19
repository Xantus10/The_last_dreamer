#include "Entity.h"


Entity::Entity(const size_t aId)
  : eid(aId) {}

void Entity::setMemPoolAddress(size_t addr) {
  memPoolAddress = addr;
}

//Get the value of isAlive
bool Entity::isAlive() const {
  return EntitiesMemPool::Instance().alive(memPoolAddress);
}

//Get the value of tags
const enum EntityTag Entity::tag() const {
  return EntitiesMemPool::Instance().tag(memPoolAddress);
}

//Get the value of id
const size_t Entity::id() const {
  return eid;
}

//Destroy the entity at the next update() EntityManager call
void Entity::destroy() {
  return EntitiesMemPool::Instance().destroy(memPoolAddress);
}
