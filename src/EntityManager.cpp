#include "EntityManager.h"


EntityManager::EntityManager() {
  EntitiesMemPool::Instance();
}

void EntityManager::removeDeadEntities(std::vector<Entity>& vec) {
  int entitiesErased = 0;
  EntitiesMemPool& mempool = EntitiesMemPool::Instance();
  for (int i = 0; i < vec.size(); i++) {
    if (!(vec.at(i).isAlive())) {
      mempool.stripTag(vec.at(i).memPoolAddress); // note: calls twice as much
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
  Entity e(entitiesCreated++);
  toAdd.push_back(e);
  e.setMemPoolAddress(EntitiesMemPool::Instance().addEntity(tag));
  return e;
}

const std::vector<Entity>& EntityManager::getEntities() {
  return entities;
}

const std::vector<Entity>& EntityManager::getEntities(const enum EntityTag tag) {
  return entityMap[tag];
}

inline Entity EntityManager::player() {
  return getEntities(EPLAYER)[0];
}
