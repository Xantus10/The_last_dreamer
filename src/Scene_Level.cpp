#include "Scene_Level.h"
#include <fstream>
#include <iostream>

Scene_Level::Scene_Level(GameEnginePointer* aGame, const std::string aLevelPath)
  : Scene(aGame, aLevelPath, 'L') {
  init();
}
// Register actions and call loadLevel()
void Scene_Level::init() {
  registerAction(sf::Keyboard::W, ACTUP);
  registerAction(sf::Keyboard::A, ACTLEFT);
  registerAction(sf::Keyboard::D, ACTRIGHT);
  registerAction(sf::Keyboard::S, ACTDOWN);
  registerAction(sf::Keyboard::Space, ACTATTACK);
  registerAction(sf::Keyboard::P, ACTPAUSE);
  registerAction(sf::Keyboard::Escape, ACTQUIT);
  registerAction(sf::Keyboard::T, ACTSELECT);
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
    {
      Entity e = entities.addEntity(ETILE);
      levelFile >> aName >> rX >> rY >> x >> y >> mb >> vb;
      e.addComponent<CAnimation>(game->getAssets().getAnimation((animationName)aName));
      e.addComponent<CAABB>(e.getComponent<CAnimation>().animation.getSize(), mb, vb);
      e.addComponent<CTransform>(getPosition(rX, rY, x, y, e), Vec2(0, 0), 0);
      //std::cout << "Added new tile " << e.getComponent<CAnimation>().animation.getName() << "|" << e.getComponent<CAABB>().size.x << " POS: " << e.getComponent<CTransform>().pos.x << " " << e.getComponent<CTransform>().pos.y << "\n";
      break;
    }
      // Player
    case 'P':
    {
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
    }
      // Enemy
    case 'E':
    {
      Entity e = entities.addEntity(EENEMY);
      int HP, Dmg;
      float speed;
      levelFile >> aName >> rX >> rY >> x >> y >> HP >> Dmg >> mb >> speed;
      e.addComponent<CAnimation>(game->getAssets().getAnimation((animationName)aName));
      e.addComponent<CAABB>(e.getComponent<CAnimation>().animation.getSize());
      e.addComponent<CTransform>(getPosition(rX, rY, x, y, e), Vec2(0, 0), 0);
      e.addComponent<CHP>(HP);
      e.addComponent<CDamage>(Dmg);
      if (mb) {
        CPatrolAI& pai = e.addComponent<CPatrolAI>(speed);
        int NOfPosts;
        levelFile >> NOfPosts;
        int PX, PY;
        for (int i = 0; i < NOfPosts; i++) {
          levelFile >> PX >> PY;
          pai.patrolPositions.push_back(getPosition(rX, rY, PX, PY, e));
        }
      }
      else {
        e.addComponent<CFollowAI>(e.getComponent<CTransform>().pos, speed);
      }
      break;
    }
    }
  }
  entities.update();
}

Vec2 Scene_Level::getPosition(int aRoomX, int aRoomY, int aEntityX, int aEntityY, Entity e) const {
  // X,Y = multiply room by window width and the individual pos of entity by 64 and add half size; (0,0 is in top left and level is 25 by 14 tiles)
  return Vec2(aRoomX * WINDOW_WIDTH + aEntityX * 64 + e.getComponent<CAABB>().halfSize.x, aRoomY * WINDOW_HEIGHT + aEntityY * 64 + e.getComponent<CAABB>().halfSize.x);
}

char animationNameToDir(animationName n, float scalex) {
  if (n == ANIMHEROBACK || n == ANIMHEROBACKRUN) {
    return 'w';
  } else if (n == ANIMHEROFACE || n == ANIMHEROFACERUN) {
    return 's';
  } else if (n == ANIMHEROSIDE || n == ANIMHEROSIDERUN) {
    return (scalex < 0) ? 'a' : 'd';
  }
}


