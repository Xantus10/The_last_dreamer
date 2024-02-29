#include "Scene_Level.h"
#include <fstream>
#include <iostream>

Scene_Level::Scene_Level(GameEnginePointer* aGame, const std::string aLevelPath)
  : Scene(aGame, aLevelPath, 'L') {
  init();
}
// Register actions and call loadLevel()
void Scene_Level::init() {
  registerAction(sf::Keyboard::W, ACTJUMP);
  registerAction(sf::Keyboard::A, ACTLEFT);
  registerAction(sf::Keyboard::D, ACTRIGHT);
  registerAction(sf::Keyboard::Space, ACTATTACK);
  registerAction(sf::Keyboard::P, ACTPAUSE);
  registerAction(sf::Keyboard::Escape, ACTQUIT);
  loadLevel();
}

void Scene_Level::loadLevel() {
  // Reload EntityManager
  entities = EntityManager();

  std::ifstream levelFile(levelPath);

  levelFile >> nextLevel.levelName >> nextLevel.levelPath >> nextLevel.levelType;

  char id = ' ';
  int aName;
  int rX, rY, x, y;
  bool mb, vb;
  float speed;
  while (levelFile >> id) {
    switch (id) {
    // Textures
    case 'T':
      Entity e = entities.addEntity(ETILE);
      levelFile >> aName >> rX >> rY >> x >> y >> mb >> vb;
      e.addComponent<CAnimation>(game->getAssets().getAnimation((animationName)aName));
      e.addComponent<CAABB>(e.getComponent<CAnimation>().animation.getSize(), mb, vb);
      e.addComponent<CTransform>(getPosition(rX, rY, x, y, e), Vec2(0, 0), 0);
      break;
    // Player
    case 'P':
      Entity e = entities.addEntity(EPLAYER);
      levelFile >> playerConfig.rX >> playerConfig.rY >> playerConfig.X >> playerConfig.Y
        >> playerConfig.BBWidth >> playerConfig.BBHeight >> playerConfig.speed >> playerConfig.Health;
      e.addComponent<CAnimation>(game->getAssets().getAnimation(ANIMHEROFACE));
      e.addComponent<CAABB>(Vec2(playerConfig.BBWidth, playerConfig.BBHeight));
      e.addComponent<CTransform>(getPosition(playerConfig.rX, playerConfig.rY, playerConfig.X, playerConfig.Y, e), Vec2(0, 0), playerConfig.speed);
      e.addComponent<CInput>();
      e.addComponent<CHP>(playerConfig.Health);
      view = game->getWindow().getDefaultView(); // change after camera implementation
      view.setCenter(e.getComponent<CTransform>().pos.x, e.getComponent<CTransform>().pos.y);
      game->getWindow().setView(view);
      break;
    // Enemy
    case 'E':
      Entity e = entities.addEntity(EENEMY);
      int HP, Dmg;
      float speed;
      levelFile >> aName >> rX >> rY >> x >> y >> HP >> Dmg >> mb >> speed;
      e.addComponent<CAnimation>(game->getAssets().getAnimation((animationName)aName));
      e.addComponent<CAABB>(e.getComponent<CAnimation>().animation.getSize());
      e.addComponent<CTransform>(getPosition(rX, rY, x, y, e), Vec2(0, 0), 0);
      if (mb) {
        CPatrolAI& pai = e.addComponent<CPatrolAI>(speed);
        int NOfPosts;
        levelFile >> NOfPosts;
        int PX, PY;
        for (int i = 0; i < NOfPosts; i++) {
          levelFile >> PX >> PY;
          pai.patrolPositions.push_back(getPosition(rX, rY, PX, PY, e));
        }
      } else {
        e.addComponent<CFollowAI>(e.getComponent<CTransform>().pos, speed);
      }
      break;
    }
  }
}

