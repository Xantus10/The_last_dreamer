#include "Entity.h"


Entity::Entity(const size_t aAddress)
  : memPoolAddress(aAddress) {}

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
  return memPoolAddress;
}

//Destroy the entity at the next update() EntityManager call
void Entity::destroy() {
  return EntitiesMemPool::Instance().destroy(memPoolAddress);
}
