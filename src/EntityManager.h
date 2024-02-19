#pragma once

#include "Entity.h"
#include <vector>
#include <map>


typedef std::map<enum EntityTag, std::vector<Entity>> EntityMap;

class EntityManager {
  // Entities created so far
  size_t entitiesCreated = 0;
  std::vector<Entity> entities;
  EntityMap entityMap; // Map of entityTags to EntityVecs of pointers (trade space for functionality)
  std::vector<Entity> toAdd;
  void removeDeadEntities(std::vector<Entity>& vec);
public:
  EntityManager();
  void update(); // update the entityManager (call every game loop)
  Entity addEntity(const enum EntityTag tag);
  const std::vector<Entity>& getEntities();
  const std::vector<Entity>& getEntities(const enum EntityTag tag);
};
