#include "Scene_Menu.h"

void Scene_Menu::init() {
  registerAction(sf::Keyboard::W, ACTUP);
  registerAction(sf::Keyboard::S, ACTDOWN);
  registerAction(sf::Keyboard::A, ACTLEFT);
  registerAction(sf::Keyboard::D, ACTRIGHT);
  registerAction(sf::Keyboard::Space, ACTSELECT);
  std::ifstream fin(levelPath);
  fin >> nextLevel.levelName >> nextLevel.levelPath >> nextLevel.levelType;
  menuTextsInit();
}


void Scene_Menu::menuTextsInit() {
  menuStrings.clear();
  menuTexts.clear();
  menuStrings.push_back("NEW GAME");
  menuStrings.push_back("CONTINUE");
  menuStrings.push_back("SETTINGS");
  menuStrings.push_back("QUIT");
  sf::Font& font = game->getAssets().getFont(FONTANTONREG);
  helpText = sf::Text("W - up    S - down    Space - select", font, 20);
  helpText.setFillColor(sf::Color(30, 30, 30));
  helpText.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 40);
  for (int i = 0; i < menuStrings.size(); i++) {
    sf::Text t = sf::Text(menuStrings[i], font, 40);
    t.setFillColor(sf::Color(50, 50, 50));
    t.setPosition(600, 200 + i * 90);
    menuTexts.push_back(t);
  }
}

void Scene_Menu::settingsTextsInit() {
  menuStrings.clear();
  menuTexts.clear();
  menuStrings.push_back("SOUNDS VOLUME");
  menuStrings.push_back("MUSIC VOLUME");
  menuStrings.push_back("GO BACK");
  sf::Font& font = game->getAssets().getFont(FONTANTONREG);
  helpText = sf::Text("W - up    S - down    Space - select/play    A - increase    D - decrease", font, 20);
  helpText.setFillColor(sf::Color(30, 30, 30));
  helpText.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 40);
  for (int i = 0; i < menuStrings.size(); i++) {
    sf::Text t = sf::Text(menuStrings[i], font, 40);
    t.setFillColor(sf::Color(50, 50, 50));
    t.setPosition(600, 200 + i * 90);
    menuTexts.push_back(t);
  }
}


void Scene_Menu::newGameTextsInit() {
  menuStrings.clear();
  menuTexts.clear();
  menuStrings.push_back("EASY");
  menuStrings.push_back("NORMAL");
  menuStrings.push_back("HARD");
  menuStrings.push_back("GO BACK");
  sf::Font& font = game->getAssets().getFont(FONTANTONREG);
  helpText = sf::Text("W - up    S - down    Space - select/play", font, 20);
  helpText.setFillColor(sf::Color(30, 30, 30));
  helpText.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 40);
  for (int i = 0; i < menuStrings.size(); i++) {
    sf::Text t = sf::Text(menuStrings[i], font, 40);
    t.setFillColor(sf::Color(50, 50, 50));
    t.setPosition(600, 200 + i * 90);
    menuTexts.push_back(t);
  }
}


void Scene_Menu::initInventory() {
  // As the menu is the first scene, we create inventory here
  heroInventory = std::make_shared<Inventory>();
  heroInventory->giveIntoInventory(Ring(0, 1));
  heroInventory->giveIntoInventory(Ring(2, 2));
  heroInventory->giveIntoInventory(Ring(3, 3));
  heroInventory->giveIntoInventory(Ring(4, 4));
}

void Scene_Menu::readGameSave() {
  std::ifstream saveFile("./assets/game_files/data/save.txt");

  saveFile >> nextLevel.levelPath;

  heroInventory = std::make_shared<Inventory>();
  int dmg, cur, max;
  for (int i = 0; i < INVENTORY_SIZE + 1; i++) {
    saveFile >> dmg >> cur >> max;
    heroInventory->giveIntoInventory(Ring(dmg, cur, max));
  }

  float boots;
  saveFile >> boots;
  heroInventory->addBoots(boots);
  int diff;
  saveFile >> diff;
  game->difficulty = (Difficulty)diff;
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
      if (positionInSettings == 1) {
        switch (menuIndex) {
          case 0:
            game->getAssets().getSound(SOUNDPICKUP).play();
            break;
          case 1:
            game->getAssets().getMusic(MUSICDREAM).play();
            Sleep(3000);
            game->getAssets().getMusic(MUSICDREAM).stop();
            break;
          default:
            positionInSettings = 0;
            menuTextsInit();
            break;
        }
      } else if (positionInSettings == 0) {
        switch (menuIndex) {
          case 0:
            positionInSettings = 2;
            newGameTextsInit();
            break;
          case 1:
            readGameSave();
            nextLevel.levelName = "CONT";
            nextLevel.levelType = 'L';
            hasEnded = true;
            break;
          case 2:
            positionInSettings = 1;
            settingsTextsInit();
            break;
          default:
            hasEnded = true;
            game->quit();
            break;
        }
      } else if (positionInSettings == 2) {
        if (menuIndex != 3) {
          initInventory();
          hasEnded = true;
          game->difficulty = (Difficulty)menuIndex;
          menuTextsInit();
          menuIndex = 0;
          positionInSettings = 0;
        } else {
          positionInSettings = 0;
          menuTextsInit();
        }
      }
      break;
    case ACTLEFT:
      if (positionInSettings == 1) {
        if (menuIndex == 0) {
          game->settings.soundVolume = (game->settings.soundVolume == 0) ? 0 : game->settings.soundVolume - 1;
        } else if (menuIndex == 1) {
          game->settings.musicVolume = (game->settings.musicVolume == 0) ? 0 : game->settings.musicVolume - 1;
        }
        game->updateSettings();
      }
      break;
    case ACTRIGHT:
      if (positionInSettings == 1) {
        if (menuIndex == 0) {
          game->settings.soundVolume = (game->settings.soundVolume == 10) ? 10 : game->settings.soundVolume + 1;
        }
        else if (menuIndex == 1) {
          game->settings.musicVolume = (game->settings.musicVolume == 10) ? 10 : game->settings.musicVolume + 1;
        }
        game->updateSettings();
      }
      break;
    }
  }
}


Scene_Menu::Scene_Menu(GameEnginePointer* game, std::string path)
  : Scene(game, path, 'M')
  , menuIndex(0) {
}

void Scene_Menu::sRender() {
  sf::RenderWindow& window = game->getWindow();
  window.clear();
  for (auto t : menuTexts) {
    window.draw(t);
  }
  if (positionInSettings == 1) {
    sf::Font& font = game->getAssets().getFont(FONTANTONREG);
    sf::Text vol = sf::Text(std::to_string(game->settings.soundVolume), font, 40);
    vol.setFillColor(sf::Color(30, 30, 30));
    vol.setPosition(900, 200);
    window.draw(vol);
    vol = sf::Text(std::to_string(game->settings.musicVolume), font, 40);
    vol.setFillColor(sf::Color(30, 30, 30));
    vol.setPosition(900, 290);
    window.draw(vol);
  }
  window.draw(helpText);
  window.display();
}
