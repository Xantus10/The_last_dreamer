#pragma once
#include "Scene.h"
#include <fstream>

class Scene_Menu : public Scene {
  std::vector<std::string> menuStrings;
  std::vector<sf::Text> menuTexts;
  int menuIndex;

  void init();
  void initInventory();
  void update();

  void sDoAction(const Action& action);
public:
  Scene_Menu(GameEnginePointer* game, std::string path);
  void sRender();
};