Vec2 Scene_Level::getPosition(int aRoomX, int aRoomY, int aEntityX, int aEntityY, Entity e) const {
  // X,Y = multiply room by window width and the individual pos of entity by 64 and add half size; (0,0 is in top left and level is 25 by 14 tiles)
  return Vec2(aRoomX * WINDOW_WIDTH + aEntityX * 64 + e.getComponent<CAABB>().halfSize.x, aRoomY * WINDOW_HEIGHT + aEntityY * 64 + e.getComponent<CAABB>().halfSize.x);
}

void Scene_Level::spawnSword(Entity e) {
  Entity sword = entities.addEntity(ESWORD);

  sword.addComponent<CAnimation>(game->getAssets().getAnimation(ANIMSWORD));
  sword.addComponent<CAABB>(sword.getComponent<CAnimation>().animation.getSize());
  sword.addComponent<CTransform>(/*Based on player scale add pos here */);
  sword.addComponent<CLifespan>(40); // Maybe remove
  sword.addComponent<CDamage>(1);

  sword.getComponent<CAnimation>().animation.getSprite().setScale(1, 1);//Set correct scale from player
}

void Scene_Level::sAnimation() {
  for (auto e : entities.getEntities()) {
    // If entity has animation
    if (e.hasComponent<CAnimation>()) {
      CAnimation& eAnimation = e.getComponent<CAnimation>();
      // Update the animation
      eAnimation.animation.update();
      // If the animation is destroy, the entity should get destroyed on animation end
      if ((eAnimation.animation.getName() == ANIMENEMYDESTROY) && eAnimation.animation.hasEnded()) {
        e.destroy();
      }
    }
  }

  // Timer for player attack animation is handeled here (Use CInput.attack for info on attacking)
  if (entities.player().getComponent<CInput>().attack) {
    if (entities.player().getComponent<CAnimation>().animation.getTotalFramesOfAnimationRan() > 18) {
      entities.player().getComponent<CInput>().attack = false;
    }
  }
}


void Scene_Level::sMovement() {
  // Update positions of enemies
  for (auto e : entities.getEntities(EENEMY)) {
    auto& transform = e.getComponent<CTransform>();
    if (e.hasComponent<CFollowAI>()) {
      /*FOLLOW AI mechanic*/
    } else {
      /*PATROL AI mechanic (just make vector to next PPoint and normalize it to speed)*/
    }
  }

  auto& playerTransform = entities.player().getComponent<CTransform>();
  auto& playerInput = entities.player().getComponent<CInput>();
  auto& playerAnimation = entities.player().getComponent<CAnimation>();
  playerTransform.previousPos = playerTransform.pos;
  
  if (playerTransform.velocity == Vec2(0, 0)) {
    if (playerInput.left != playerInput.right) {
      if (playerInput.left) {
        playerTransform.velocity.x -= playerConfig.speed;
        if (playerAnimation.animation.getName() != ANIMHEROSIDERUN) {
          playerAnimation.animation = game->getAssets().getAnimation(ANIMHEROSIDERUN);/*Finish movement system*/
        }
        playerAnimation.animation.getSprite().setScale(-1, 1);
      }
      if (playerInput.right) {
        playerTransform.velocity.x += playerConfig.speed;
        if (playerAnimation.animation.getName() != ANIMHEROSIDERUN) {
          playerAnimation.animation = game->getAssets().getAnimation(ANIMHEROSIDERUN);
        }
        playerAnimation.animation.getSprite().setScale(1, 1);
      }

    }
    else if (!playerInput.right) {

    }
  }
  
  // Finally update position
  playerTransform.pos += playerTransform.velocity;

  // After updating position update sword pos
  if (playerInput.attack) {

  }
}

void Scene_Level::sLifespan() {
  for (auto e : entities.getEntities(ESWORD)) {
    auto& lifespanComponent = e.getComponent<CLifespan>();
    if (lifespanComponent.lifespan < 0) {
      e.destroy();
    } else {
      lifespanComponent.lifespan--;
    }
  }
}