void Scene_Level::spawnSword(Entity e) {
  Entity sword = entities.addEntity(ESWORD);

  char playerDir = animationNameToDir(e.getComponent<CAnimation>().animation.getName(), e.getComponent<CAnimation>().animation.getSprite().getScale().x);

  sword.addComponent<CAnimation>(game->getAssets().getAnimation(ANIMSWORD));
  sword.addComponent<CAABB>(sword.getComponent<CAnimation>().animation.getSize());
  Vec2 posShift;
  float rot = 0;
  switch (playerDir) {
    case 'w':
      posShift = Vec2(0, -54);
      break;
    case 's':
      posShift = Vec2(0, 64);
      rot = 180;
      break;
    case 'a':
      posShift = Vec2(-44, 0);
      rot = 270;
      break;
    case 'd':
      posShift = Vec2(44, 0);
      rot = 90;
      break;
  }
  sword.addComponent<CTransform>(e.getComponent<CTransform>().pos + posShift, Vec2(0, 0), 0);
  sword.addComponent<CLifespan>(20); // Maybe remove
  sword.addComponent<CDamage>(1);

  sword.getComponent<CAnimation>().animation.getSprite().setRotation(rot);
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


char velocityToDir(Vec2& vel) {
  if (vel.x > 0) {
    return 'd';
  } else if (vel.x < 0) {
    return 'a';
  } else if (vel.y < 0) {
    return 'w';
  }
  return 's';
}


void Scene_Level::sMovement() {
  // Update positions of enemies
  for (auto e : entities.getEntities(EENEMY)) {
    auto& transform = e.getComponent<CTransform>();
    if (e.hasComponent<CFollowAI>()) {
      Vec2 path(1, 1);
      //FOLLOW AI mechanic
      for (auto et : entities.getEntities(ETILE)) {
        if (et.getComponent<CAABB>().blockVision) {
          if (entityIntersect(transform.pos, entities.player().getComponent<CTransform>().pos, et)) {
            path = Vec2(0, 0);
          }
        }
      }
      if (path == Vec2(1, 1)) {
        path = entities.player().getComponent<CTransform>().pos - transform.pos;
        path.normalize();
        path *= e.getComponent<CFollowAI>().speed;
        transform.velocity = path;
      }
    } else {
      //PATROL AI mechanic (just make vector to next PPoint and normalize it to speed)
      Vec2 direction = e.getComponent<CPatrolAI>().nextPosition() - transform.pos;
      direction.normalize();
      direction *= e.getComponent<CPatrolAI>().speed;
      transform.velocity = direction;
    }
    transform.previousPos = transform.pos;
    transform.pos += transform.velocity;
  }

  auto& playerTransform = entities.player().getComponent<CTransform>();
  auto& playerInput = entities.player().getComponent<CInput>();
  auto& playerAnimation = entities.player().getComponent<CAnimation>();
  playerTransform.previousPos = playerTransform.pos;
  
  if (playerTransform.velocity == Vec2(0, 0)) {
    if (playerInput.left != playerInput.right) {
      playerAnimation.animation = game->getAssets().getAnimation(ANIMHEROSIDERUN);
      if (playerInput.left) {
        playerTransform.velocity.x -= playerConfig.speed;
        playerAnimation.animation.getSprite().setScale(-1, 1);
      } else {
        playerTransform.velocity.x += playerConfig.speed;
        playerAnimation.animation.getSprite().setScale(1, 1);
      }
    }
    else {
      if (playerInput.up != playerInput.down) {
        if (playerInput.up) {
          playerTransform.velocity.y -= playerConfig.speed;
          playerAnimation.animation = game->getAssets().getAnimation(ANIMHEROBACKRUN);
        }
        else {
          playerTransform.velocity.y += playerConfig.speed;
          playerAnimation.animation = game->getAssets().getAnimation(ANIMHEROFACERUN);
        }
      }
    }
  } else {
    switch (velocityToDir(playerTransform.velocity)) {
      case 'w':
        if (!playerInput.up) {
          playerTransform.velocity = Vec2(0, 0);
        }
        break;
      case 's':
        if (!playerInput.down) {
          playerTransform.velocity = Vec2(0, 0);
        }
        break;
      case 'a':
        if (!playerInput.left) {
          playerTransform.velocity = Vec2(0, 0);
        }
        break;
      case 'd':
        if (!playerInput.right) {
          playerTransform.velocity = Vec2(0, 0);
        }
        break;
    }
  }
  
  // Finally update position
  playerTransform.pos += playerTransform.velocity;

  // After updating position update sword pos and rotation
  for (auto e : entities.getEntities(ESWORD)) {
    char playerDir = animationNameToDir(playerAnimation.animation.getName(), playerAnimation.animation.getSprite().getScale().x);
    Vec2 posShift;
    float rot = 0;
    switch (playerDir) {
      case 'w':
        posShift = Vec2(0, -54);
        break;
      case 's':
        posShift = Vec2(0, 64);
        rot = 180;
        break;
      case 'a':
        posShift = Vec2(-44, 0);
        rot = 270;
        break;
      case 'd':
        posShift = Vec2(44, 0);
        rot = 90;
        break;
    }

    auto& eTransform = e.getComponent<CTransform>();
    eTransform.previousPos = eTransform.pos;
    eTransform.pos = playerTransform.pos + posShift;
    e.getComponent<CAnimation>().animation.getSprite().setRotation(rot);
  }
}

void Scene_Level::sLifespan() {
  for (auto e : entities.getEntities(ESWORD)) {
    auto& lifespanComponent = e.getComponent<CLifespan>();
    if (lifespanComponent.lifespan == 0) {
      e.destroy();
    } else {
      lifespanComponent.lifespan--;
    }
  }
}

void Scene_Level::sInvincibility() {
  for (auto e : entities.getEntities()) {
    if (e.hasComponent<CInvincibility>()) {
      e.getComponent<CInvincibility>().iframes--;
      if (e.getComponent<CInvincibility>().iframes == 0) {
        e.removeComponent<CInvincibility>();
      }
    }
  }
}

void Scene_Level::sCollision() {
  Vec2 overlap;

  for (auto e : entities.getEntities(EENEMY)) {
    // Enemy + sword
    for (auto s : entities.getEntities(ESWORD)) {
      overlap = getOverlap(e, s);
      if (overlap.x > 0 && overlap.y > 0) {
        if (!e.hasComponent<CInvincibility>()) {
          e.getComponent<CHP>().currentHp -= s.getComponent<CDamage>().damage;
          if (e.getComponent<CHP>().currentHp == 0) {
            e.getComponent<CAnimation>().animation = game->getAssets().getAnimation(ANIMENEMYDESTROY);
            e.removeComponent<CAABB>();
            if (e.hasComponent<CFollowAI>()) { e.getComponent<CFollowAI>().speed = 0; }
            else { e.getComponent<CPatrolAI>().speed = 0; }
          } else {
            e.addComponent<CInvincibility>(22);
          }
        }
      }
    }
    // Enemy + tile
    for (auto t : entities.getEntities(ETILE)) {
      if (t.getComponent<CAABB>().blockMovement) {
        overlap = getOverlap(e, t);
        Vec2 prevOverlap = getPreviousOverlap(e, t);
        if (overlap.x > 0 && overlap.y > 0) {
          // Up/Down
          if (prevOverlap.x > 0) {
            // From Top
            if (e.getComponent<CTransform>().pos.y < t.getComponent<CTransform>().pos.y) {
              e.getComponent<CTransform>().pos.y -= overlap.y;
            }
            else { // From bottom
              e.getComponent<CTransform>().pos.y += overlap.y;
            } // From left/right
          }
          else {
            // From left
            if (e.getComponent<CTransform>().pos.x < t.getComponent<CTransform>().pos.x) {
              e.getComponent<CTransform>().pos.x -= overlap.x;
            }
            else { // From right
              e.getComponent<CTransform>().pos.x += overlap.x;
            }
          }
        }
      }
    }
    // Player + enemy
    overlap = getOverlap(e, entities.player());
    // 2 bcs we give some breathing room for hero
    if (overlap.x > 2 && overlap.y > 2) {
      entities.player().getComponent<CHP>().currentHp -= e.getComponent<CDamage>().damage;
    }
  }
  /*
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
  }*/
  if (entities.player().getComponent<CHP>().currentHp == 0) {
    loadLevel(); // TODO: Add respawn(), it will destroy all EENEMY, EBULLET, EPLAYER | and recreate them, BUT it will preserve the map layout, also add EDESTROYABLE, for bricks and respawn them as well
  }
}

void Scene_Level::sSetView() {
  if (cameraIsFollowMode) {
    view.setCenter(entities.player().getComponent<CTransform>().pos.x, entities.player().getComponent<CTransform>().pos.y);
  } else {
    sf::Vector2f v = sf::Vector2f(((int)entities.player().getComponent<CTransform>().pos.x / (int)WINDOW_WIDTH) * WINDOW_WIDTH + WINDOW_WIDTH / 2,
      ((int)entities.player().getComponent<CTransform>().pos.y / (int)WINDOW_HEIGHT)* WINDOW_HEIGHT + WINDOW_HEIGHT / 2);
    if (entities.player().getComponent<CTransform>().pos.x < 0) {
      v.x-=WINDOW_WIDTH;
    }
    if (entities.player().getComponent<CTransform>().pos.y < 0) {
      v.y-=WINDOW_HEIGHT;
    }
    view.setCenter(v);
  }
  
  game->getWindow().setView(view);
}

void Scene_Level::sRender() {
  sf::Font font = game->getAssets().getFont(FONTINCONSOLATAREG);
  sf::Text hpText;
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
      hpText = sf::Text(std::to_string(e.getComponent<CHP>().currentHp), font, 22);
      hpText.setOrigin(hpText.getLocalBounds().width / 2.0f, hpText.getLocalBounds().height / 2.0f);
      hpText.setFillColor(sf::Color(230, 0, 0));
      hpText.setPosition(e.getComponent<CTransform>().pos.x - 2, e.getComponent<CTransform>().pos.y - 58);
      e.getComponent<CAnimation>().animation.getSprite().setPosition(e.getComponent<CTransform>().pos.x, e.getComponent<CTransform>().pos.y);
      window.draw(e.getComponent<CAnimation>().animation.getSprite());
      window.draw(hpText);
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
      hpText = sf::Text(std::to_string(e.getComponent<CHP>().currentHp), font, 22);
      hpText.setOrigin(hpText.getLocalBounds().width / 2.0f, hpText.getLocalBounds().height / 2.0f);
      hpText.setFillColor(sf::Color(230, 0, 0));
      hpText.setPosition(e.getComponent<CTransform>().pos.x-2, e.getComponent<CTransform>().pos.y-58);
      e.getComponent<CAnimation>().animation.getSprite().setPosition(e.getComponent<CTransform>().pos.x, e.getComponent<CTransform>().pos.y);
      window.draw(e.getComponent<CAnimation>().animation.getSprite());
      window.draw(hpText);
    }
  }
  window.display();
}

