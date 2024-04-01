#include "Scene_Night.h"
#include <fstream>
#include <iostream>

Scene_Night::Scene_Night(GameEnginePointer* aGame, const std::string aLevelPath)
  : Scene(aGame, aLevelPath, 'L') {
  init();
}

// Register actions and call loadLevel()
void Scene_Night::init() {
  registerAction(sf::Keyboard::W, ACTJUMP);
  registerAction(sf::Keyboard::A, ACTLEFT);
  registerAction(sf::Keyboard::D, ACTRIGHT);
  registerAction(sf::Keyboard::P, ACTPAUSE);
  loadLevel();
}

Vec2 Scene_Night::gridCoordToXY(float gridX, float gridY,Entity e) {
  // Size to get half size
  Vec2& size = e.getComponent<CAABB>().halfSize;
  // X = multiply by 64 and add half size | Y is reversed (0,0 is in bottom left)
  Vec2 retVec((gridX * DEFAULT_BLOCK_SIZE) + size.x + (DEFAULT_BLOCK_SIZE/2 - size.x), (WINDOW_HEIGHT - (gridY * DEFAULT_BLOCK_SIZE)) - size.y + (DEFAULT_BLOCK_SIZE/2 - size.y));
  return retVec;
}

void Scene_Night::loadLevel() {
  // Reload EntityManager
  entities = EntityManager();
  nightmare = false;

  std::ifstream levelFile(levelPath);

  char id = ' ';
  int x, y;
  while (levelFile >> id) {
    switch (id) {
    // Player
    case 'P':
    {
      // We check if the entity is really our player (resolves problems when reloading)
      if (player.isAlive() && player.tag() == EPLAYER) {
        player.destroy();
      }
      player = entities.addEntity(EPLAYER);
      levelFile >> playerConfig.spawnX >> playerConfig.spawnY >> playerConfig.BBWidth >> playerConfig.BBHeight
        >> playerConfig.speed >> playerConfig.jumpSpeed >> playerConfig.maxSpeed >> playerConfig.gravity;
      player.addComponent<CAnimation>(game->getAssets().getAnimation(ANIMHEROSIDE));
      player.addComponent<CAABB>(Vec2(playerConfig.BBWidth, playerConfig.BBHeight));
      player.addComponent<CTransform>(gridCoordToXY(playerConfig.spawnX, playerConfig.spawnY, player), Vec2(0, 0), playerConfig.speed);
      player.addComponent<CInput>();
      view = game->getWindow().getDefaultView();
      view.setCenter(player.getComponent<CTransform>().pos.x, player.getComponent<CTransform>().pos.y-WINDOW_HEIGHT/3);
      game->getWindow().setView(view);
      break;
    }
    // Enemy
    case 'E':
    {
      Entity e = entities.addEntity(EENEMY);
      levelFile >> x >> y;
      e.addComponent<CAnimation>(game->getAssets().getAnimation(ANIMNIGHTWHEEL));
      e.addComponent<CAABB>(e.getComponent<CAnimation>().animation.getSize(), true, true);
      e.addComponent<CTransform>(gridCoordToXY(x, y, e), Vec2(0, 0), 0);
      e.removeComponent<CAABB>();
      break;
    }
    case 'C':
    {
      Entity e = entities.addEntity(EITEM);
      levelFile >> x >> y;
      e.addComponent<CAnimation>(game->getAssets().getAnimation(ANIMNIGHTCOIN));
      e.addComponent<CAABB>(e.getComponent<CAnimation>().animation.getSize(), true, true);
      e.addComponent<CTransform>(gridCoordToXY(x, y, e), Vec2(0, 0), 0);
      break;
    }
    case 'B':
    {
      Entity e = entities.addEntity(EBLOCK);
      levelFile >> x >> y;
      e.addComponent<CAnimation>(game->getAssets().getAnimation(ANIMNIGHTPLATFORM));
      e.addComponent<CAABB>(e.getComponent<CAnimation>().animation.getSize(), true, true);
      e.addComponent<CTransform>(gridCoordToXY(x, y, e), Vec2(0, 0), 0);
      break;
    }
    }
  }
  levelFile.close();
  entities.update();
}

// Use when swapping to nightmare
void Scene_Night::triggerNightmare() {
  nightmare = true;
  for (auto e : entities.getEntities(EENEMY)) {
    e.addComponent<CAABB>(e.getComponent<CAnimation>().animation.getSize(), true, true);
  }
}