void Scene_Level::sCollision() {
  bool wasDead = false;
  auto overlap = getOverlap(player, player);

  for (auto e : entities.getEntities(EENEMY)) {
    // Enemy + bullet
    for (auto b : entities.getEntities(EBULLET)) {
      overlap = getOverlap(e, b);
      if (overlap.x > 0 && overlap.y > 0) {
        e->getComponent<CAnimation>().animation = game->getAssets().getAnimation(ANIMENEMYMUSHROOMKILL);
        e->getComponent<CTransform>().velocity = Vec2(0, 0);
        e->removeComponent<CAABB>();
        b->destroy();
      }
    }
    // Enemy + tile
    for (auto t : entities.getEntities(ETILE)) {
      overlap = getOverlap(e, t);
      if (overlap.x > 0 && overlap.y > 0) {
        // We are colliding from right
        if (e->getComponent<CTransform>().previousPos.x > t->getComponent<CTransform>().pos.x) {
          e->getComponent<CTransform>().pos.x += overlap.x;
          e->getComponent<CAnimation>().animation.getSprite().setScale(1, 1);
        } else { // We are colliding from left
          e->getComponent<CTransform>().pos.x -= overlap.x;
          e->getComponent<CAnimation>().animation.getSprite().setScale(-1, 1);
        }
        e->getComponent<CTransform>().velocity.invertX();
      }
    }
    // Player + enemy
    overlap = getOverlap(e, player);
    if (overlap.x > 2 && overlap.y > 2) {
      wasDead = true;
    }
  }
  // Do boundaries stop at left and reload level on bottom
  // Boundaries left
  if (player->getComponent<CTransform>().pos.x < player->getComponent<CAnimation>().animation.getSize().x / 2) {
    player->getComponent<CTransform>().pos.x = player->getComponent<CAnimation>().animation.getSize().x / 2;
  }
  // Boundaries bottom
  if (player->getComponent<CTransform>().pos.y > game->getWindow().getSize().y - player->getComponent<CAnimation>().animation.getSize().y / 2) {
    wasDead = true;
  }

  // Do player + ETILE
  for (auto t : entities.getEntities(ETILE)) {
    overlap = getOverlap(player, t);
    auto prevOverlap = getPreviousOverlap(player, t);
    // If we are colliding
    if (overlap.x > 0 && overlap.y > 0) {
      //Finish collision
      if (t->getComponent<CAnimation>().animation.getName() == ANIMFINISH) {
        hasEnded = true;
      } else {
        // We are coming from left or right
        if (prevOverlap.y > 0) {
          // Left
          if (player->getComponent<CTransform>().previousPos.x > t->getComponent<CTransform>().pos.x) {
            player->getComponent<CTransform>().pos.x += overlap.x;
          } else { // Right
            player->getComponent<CTransform>().pos.x -= overlap.x;
          }
        } else { // Coming from top or bottom
          // Bottom
          if (player->getComponent<CTransform>().previousPos.y > t->getComponent<CTransform>().pos.y) {
            player->getComponent<CTransform>().pos.y += overlap.y;
            player->getComponent<CInput>().jump = false;
          } else { // Top
            player->getComponent<CTransform>().pos.y -= overlap.y;
            player->getComponent<CInput>().airborne = false;
          }
        }
      }
    }
    // Tile + bullet
    for (auto b : entities.getEntities(EBULLET)) {
      overlap = getOverlap(t, b);
      if (overlap.x > 0 && overlap.y > 0) {
        b->destroy();
        if (t->getComponent<CAnimation>().animation.getName() == ANIMWOODBLOCK) {
          t->getComponent<CAnimation>().animation = game->getAssets().getAnimation(ANIMWOODDESTROY);
        }
      }
    }
  }
  if (wasDead) {
    loadLevel(); // TODO: Add respawn(), it will destroy all EENEMY, EBULLET, EPLAYER | and recreate them, BUT it will preserve the map layout, also add EDESTROYABLE, for bricks and respawn them as well
  }
}

