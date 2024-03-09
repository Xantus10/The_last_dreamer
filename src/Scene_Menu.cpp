#include "Scene_Menu.h"

void Scene_Menu::init() {
  registerAction(sf::Keyboard::W, ACTUP);
  registerAction(sf::Keyboard::S, ACTDOWN);
  registerAction(sf::Keyboard::Space, ACTSELECT);
  std::ifstream fin(levelPath);
  fin >> nextLevel.levelName >> nextLevel.levelPath >> nextLevel.levelType;
  menuStrings.push_back("NEW GAME");
  menuStrings.push_back("CONTINUE");
  menuStrings.push_back("QUIT");
  sf::Font& font = game->getAssets().getFont(FONTANTONREG);
  for (int i = 0; i < menuStrings.size(); i++) {
    sf::Text t = sf::Text(menuStrings[i], font, 40);
    t.setFillColor(sf::Color(30, 30, 30));
    t.setPosition(600, 200+i*90);
    menuTexts.push_back(t);
  }
}

void Scene_Menu::update() {
  game->getWindow().setView(game->getWindow().getDefaultView());
  if (menuIndex < 0) {
    menuIndex = menuStrings.size() - 1;
  } else if (menuIndex >= menuStrings.size()) {
    menuIndex = 0;
  }
  for (auto& t : menuTexts) {
    t.setFillColor(sf::Color(30, 30, 30));
  }
  menuTexts[menuIndex].setFillColor(sf::Color(50, 10, 240));
  sRender();
}

void Scene_Menu::sDoAction(const Action& action) {
  if (action.getActionType() == ACTSTART) {
    switch (action.getActionName()) {
    case ACTUP:
      menuIndex -= 1;
      break;
    case ACTDOWN:
      menuIndex += 1;
      break;
    case ACTSELECT:
      if (menuIndex == 0) {
        hasEnded = true;
      } else if (menuIndex == 1) {
        // After save states read save file into nextLevel
      } else {
        hasEnded = true;
        game->quit();
      }
      break;
    }
  }
}


Scene_Menu::Scene_Menu(GameEnginePointer* game, std::string path)
  : Scene(game, path, 'M')
  , menuIndex(0) {
  init();
}

void Scene_Menu::sRender() {
  game->getWindow().clear();
  for (auto t : menuTexts) {
    game->getWindow().draw(t);
  }
  game->getWindow().display();
}