void Scene_Level::sDoAction(const Action& action) {
  if (action.getActionType() == ACTSTART) {
    switch (action.getActionName()) {
    case ACTUP:
      entities.player().getComponent<CInput>().up = true;
      break;
    case ACTLEFT:
      entities.player().getComponent<CInput>().left = true;
      break;
    case ACTRIGHT:
      entities.player().getComponent<CInput>().right = true;
      break;
    case ACTDOWN:
      entities.player().getComponent<CInput>().down = true;
      break;
    case ACTATTACK:
      if (!entities.player().getComponent<CInput>().attack && (currentFrame - lastAttackFrame) > 20) {
        entities.player().getComponent<CInput>().attack = true;
        lastAttackFrame = currentFrame;
        spawnSword(entities.player());
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
    case ACTSELECT:
      cameraIsFollowMode = !cameraIsFollowMode;
    }
  } else {
    auto& anim = entities.player().getComponent<CAnimation>();
    switch (action.getActionName()) {
    case ACTUP:
      entities.player().getComponent<CInput>().up = false;
      if (anim.animation.getName() == ANIMHEROBACKRUN) {
        anim.animation = game->getAssets().getAnimation(ANIMHEROBACK);
      }
      break;
    case ACTLEFT:
      entities.player().getComponent<CInput>().left = false;
      if (anim.animation.getName() == ANIMHEROSIDERUN) {
        auto scale = anim.animation.getSprite().getScale();
        if (scale.x == -1) {
          anim.animation = game->getAssets().getAnimation(ANIMHEROSIDE);
          anim.animation.getSprite().setScale(scale);
        }
      }
      break;
    case ACTRIGHT:
      entities.player().getComponent<CInput>().right = false;
      if (anim.animation.getName() == ANIMHEROSIDERUN) {
        auto scale = anim.animation.getSprite().getScale();
        if (scale.x == 1) {
          anim.animation = game->getAssets().getAnimation(ANIMHEROSIDE);
          anim.animation.getSprite().setScale(scale);
        }
      }
      break;
    case ACTDOWN:
      entities.player().getComponent<CInput>().down = false;
      if (anim.animation.getName() == ANIMHEROFACERUN) {
        anim.animation = game->getAssets().getAnimation(ANIMHEROFACE);
      }
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
    sInvincibility(); //// Then update the iframes of all entities (honestly doesnt matter where)
  }
  sRender(); // After everything render
  currentFrame++;
}
