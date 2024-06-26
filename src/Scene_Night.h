#pragma once
#include "Scene.h"

struct PlayerConfigNight { float spawnX, spawnY, BBWidth, BBHeight, speed, jumpSpeed, maxSpeed, gravity; };

char velocityToDirAD(Vec2& vel);

class Scene_Night : public Scene {
  EntityManager entities = EntityManager(); // Scene EntityManager
  PlayerConfigNight playerConfig;
  sf::View view;
  bool nightmare = false;
  Entity player = entities.addEntity(EDEFAULT);

  void loadLevel(); // (Re)Load level

  void triggerNightmare();

  Vec2 gridCoordToXY(float gridX, float gridY,Entity e); // Return the x, y pos of entity bysed on grid pos
  void sAnimation(); // Animation system
  void sMovement(); // Movement system
  void sCollision(); // Collision detection and resolution system
  void sRender(); // Rendering system
  void sCoins(); // Coins functionality
  void sDoAction(const Action& action);
  void sSetView(); // Camera system
  void sInvincibility(); // IFrames system
public:
  void init(); // Initialize
  Scene_Night(GameEnginePointer* aGame, const std::string aLevelPath);
  void update();
};

