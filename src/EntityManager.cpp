#include "EntityManager.h"


EntityManager::EntityManager() {
  EntitiesMemPool::Instance();
}

void EntityManager::removeDeadEntities(std::vector<Entity>& vec) {
  int entitiesErased = 0;
  EntitiesMemPool& mempool = EntitiesMemPool::Instance();
  size_t size = vec.size();
  for (int i = 0; i < size; i++) {
    if (!(vec.at(i - entitiesErased).isAlive())) {
      mempool.stripTag(vec.at(i - entitiesErased).memPoolAddress); // note: calls twice as much
      vec.erase(vec.begin() + (i - entitiesErased));
      entitiesErased++;
    }
  }
}

// update the entityManager (call every game loop)
void EntityManager::update() {
  removeDeadEntities(entities);
  for (auto& item : entityMap) {
    removeDeadEntities(item.second);
  }

  for (auto entityToAdd : toAdd) {
    entities.push_back(entityToAdd);
    entityMap[entityToAdd.tag()].push_back(entityToAdd);
  }
  toAdd.clear();
}

Entity EntityManager::addEntity(const enum EntityTag tag) {
  Entity e(EntitiesMemPool::Instance().addEntity(tag));
  toAdd.push_back(e);
  return e;
}

const std::vector<Entity>& EntityManager::getEntities() {
  return entities;
}

const std::vector<Entity>& EntityManager::getEntities(const enum EntityTag tag) {
  return entityMap[tag];
}

Entity EntityManager::player() {
  return getEntities(EPLAYER)[0];
}

void EntityManager::freeAllEntities() {
  for (auto e : entities) {
    e.destroy();
  }
  update();
}
