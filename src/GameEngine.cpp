#include "GameEngine.h"
#include <fstream>
#include <iostream>

GameEngine::GameEngine() {
  window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Game"/*, sf::Style::Fullscreen */ );
  window.setFramerateLimit(60);
  std::cout << "Begin asset loading...\n";
  initAssets();
  std::cout << "Ended asset loading...\n";
  initScene();
  std::cout << "Ended menu initialization...\n";
}

void GameEngine::initAssets() {
  assets = Assets();
  std::ifstream assetsfile("./assets/assets.txt");
  char assetIndicator = ' ';
  std::string assetfilename = "";
  int textureIndex = 0;
  int frameCount = 0;
  int animationSpeed = 0;
  int texCount = 0;
  int aniCount = 0;
  int soundCount = 0;
  int musicCount = 0;
  int fontCount = 0;
  while (assetsfile >> assetIndicator) {
    switch (assetIndicator) {
    case 'T':
      assetsfile >> assetfilename;
      assets.addTexture((textureName)texCount, assetfilename);
      texCount++;
      break;
    case 'A':
      assetsfile >> textureIndex >> frameCount >> animationSpeed;
      assets.addAnimation((animationName)aniCount, Animation((animationName)aniCount, assets.getTexture((textureName)textureIndex), frameCount, animationSpeed));
      aniCount++;
      break;
    case 'S':
      assetsfile >> assetfilename;
      assets.addSound((soundName)soundCount, assetfilename);
      soundCount++;
      break;
    case 'M':
      assetsfile >> assetfilename;
      assets.addSound((soundName)musicCount, assetfilename);
      musicCount++;
      break;
    case 'F':
      assetsfile >> assetfilename;
      assets.addFont((fontName)fontCount, assetfilename);
      fontCount++;
      break;
    }
  }
}

void GameEngine::initScene() {
  scenesMap["Menu"] = std::make_shared<Scene_Menu>(this, "./assets/game_files/levels/menu.txt");
  currentScene = "Menu";
}

std::shared_ptr<Scene> GameEngine::getCurrentScene() {
  return scenesMap[currentScene];
}

// There is a null pointer problem smwhr here
void GameEngine::run() {
  std::cout << "Game starting->" << running << "\n";
  while (running) {
    sUserInput();
    scenesMap[currentScene]->update();
    if (scenesMap[currentScene]->getHasEnded()) {
      if (scenesMap[currentScene]->nextLevel.levelPath == "") {
        changeScene(scenesMap[currentScene]->nextLevel.levelName);
      } else {
        bool endCur = (scenesMap[currentScene]->getId() != 'M');
        if (scenesMap[currentScene]->nextLevel.levelType == 'L') {
         changeScene(scenesMap[currentScene]->nextLevel.levelName, std::make_shared<Scene_Level>(this, scenesMap[currentScene]->nextLevel.levelPath), endCur);
        }
        else if (scenesMap[currentScene]->nextLevel.levelType == 'D') {
         changeScene(scenesMap[currentScene]->nextLevel.levelName, std::make_shared<Scene_Dialogue>(this, scenesMap[currentScene]->nextLevel.levelPath), endCur);
        }
      }
    }
  }
}

void GameEngine::quit() {
  window.close();
  running = false;
}

void GameEngine::changeScene(std::string aSceneId, bool endCurrentScene) {
  if (endCurrentScene) scenesMap.erase(currentScene);
  currentScene = aSceneId;
  scenesMap[currentScene]->notEnd();
}

void GameEngine::changeScene(std::string aSceneId, std::shared_ptr<Scene> aScenePtr, bool endCurrentScene) {
  scenesMap[aSceneId] = aScenePtr;
  std::string tmp = aSceneId;
  if (endCurrentScene) scenesMap.erase(currentScene);
  currentScene = tmp;
}

Assets& GameEngine::getAssets() {
  return assets;
}

sf::RenderWindow& GameEngine::getWindow() {
  return window;
}

void GameEngine::sUserInput() {
  sf::Event event;
  while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      running = false;
    }

    if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
      actionName name = scenesMap[currentScene]->getActionFromKey(event.key.code);
      if (name == ACTNOOPERATION) { continue; }
      const actionType t = (event.type == sf::Event::KeyPressed) ? ACTSTART : ACTEND;
      scenesMap[currentScene]->sDoAction(Action(name, t));
    }
  }
}
