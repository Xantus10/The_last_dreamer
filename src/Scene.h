#pragma once
#include "EntityManager.h"
#include "Action.h"
#include "Assets.h"
#include "GameEnginePointer.h"
#include "Inventory.h"


//levelName - name (len<=5), path to level,  leveltypes - M for menu, L for levels, D for dialogues, N for night levels
struct NextLevel { std::string levelName, levelPath; char levelType; };


class Scene {
protected:
  GameEnginePointer* game; //Game object
  size_t currentFrame; //Current frame of this scene
  bool paused=false; //Is the scene paused (Can the scene even be paused)
  bool hasEnded=false; //Has this scene ended
  char id; // Id of the scene
  std::map<int, actionName> actionMap; //Map of sfml keycodes to action names
  std::string levelPath;
  std::shared_ptr<Inventory> heroInventory;
public:
  NextLevel nextLevel; // Next level information

  // Nightleak stuff (only for scene_level)
  std::vector<NextLevel> nightleakNextLevels;
  int endcode = -1; // Code for different ends of a scene (-1 - normal end, AnythingElse - end through nightleak)
  //

  virtual void update() = 0; //Update the scene (call all systems)
  virtual void sDoAction(const Action& action) = 0; //Interface for excecuting actions sent by GameEngine
  virtual void sRender() = 0; //Render all sprites to window
  virtual void init() = 0; //Initialize

  bool getHasEnded(); // get if the scene ended
  void notEnd();
  actionName getActionFromKey(int keycode); // get the action code corresponding to given keycode
  void registerAction(int aKeyCode, actionName aName); //Register new action (Map the actionName to the keyCode) (call in init of all derived scenes)
  char getId();
  void setInventory(std::shared_ptr<Inventory> aInvPtr);
  std::shared_ptr<Inventory> getInventory();
  Scene();
  Scene(GameEnginePointer* aGame, std::string aLevelPath, char aId);
};
