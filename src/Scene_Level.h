#pragma once
#include "Scene.h"
#include "Physics.h"

struct PlayerConfig { int rX, rY, X, Y, BBWidth, BBHeight, Health; float speed; };

class Scene_Level : public Scene {
  EntityManager entities = EntityManager(); //Scene's EntityManager
  PlayerConfig playerConfig;
  sf::View view;
  size_t lastAttackFrame = 0;
  bool cameraIsFollowMode = false;
  Entity player = entities.addEntity(EDEFAULT);

  void loadLevel(); // (Re)Load level

  Vec2 getPosition(int aRoomX, int aRoomY, int aEntityX, int aEntityY, Entity e) const; // Return the x, y pos of middle of entity
  Vec2 getCenterOfScreen();
  void spawnSword(Entity e); // Spawn sword at Entity's position
  void sMovement(); // Movement system
  void sCollision(); // Collision detection and resolution system
  void sAnimation(); // Animation system
  void sRender(); // Rendering system
  void sLifespan(); // Lifespan system
  void sInvincibility(); // Invincibility system
  void sSetView(); // System for setting the right view
  void sInventory(); // System for showing inventory (only when paused)
  void sDoAction(const Action& action);
public:
  void init(); // Initialize
  Scene_Level(GameEnginePointer* aGame, const std::string aLevelPath);
  void update();
};