void Scene_Night::sAnimation() {
  for (auto e : entities.getEntities()) {
    // If entity has animation
    if (e.hasComponent<CAnimation>()) {
      CAnimation& eAnimation = e.getComponent<CAnimation>();
      // Update the animation
      eAnimation.animation.update();
      // If the animation is destroy, the entity should get destroyed on animation end
      if ((!eAnimation.repeating) && eAnimation.animation.hasEnded()) {
        e.destroy();
      }
    }
  }
}

char velocityToDirAD(Vec2& vel) {
  if (vel.x >= 0) {
    return 'd';
  }
  return 'a';
}

void Scene_Night::sMovement() {
  auto& playerTransform = player.getComponent<CTransform>();
  auto& playerInput = player.getComponent<CInput>();
  auto& playerAnimation = player.getComponent<CAnimation>();
  playerTransform.previousPos = playerTransform.pos;

  if (playerTransform.velocity.x == 0) {
    if (playerInput.left != playerInput.right) {
      playerAnimation.animation = game->getAssets().getAnimation(ANIMHEROSIDERUN);
      if (playerInput.left) {
        playerTransform.velocity.x -= playerConfig.speed;
        playerAnimation.animation.getSprite().setScale(-1, 1);
      }
      else {
        playerTransform.velocity.x += playerConfig.speed;
        playerAnimation.animation.getSprite().setScale(1, 1);
      }
    }
  }
  else {
    switch (velocityToDirAD(playerTransform.velocity)) {
    case 'a':
      if (!playerInput.left) {
        playerTransform.velocity.x = 0;
      }
      break;
    case 'd':
      if (!playerInput.right) {
        playerTransform.velocity.x = 0;
      }
      break;
    }
  }

  // We utilize CInput.attack as airborne indicator
  if (playerInput.up && !playerInput.attack) {
    playerInput.attack = true;
    playerTransform.velocity.y -= playerConfig.jumpSpeed;
  } else if (!playerInput.up && playerInput.attack && playerTransform.velocity.y < 0) {
    playerTransform.velocity.y = 0;
  }
  playerTransform.velocity.y += playerConfig.gravity;

  // Max speed check
  if (playerTransform.velocity.y > playerConfig.maxSpeed) {
    playerTransform.velocity.y = playerConfig.maxSpeed;
  }

  // Finally update position
  playerTransform.pos += playerTransform.velocity;
}


void Scene_Night::sCollision() {
  Vec2 overlap;
  bool died = false;
  
  for (auto e : entities.getEntities(EENEMY)) {
    // Player + enemy
    overlap = getOverlap(e, player);
    // 10 bcs we give some breathing room for hero
    if (overlap.x > 10 && overlap.y > 10) {
      died = true;
    }
  }

  // Player + Items (Coin)
  for (auto i : entities.getEntities(EITEM)) {
    overlap = getOverlap(player, i);
    if (overlap.x > 0 && overlap.y > 0) {
      // If it collides with coin, then we destroy the coin
      i.destroy();
      if (!nightmare) {
        triggerNightmare();
      }
    }
  }

  // Do player + EBLOCK
  for (auto t : entities.getEntities(EBLOCK)) {
    if (t.getComponent<CAABB>().blockMovement) {
      overlap = getOverlap(player, t);
      Vec2 prevOverlap = getPreviousOverlap(player, t);
      // If we are colliding note: We CANNOT do a bit of overlap btwn player and tile
      if (overlap.x > 0 && overlap.y > 0) {
        // Up/Down
        if (prevOverlap.x >= 0 && overlap.x > overlap.y) {
          // From Top
          if (player.getComponent<CTransform>().pos.y < t.getComponent<CTransform>().pos.y) {
            player.getComponent<CTransform>().pos.y -= overlap.y;
            // remove airborne
            player.getComponent<CTransform>().velocity.y = 0;
            player.getComponent<CInput>().attack = false;
          }
          else { // From bottom
            player.getComponent<CTransform>().pos.y += overlap.y;
            // stop at block
            player.getComponent<CTransform>().velocity.y = 0;
          } // From left/right
        }
        else {
          // From left
          if (player.getComponent<CTransform>().pos.x < t.getComponent<CTransform>().pos.x) {
            player.getComponent<CTransform>().pos.x -= overlap.x;
          }
          else { // From right
            player.getComponent<CTransform>().pos.x += overlap.x;
          }
        }
      }
    }
  }
  if (died) {
    // If it collides with enemy we kill the hero and restart
    game->getAssets().getSound(SOUNDHERODEATH).play();
    entities.freeAllEntities();
    loadLevel();
  }
}

