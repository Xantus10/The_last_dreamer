#include "Scene.h"
#include <iostream>



bool Scene::getHasEnded() {
  return hasEnded;
}

void Scene::registerAction(int aKeyCode, actionName aName) {
  actionMap[aKeyCode] = aName;
}

actionName Scene::getActionFromKey(int keycode) {
  if (actionMap.find(keycode) == actionMap.end()) {
    return ACTNOOPERATION;
  } else {
    return actionMap[keycode];
  }
}

char Scene::getId() {
  return id;
}

void Scene::setInventory(std::shared_ptr<Inventory> aInvPtr) {
  heroInventory = aInvPtr;
}

std::shared_ptr<Inventory> Scene::getInventory() {
  return heroInventory;
}

void Scene::notEnd() {
  hasEnded = false;
}

Scene::Scene(GameEnginePointer* aGame, std::string aLevelPath, char aId) 
  : game(aGame)
  , levelPath(aLevelPath)
  , id(aId) {}

Scene::Scene() {}
