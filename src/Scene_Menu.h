#pragma once
#include "Scene.h"
#include <fstream>
#include <windows.h>

class Scene_Menu : public Scene {
  std::vector<std::string> menuStrings;
  std::vector<sf::Text> menuTexts;
  int menuIndex = 0;
  sf::Text helpText;
  bool inSettings = false;

  void initInventory();
  void readGameSave();
  void sRender();

  void menuTextsInit();
  void settingsTextsInit();

  void sDoAction(const Action& action);
public:
  void init();
  Scene_Menu(GameEnginePointer* game, std::string path);
  void update();
};
