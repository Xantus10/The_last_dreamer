#include "Scene_Night.h"
#include <fstream>
#include <iostream>

Scene_Night::Scene_Night(GameEnginePointer* aGame, const std::string aLevelPath)
  : Scene(aGame, aLevelPath, 'L') {
  init();
}

// Register actions and call loadLevel()
void Scene_Night::init() {
  game->getAssets().getMusic(MUSICDREAM).setLoop(true);
  game->getAssets().getMusic(MUSICDREAM).play();
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
  int x, y, x2, y2;
  float speed;
  while (levelFile >> id) {
    switch (id) {
    // Player
    case 'P':
    {
      // We check if the player wasnt freed already (resolves problems when reloading)
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
    case 'M':
    { // Moving tiles
      Entity e = entities.addEntity(ETILE);
      levelFile >> x >> y >> x2 >> y2 >> speed;
      e.addComponent<CAnimation>(game->getAssets().getAnimation(ANIMNIGHTPLATFORM));
      e.addComponent<CAABB>(e.getComponent<CAnimation>().animation.getSize(), true, true);
      Vec2 fpos = gridCoordToXY(x, y, e);
      Vec2 spos = gridCoordToXY(x2, y2, e);
      e.addComponent<CTransform>(fpos, Vec2(0, 0), 0);
      e.addComponent<CPatrolAI>(speed);
      e.getComponent<CPatrolAI>().patrolPositions.push_back(fpos);
      e.getComponent<CPatrolAI>().patrolPositions.push_back(spos);
      break;
    }
    case 'N':
    { // Moving wheels
      Entity e = entities.addEntity(ESWORD);
      levelFile >> x >> y >> x2 >> y2 >> speed;
      e.addComponent<CAnimation>(game->getAssets().getAnimation(ANIMNIGHTWHEEL));
      e.addComponent<CAABB>(e.getComponent<CAnimation>().animation.getSize(), true, true);
      Vec2 fpos = gridCoordToXY(x, y, e);
      Vec2 spos = gridCoordToXY(x2, y2, e);
      e.addComponent<CTransform>(fpos, Vec2(0, 0), 0);
      e.addComponent<CPatrolAI>(speed);
      e.getComponent<CPatrolAI>().patrolPositions.push_back(fpos);
      e.getComponent<CPatrolAI>().patrolPositions.push_back(spos);
      e.removeComponent<CAABB>();
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
  for (auto e : entities.getEntities(ESWORD)) {
    e.addComponent<CAABB>(e.getComponent<CAnimation>().animation.getSize(), true, true);
  }
  game->getAssets().getMusic(MUSICDREAM).stop();
  game->getAssets().getMusic(MUSICNIGHTMARE).setLoop(true);
  game->getAssets().getMusic(MUSICNIGHTMARE).play();
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
  if (player.getComponent<CInput>().attack) {
    float scalex = player.getComponent<CAnimation>().animation.getSprite().getScale().x;
    player.getComponent<CAnimation>().animation = game->getAssets().getAnimation(ANIMHEROJUMP);
    player.getComponent<CAnimation>().animation.getSprite().setScale(scalex, 1);
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

  playerTransform.velocity.x = 0;

  if (playerInput.left != playerInput.right) {
    if (playerInput.attack) {
      playerAnimation.animation = game->getAssets().getAnimation(ANIMHEROJUMP);
    } else if (playerAnimation.animation.getName() == ANIMHEROSIDE) {
      playerAnimation.animation = game->getAssets().getAnimation(ANIMHEROSIDERUN);
    }
    if (playerInput.left) {
      playerTransform.velocity.x = -playerConfig.speed;
      playerAnimation.animation.getSprite().setScale(-1, 1);
    } else {
      playerTransform.velocity.x = playerConfig.speed;
      playerAnimation.animation.getSprite().setScale(1, 1);
    }
  } else {
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
  
  // Moving tiles
  for (auto t : entities.getEntities(ETILE)) {
    CPatrolAI& pai = t.getComponent<CPatrolAI>();
    CTransform& trans = t.getComponent<CTransform>();
    trans.velocity = pai.nextPosition() - trans.pos;
    trans.velocity.normalize();
    trans.velocity *= pai.speed;
    trans.previousPos = trans.pos;
    trans.pos += trans.velocity;
  }
  // Moving wheels
  for (auto t : entities.getEntities(ESWORD)) {
    if (!t.hasComponent<CInvincibility>()) {
      CPatrolAI& pai = t.getComponent<CPatrolAI>();
      CTransform& trans = t.getComponent<CTransform>();
      trans.velocity = pai.nextPosition() - trans.pos;
      trans.velocity.normalize();
      trans.velocity *= pai.speed;
      trans.previousPos = trans.pos;
      trans.pos += trans.velocity;
    }
  }
}


void Scene_Night::sInvincibility() {
  // Just for ESWORD, bcs nothing else has it
  for (auto e : entities.getEntities(ESWORD)) {
    if (e.hasComponent<CInvincibility>()) {
      e.getComponent<CInvincibility>().iframes--;
      if (e.getComponent<CInvincibility>().iframes == 0) {
        e.removeComponent<CInvincibility>();
      }
    }
  }
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
  
  // Moving enemies
  for (auto e : entities.getEntities(ESWORD)) {
    // Player + enemy
    overlap = getOverlap(e, player);
    // 10 bcs we give some breathing room for hero
    if (overlap.x > 10 && overlap.y > 10) {
      died = true;
    }
    if (isInside(e.getComponent<CPatrolAI>().nextPosition(), e)) {
      e.getComponent<CPatrolAI>().incrementPosition();
      // We stand in place for 2 sec
      e.addComponent<CInvincibility>(90);
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
      } else {
        game->getAssets().getSound(SOUNDPICKUP).play();
      }
    }
  }

  // Do player + ETILE (First so we can resolve collisions with EBLOCK)
  for (auto t : entities.getEntities(ETILE)) {
    if (t.getComponent<CAABB>().blockMovement) {
      overlap = getOverlap(player, t);
      // If we are colliding note: We CANNOT do a bit of overlap btwn player and tile
      if (overlap.x > 0 && overlap.y > 0) {
        Vec2 prevOverlap = getPreviousOverlap(player, t);
        // Up/Down
        if (prevOverlap.x >= 0 && overlap.x > overlap.y) {
          // From Top
          if (player.getComponent<CTransform>().pos.y < t.getComponent<CTransform>().pos.y) {
            player.getComponent<CTransform>().pos.y -= overlap.y;
            // remove airborne
            player.getComponent<CTransform>().velocity.y = 0;
            player.getComponent<CInput>().attack = false;
            // Stop jump animation
            if (player.getComponent<CAnimation>().animation.getName() == ANIMHEROJUMP) {
              animationName n = (player.getComponent<CInput>().left || player.getComponent<CInput>().right) ? ANIMHEROSIDERUN : ANIMHEROSIDE;
              float scalex = player.getComponent<CAnimation>().animation.getSprite().getScale().x;
              player.getComponent<CAnimation>().animation = game->getAssets().getAnimation(n);
              player.getComponent<CAnimation>().animation.getSprite().setScale(scalex, 1);
            }
            // Move player with ETILE (moving tiles functionality (we do not update prevPos))
            player.getComponent<CTransform>().pos += t.getComponent<CTransform>().velocity;
            player.getComponent<CTransform>().velocity += t.getComponent<CTransform>().velocity;
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
    if (isInside(t.getComponent<CPatrolAI>().nextPosition(), t)) {
      t.getComponent<CPatrolAI>().incrementPosition();
    }
  }

  // Do player + EBLOCK
  for (auto t : entities.getEntities(EBLOCK)) {
    if (t.getComponent<CAABB>().blockMovement) {
      overlap = getOverlap(player, t);
      // If we are colliding note: We CANNOT do a bit of overlap btwn player and tile
      if (overlap.x > 0 && overlap.y > 0) {
        Vec2 prevOverlap = getPreviousOverlap(player, t);
        // Up/Down
        if (prevOverlap.x >= 0 && overlap.x > overlap.y) {
          // From Top
          if (player.getComponent<CTransform>().pos.y < t.getComponent<CTransform>().pos.y) {
            player.getComponent<CTransform>().pos.y -= overlap.y;
            // remove airborne
            player.getComponent<CTransform>().velocity.y = 0;
            player.getComponent<CInput>().attack = false;
            // Remove animation
            if (player.getComponent<CAnimation>().animation.getName() == ANIMHEROJUMP) {
              animationName n = (player.getComponent<CInput>().left || player.getComponent<CInput>().right) ? ANIMHEROSIDERUN : ANIMHEROSIDE;
              float scalex = player.getComponent<CAnimation>().animation.getSprite().getScale().x;
              player.getComponent<CAnimation>().animation = game->getAssets().getAnimation(n);
              player.getComponent<CAnimation>().animation.getSprite().setScale(scalex, 1);
            }
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
    game->getAssets().getMusic(MUSICNIGHTMARE).stop();
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
  for (auto e : entities.getEntities(ETILE)) {
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
    for (auto e : entities.getEntities(ESWORD)) {
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
    sAnimation(); // Then call animations (Because of player airborne animation)
    sCoins(); // Check for end through collecting all coins
    sInvincibility(); // Update CInvincibility for ESWORD
  }
  sRender(); // After everything render
  currentFrame++;
}
