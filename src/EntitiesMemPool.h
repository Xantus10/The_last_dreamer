#pragma once
#include <tuple>
#include "Components.h"
#include <iostream>

#define MAX_ENTITIES 10000

//enum with all tag values an entity can have
enum EntityTag {
  //Default tag (Proper entity should not have DEFAULT tag)
  EDEFAULT,
  //Tag indicating a player
  EPLAYER,
  //Tag for all enemies
  EENEMY,
  //Tag for sword
  ESWORD,
  //Tag for tiles
  ETILE,
  //Tag for pickupable items (rings, keys)
  EITEM,
  //Tag for blocks
  EBLOCK,
  //Tag for nightleaks
  ENIGHTLEAK
};

typedef std::tuple<
  std::vector<CTransform>,
  std::vector<CAABB>,
  std::vector<CAnimation>,
  std::vector<CLifespan>,
  std::vector<CInput>,
  std::vector<CDamage>,
  std::vector<CInvincibility>,
  std::vector<CHP>,
  std::vector<CFollowAI>,
  std::vector<CPatrolAI>
> componentsVectorTuple;

class EntitiesMemPool {
  // Pool of all components
  componentsVectorTuple componentspool;
  // Vec of all entity's tags
  std::vector<EntityTag> tags;
  // Vec of all entity's status (use for finding empty places)
  std::vector<bool> isAlive;
  // Private Constructor
  EntitiesMemPool(size_t maxEntities);
public:
  static EntitiesMemPool& Instance() {
    static EntitiesMemPool pool(MAX_ENTITIES);
    return pool;
  }

  template <typename T>
  bool hasComponent(size_t aEntityId) {
    return getComponent<T>(aEntityId).has;
  }

  template <typename T>
  T& getComponent(size_t aEntityId) {
    return std::get<std::vector<T>>(componentspool)[aEntityId];
  }

  template <typename T, typename... TArgs>
  T& addComponent(size_t aEntityId, TArgs&&... args) {
    auto& component = getComponent<T>(aEntityId);
    component = T(std::forward<TArgs>(args)...);
    component.has = true;
    return component;
  }

  template <typename T>
  void removeComponent(size_t aEntityId) {
    getComponent<T>(aEntityId) = T();
  }

  void stripTag(size_t aEntityId);
  void destroy(size_t aEntityId);

  size_t addEntity(EntityTag tag);

  bool alive(size_t aEntityId);

  EntityTag tag(size_t aEntityId);
};
