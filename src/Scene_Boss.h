#pragma once
#include "Scene_Night.h"


class Scene_Boss : public Scene {
  EntityManager entities = EntityManager(); // Scene EntityManager
  Assets bossAssets;
  PlayerConfigNight playerConfig;
  sf::View view;
  bool nightmare = false;
  Entity player = entities.addEntity(EDEFAULT);

  void loadLevel(); // (Re)Load level
  void loadAssets();


  Vec2 gridCoordToXY(float gridX, float gridY, Entity e); // Return the x, y pos of entity bysed on grid pos
  void spawnSword(Entity e); // Spawn sword at Entity's position
  void sAnimation(); // Animation system
  void sMovement(); // Movement system
  void sCollision(); // Collision detection and resolution system
  void sRender(); // Rendering system
  void sDoAction(const Action& action);
  void sSetView(); // Camera system
  void sInvincibility(); // IFrames system
public:
  void init(); // Initialize
  Scene_Boss(GameEnginePointer* aGame, const std::string aLevelPath);
  void update();
};
