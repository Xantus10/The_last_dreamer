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

Scene::Scene(GameEnginePointer* aGame, std::string aLevelPath, char aId) 
  : game(aGame)
  , levelPath(aLevelPath)
  , id(aId) {}

Scene::Scene() {}
