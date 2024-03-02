#pragma once

#include <memory>
#include "EntitiesMemPool.h"


//Entity class (Can only be created by EntityManager), holds Entity address in mempool
class Entity {
  //All Entities will be handled ONLY through EntityManager
  friend class EntityManager;

  //Unique ID of an entity
  size_t eid = 0;
  //Index of cell in mempool // EVENTUALLY MAKE ONE VALUE and maybe decrease size
  size_t memPoolAddress;

  //Private constructor
  Entity(const size_t aId);
  void setMemPoolAddress(size_t addr);
public:
  //Templated Components Functions

  // Add a component addComponent<ComponentClass>(Args for ComponentClass constructor)
  template <typename T, typename... TArgs>
  T& addComponent(TArgs&&... args) {
    return EntitiesMemPool::Instance().addComponent<T>(memPoolAddress, std::forward<TArgs>(args)...);
  }

  // Returns true if entity has certain component
  template <typename T>
  bool hasComponent() {
    return EntitiesMemPool::Instance().hasComponent<T>(memPoolAddress);
  }

  // Return REFERENCE to component (do auto&)
  template <typename T>
  T& getComponent() {
    return EntitiesMemPool::Instance().getComponent<T>(memPoolAddress);
  }

  // Remove a component from entity
  template <typename T>
  void removeComponent() {
    EntitiesMemPool::Instance().removeComponent<T>(memPoolAddress);
  }

  //Get the value of r_alive
  bool isAlive() const;
  //Get the value of r_tag
  const enum EntityTag tag() const;
  //Get the value of r_id
  const size_t id() const;
  //Destroy the entity at the next update() EntityManager call
  void destroy();
};
