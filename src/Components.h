#pragma once
#include "Vec2.h"
#include "Animation.h"
#include <SFML/Graphics.hpp>


//base component class
class Component {
public:
  bool has = false; // Bool to check if an entity has the component
  Component() {}
};

//Movement component
class CTransform : public Component {
public:
  //Current position
  Vec2 pos = { 0.0f, 0.0f };
  //Previous position
  Vec2 previousPos = { 0.0f, 0.0f };
  //Current Vec2 velocity
  Vec2 velocity = { 0.0f, 0.0f };
  //Speed
  float speed = 0;

  //Constructor
  CTransform(const Vec2& aPos, const Vec2& aVelocity, const float aSpeed)
    : pos(aPos)
    , previousPos(aPos)
    , velocity(aVelocity)
    , speed(aSpeed) {}
  CTransform() {}
};

//Axis Aligned Bounding Box component
class CAABB : public Component {
public:
  Vec2 size = { 0.0f, 0.0f };
  Vec2 halfSize = { 0.0f, 0.0f };
  bool blockVision = false;
  bool blockMovement = false;

  CAABB() {}
  CAABB(const Vec2& aSize)
    : size(aSize)
    , halfSize(aSize / 2) {}
  CAABB(const Vec2& aSize, bool aBVis, bool aBMov)
    : size(aSize)
    , halfSize(aSize/2)
    , blockVision(aBVis)
    , blockMovement(aBMov) {}
};

//Animation component (has logic in Animation class)
class CAnimation : public Component {
public:
  Animation animation;
  bool repeating = false;

  CAnimation() {}
  CAnimation(Animation& aAnimation, bool aRepeat=false)
    : animation(aAnimation)
    , repeating(aRepeat) {}
};

//Lifespan of entity
class CLifespan : public Component {
public:
  uint32_t maxLifespan;
  uint32_t lifespan;
  CLifespan(size_t aInitialLifespan)
    : maxLifespan(aInitialLifespan)
    , lifespan(aInitialLifespan) {}
  CLifespan() {}
};

//Input component for user interaction and states
class CInput : public Component {
public:
  bool left=false, right=false, up=false, down=false, attack=false;
  CInput() {}
};

class CDamage : public Component {
public:
  uint8_t damage = 1;
  CDamage() {}
  CDamage(uint8_t aDamage)
    : damage(aDamage) {}
};

class CInvincibility : public Component {
public:
  uint32_t iframes = 0;
  CInvincibility() {}
  CInvincibility(uint32_t aIframes)
    : iframes(aIframes) {}
};

class CHP : public Component {
public:
  uint8_t maxHp = 1;
  int8_t currentHp = 1;
  CHP() {}
  CHP(uint8_t aMax, int16_t aCur)
    : maxHp(aMax)
    , currentHp(aCur) {}
};

class CFollowAI : public Component {
public:
  Vec2 defaultPosition = { 0.0f, 0.0f };
  float speed = 0.0f;
  CFollowAI() {}
  CFollowAI(Vec2 aPos, float aSpeed)
    : defaultPosition(aPos)
    , speed(aSpeed) {}
};

class CPatrolAI : public Component {
public:
  std::vector<Vec2> patrolPositions;
  uint8_t currentPosition = 0;
  float speed = 0.0f;
  CPatrolAI() {}
  CPatrolAI(std::vector<Vec2>& aPosVec, float aSpeed)
    : patrolPositions(aPosVec)
    , speed(aSpeed) {}
};