void Scene_Level::sSetView() {
  if (cameraIsFollowMode) {
    view.setCenter(entities.player().getComponent<CTransform>().pos.x, entities.player().getComponent<CTransform>().pos.y);
  } else {
    view.setCenter((entities.player().getComponent<CTransform>().pos.x/WINDOW_WIDTH)*WINDOW_WIDTH + WINDOW_WIDTH/2,
      (entities.player().getComponent<CTransform>().pos.y / WINDOW_HEIGHT)*WINDOW_HEIGHT + WINDOW_HEIGHT/2);
  }
  
  game->getWindow().setView(view);
}

void Scene_Level::sRender() {
  auto& window = game->getWindow();
  window.clear();
  for (auto e : entities.getEntities(ETILE)) {
    if (e.hasComponent<CAnimation>()) {
      e.getComponent<CAnimation>().animation.getSprite().setPosition(e.getComponent<CTransform>().pos.x, e.getComponent<CTransform>().pos.y);
      window.draw(e.getComponent<CAnimation>().animation.getSprite());
    }
  }
  for (auto e : entities.getEntities(EENEMY)) {
    if (e.hasComponent<CAnimation>()) {
      e.getComponent<CAnimation>().animation.getSprite().setPosition(e.getComponent<CTransform>().pos.x, e.getComponent<CTransform>().pos.y);
      window.draw(e.getComponent<CAnimation>().animation.getSprite());
    }
  }
  for (auto e : entities.getEntities(ESWORD)) {
    if (e.hasComponent<CAnimation>()) {
      e.getComponent<CAnimation>().animation.getSprite().setPosition(e.getComponent<CTransform>().pos.x, e.getComponent<CTransform>().pos.y);
      window.draw(e.getComponent<CAnimation>().animation.getSprite());
    }
  }
  for (auto e : entities.getEntities(EPLAYER)) {
    if (e.hasComponent<CAnimation>()) {
      e.getComponent<CAnimation>().animation.getSprite().setPosition(e.getComponent<CTransform>().pos.x, e.getComponent<CTransform>().pos.y);
      window.draw(e.getComponent<CAnimation>().animation.getSprite());
    }
  }
  window.display();
}

void Scene_Level::sDoAction(const Action& action) {
  if (action.getActionType() == ACTSTART) {
    switch (action.getActionName()) {
    case ACTJUMP:
      if (!player->getComponent<CInput>().airborne) {
        player->getComponent<CInput>().jump = true;
      }
      break;
    case ACTLEFT:
      player->getComponent<CInput>().left = true;
      break;
    case ACTRIGHT:
      player->getComponent<CInput>().right = true;
      break;
    case ACTSHOOT:
      if (!player->getComponent<CInput>().attack && (currentFrame - lastAttackFrame) > 60) {
        player->getComponent<CInput>().attack = true;
        lastAttackFrame = currentFrame;
        spawnBullet(player);
        auto prevAnimationScale = player->getComponent<CAnimation>().animation.getSprite().getScale();
        player->getComponent<CAnimation>().animation = game->getAssets().getAnimation(ANIMMCSHOOT);
        player->getComponent<CAnimation>().animation.getSprite().setScale(prevAnimationScale);
      }
      break;

    case ACTQUIT:
      hasEnded = true;
      nextLevel.levelName = "Menu";
      nextLevel.levelPath = "";
      nextLevel.levelType = 'M';
      break;
    case ACTPAUSE:
      paused = !paused;
      break;
    }
  } else {
    switch (action.getActionName()) {
    case ACTJUMP:
      player->getComponent<CInput>().jump = false;
      break;
    case ACTLEFT:
      player->getComponent<CInput>().left = false; /*make animation change to standing here*/
      break;
    case ACTRIGHT:
      player->getComponent<CInput>().right = false;
      break;
    }
  }
}

void sDebug() {} // only maybe implement

void Scene_Level::update() {
  entities.update(); // First of all update the entities (remove and add)
  if (!paused) {
    sMovement(); // Call movement first
    sCollision(); // After movement call collisions to resolve any overlaps
    sSetView(); // After position has been resolved make the camera close in on player
    sAnimation(); // Then call animations (Because of player attack animation)
    sLifespan(); // Then update the lifespan of all entities (honestly doesnt matter where)
  }
  sRender(); // After everything render
  currentFrame++;
}
