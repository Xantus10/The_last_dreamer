#pragma once
#include <map>
#include <string>

#include "Scene_Menu.h"
#include "Scene_Level.h"
#include "Scene_Dialogue.h"
#include "Scene_Night.h"

#include "GameEnginePointer.h"
class GameEngine : public GameEnginePointer {
  std::map<std::string, std::shared_ptr<Scene>> scenesMap; //Scene indicator should not be longer than 5 letters
  std::string currentScene; // Current scene indicator
  sf::RenderWindow window; // Game window
  Assets assets; // Assets manager
  bool running=true; // Is the game running?

  void initAssets(); // Initialize assets
  void initScene(); // initialize intro (menu) scene
  std::shared_ptr<Scene> getCurrentScene();
public:
  GameEngine();
  void run();
  void quit();
  void changeScene(std::string aSceneId, bool endCurrentScene = true);
  void changeScene(std::string aSceneId, std::shared_ptr<Scene> aScenePtr, bool endCurrentScene = true);
  Assets& getAssets();
  sf::RenderWindow& getWindow();
  void sUserInput(); //Construct action objects based on the input
};
