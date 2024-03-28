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
  Vec2 retVec((gridX * 64) + size.x, (WINDOW_HEIGHT - (gridY * 64)) - size.y);
  return retVec;
}

void Scene_Night::loadLevel() {
  // Reload EntityManager
  entities = EntityManager();

  std::ifstream levelFile(levelPath);

  char id = ' ';
  int x, y;
  while (levelFile >> id) {
    switch (id) {
    // Player
    case 'P':
    {
      Entity e = entities.addEntity(EPLAYER);
      levelFile >> playerConfig.spawnX >> playerConfig.spawnY >> playerConfig.BBWidth >> playerConfig.BBHeight
        >> playerConfig.speed >> playerConfig.jumpSpeed >> playerConfig.maxSpeed >> playerConfig.gravity;
      e.addComponent<CAnimation>(game->getAssets().getAnimation(ANIMHEROSIDE));
      e.addComponent<CAABB>(Vec2(playerConfig.BBWidth, playerConfig.BBHeight));
      e.addComponent<CTransform>(gridCoordToXY(playerConfig.spawnX, playerConfig.spawnY, e), Vec2(0, 0), playerConfig.speed);
      e.addComponent<CInput>();
      view = game->getWindow().getDefaultView();
      view.setCenter(e.getComponent<CTransform>().pos.x, e.getComponent<CTransform>().pos.y-WINDOW_HEIGHT/4);
      game->getWindow().setView(view);
      break;
    }
    // Enemy
    case 'E':
    {
      Entity e = entities.addEntity(EENEMY);
      levelFile >> x >> y;
      e.addComponent<CAnimation>(game->getAssets().getAnimation(ANIMENEMYWITHARMOR)); /*PLACEHOLDER*/
      e.addComponent<CTransform>(gridCoordToXY(x, y, e), Vec2(0, 0), 0);
      break;
    }
    case 'C':
    {
      Entity e = entities.addEntity(EITEM);
      levelFile >> x >> y;
      e.addComponent<CAnimation>(game->getAssets().getAnimation(ANIMRING)); /*PLACEHOLDER*/
      e.addComponent<CAABB>(e.getComponent<CAnimation>().animation.getSize(), true, true);
      e.addComponent<CTransform>(gridCoordToXY(x, y, e), Vec2(0, 0), 0);
      break;
    }
    case 'B':
    {
      Entity e = entities.addEntity(EBLOCK);
      levelFile >> x >> y;
      e.addComponent<CAnimation>(game->getAssets().getAnimation(ANIMFLOORFLOWERS)); /*PLACEHOLDER*/
      e.addComponent<CAABB>(e.getComponent<CAnimation>().animation.getSize(), true, true);
      e.addComponent<CTransform>(gridCoordToXY(x, y, e), Vec2(0, 0), 0);
      break;
    }
    }
  }
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

char velocityToDir(Vec2& vel) {
  if (vel.x >= 0) {
    return 'd';
  }
  return 'a';
}

void Scene_Night::sMovement() {
  auto& playerTransform = entities.player().getComponent<CTransform>();
  auto& playerInput = entities.player().getComponent<CInput>();
  auto& playerAnimation = entities.player().getComponent<CAnimation>();
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
    switch (velocityToDir(playerTransform.velocity)) {
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
    playerTransform.velocity.y += playerConfig.jumpSpeed;
  }
  playerTransform.velocity.y -= playerConfig.gravity;

  // Max speed check
  if (playerTransform.velocity.y > playerConfig.maxSpeed) {
    playerTransform.velocity.y = playerConfig.maxSpeed;
  }

  // Finally update position
  playerTransform.pos += playerTransform.velocity;
}


void Scene_Night::sCollision() {
  Vec2 overlap;
  Entity player = entities.player();

  for (auto e : entities.getEntities(EENEMY)) {
    // Player + enemy
    overlap = getOverlap(e, player);
    // 2 bcs we give some breathing room for hero
    if (overlap.x > 2 && overlap.y > 2) {
      // If it collides with enemy we kill the hero and restart
      game->getAssets().getSound(SOUNDHERODEATH).play();
      entities.freeAllEntities();
      loadLevel();
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
        if (prevOverlap.x > 0) {
          // From Top
          if (player.getComponent<CTransform>().pos.y < t.getComponent<CTransform>().pos.y) {
            player.getComponent<CTransform>().pos.y -= overlap.y;
          }
          else { // From bottom
            player.getComponent<CTransform>().pos.y += overlap.y;
            // remove airborne
            player.getComponent<CTransform>().velocity.y = 0;
            player.getComponent<CInput>().attack = false;
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
  if (entities.player().getComponent<CHP>().currentHp <= 0) {
    
  }
}

void Scene_Night::sCoins() {
  if (entities.getEntities().size() == 0) {
    hasEnded = true;
    entities.freeAllEntities();
  }
}

void Scene_Night::sSetView() {
  CTransform& playerTransform = entities.player().getComponent<CTransform>();
  auto& tmp = view.getCenter();
  Vec2 viewPos(tmp.x, tmp.y);
  if (abs(playerTransform.pos.x - viewPos.x) > 80) {
    viewPos.x += playerTransform.velocity.x;
  }
  if (abs(playerTransform.pos.y - viewPos.y) > 150) {
    viewPos.y += playerTransform.velocity.y;
  }
  view.setCenter(viewPos.x, viewPos.y);
  game->getWindow().setView(view);
}

void Scene_Night::sRender() {
  auto& window = game->getWindow();
  window.clear(sf::Color(5, 5, 50));
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
}

void Scene_Night::sDoAction(const Action& action) {
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
      player->getComponent<CInput>().left = false;
      break;
    case ACTRIGHT:
      player->getComponent<CInput>().right = false;
      break;
    }
  }
}

void sDebug() {} // only maybe implement

void Scene_Night::update() {
  entities.update(); // First of all update the entities (remove and add)
  if (!paused) {
    sMovement(); // Call movement first
    sCollision(); // After movement call collisions to resolve any overlaps
    sSetView(); // After position has been resolved make the camera close in on player
    sAnimation(); // Then call animations (Because of player attack animation)
  }
  sRender(); // After everything render
  currentFrame++;
}
