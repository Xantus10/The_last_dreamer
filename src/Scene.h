#pragma once
#include "EntityManager.h"
#include "Action.h"
#include "Assets.h"
#include "GameEnginePointer.h"


// leveltypes - M for menu, L for levels, I for images, D for dialogues
struct NextLevel { std::string levelName, levelPath; char levelType; };

class Scene {
protected:
  GameEnginePointer* game; //Game object
  EntityManager entities; //Scene's EntityManager
  size_t currentFrame; //Current frame of this scene
  bool paused=false; //Is the scene paused (Can the scene even be paused)
  bool hasEnded=false; //Has this scene ended
  char id; // Id of the scene
  std::map<int, actionName> actionMap; //Map of sfml keycodes to action names
  std::string levelPath;
public:
  NextLevel nextLevel; // Next level information

  virtual void update() = 0; //Update the scene (call all systems)
  virtual void sDoAction(const Action& action) = 0; //The type of argument could change //Interface for excecuting actions sent by GameEngine
  virtual void sRender() = 0; //Render all sprites to window

  bool getHasEnded(); // get if the scene ended
  actionName getActionFromKey(int keycode); // get the action code corresponding to given keycode
  void registerAction(int aKeyCode, actionName aName); // ARg type could change //Register new action (Map the actionName to the keyCode) (call in init of all derived scenes)
  char getId();
  Scene();
  Scene(GameEnginePointer* aGame, std::string aLevelPath, char aId);
};