void Scene_Night::sCoins() {
  if (entities.getEntities(EITEM).size() == 0) {
    hasEnded = true;
    entities.freeAllEntities();
  }
}

void Scene_Night::sSetView() {
  CTransform& playerTransform = player.getComponent<CTransform>();
  auto& tmp = view.getCenter();
  Vec2 viewPos(tmp.x, tmp.y);
  if (abs(playerTransform.pos.x - viewPos.x) > 50) {
    viewPos.x += playerTransform.velocity.x;
  }
  viewPos.y = playerTransform.pos.y;
  view.setCenter(viewPos.x, viewPos.y);
  game->getWindow().setView(view);
}

void Scene_Night::sRender() {
  auto& window = game->getWindow();
  window.clear(sf::Color(3, 3, 35));
  for (auto e : entities.getEntities(EBLOCK)) {
    if (e.hasComponent<CAnimation>()) {
      e.getComponent<CAnimation>().animation.getSprite().setPosition(e.getComponent<CTransform>().pos.x, e.getComponent<CTransform>().pos.y);
      window.draw(e.getComponent<CAnimation>().animation.getSprite());
    }
  }
  for (auto e : entities.getEntities(EITEM)) {
    if (e.hasComponent<CAnimation>()) {
      e.getComponent<CAnimation>().animation.getSprite().setPosition(e.getComponent<CTransform>().pos.x, e.getComponent<CTransform>().pos.y);
      window.draw(e.getComponent<CAnimation>().animation.getSprite());
    }
  }
  if (nightmare) {
    for (auto e : entities.getEntities(EENEMY)) {
      if (e.hasComponent<CAnimation>()) {
        e.getComponent<CAnimation>().animation.getSprite().setPosition(e.getComponent<CTransform>().pos.x, e.getComponent<CTransform>().pos.y);
        window.draw(e.getComponent<CAnimation>().animation.getSprite());
      }
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

void Scene_Night::sDoAction(const Action& action) {
  if (action.getActionType() == ACTSTART) {
    switch (action.getActionName()) {
      case ACTJUMP:
        if (!player.getComponent<CInput>().attack) {
          player.getComponent<CInput>().up = true;
        }
        break;
      case ACTLEFT:
        player.getComponent<CInput>().left = true;
        break;
      case ACTRIGHT:
        player.getComponent<CInput>().right = true;
        break;
      case ACTPAUSE:
        paused = !paused;
        break;
    }
  } else {
    auto& anim = player.getComponent<CAnimation>();
    switch (action.getActionName()) {
      case ACTJUMP:
        player.getComponent<CInput>().up = false;
        break;
      case ACTLEFT:
        player.getComponent<CInput>().left = false;
        if (anim.animation.getName() == ANIMHEROSIDERUN) {
          auto scale = anim.animation.getSprite().getScale();
          if (scale.x == -1) {
            anim.animation = game->getAssets().getAnimation(ANIMHEROSIDE);
            anim.animation.getSprite().setScale(scale);
          }
        }
        break;
      case ACTRIGHT:
        player.getComponent<CInput>().right = false;
        if (anim.animation.getName() == ANIMHEROSIDERUN) {
          auto scale = anim.animation.getSprite().getScale();
          if (scale.x == 1) {
            anim.animation = game->getAssets().getAnimation(ANIMHEROSIDE);
            anim.animation.getSprite().setScale(scale);
          }
        }
        break;
    }
  }
}

void Scene_Night::update() {
  entities.update(); // First of all update the entities (remove and add)
  if (!paused) {
    sMovement(); // Call movement first
    sCollision(); // After movement call collisions to resolve any overlaps
    sSetView(); // After position has been resolved make the camera close in on player
    sAnimation(); // Then call animations (Because of player attack animation)
    sCoins(); // Check for end through collecting all coins
  }
  sRender(); // After everything render
  currentFrame++;
}
