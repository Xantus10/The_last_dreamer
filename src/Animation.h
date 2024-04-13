#pragma once
#include "Vec2.h"
#include <SFML/Graphics.hpp>

//Enum of all animation names (prefix ANIM)
enum animationName {
  ANIMENEMYBASIC = 0,
  ANIMENEMYDESTROY = 1,
  ANIMENEMYWITHARMOR = 2,
  ANIMFLOOR = 3,
  ANIMFLOORFLOWERS = 4,
  ANIMHEROBACK = 5,
  ANIMHEROBACKRUN = 6,
  ANIMHEROFACE = 7,
  ANIMHEROFACERUN = 8,
  ANIMHEROSIDE = 9,
  ANIMHEROSIDERUN = 10,
  ANIMSWORD = 11,
  ANIMWATER = 12,
  ANIMBORDER = 13,
  ANIMBUSH = 14,
  ANIMGRASS = 15,
  ANIMTREE = 16,
  ANIMTELEPORT = 17,
  ANIMHPCONTAINER = 18,
  ANIMTORCH = 19,
  ANIMFINISH = 20,
  ANIMRING = 21,
  ANIMKEYGREEN = 22,
  ANIMKEYPURPLE = 23,
  ANIMKEYYELLOW = 24,
  ANIMBLOCKGREEN = 25,
  ANIMBLOCKPURPLE = 26,
  ANIMBLOCKYELLOW = 27,
  ANIMNIGHTCOIN = 28,
  ANIMNIGHTPLATFORM = 29,
  ANIMNIGHTWHEEL = 30,
  ANIMNIGHTLEAK = 31,
  ANIMHEROJUMP = 32,
  ANIMBOOTS = 33
};

//Animation functionality
class Animation {
  // Sprite to be drawn
  sf::Sprite sprite;
  // how much frames are in the animation
  uint8_t frameCount;
  // current frame the animation is running
  size_t currentFrame;
  // change frame every # frames
  uint8_t animationSpeed;
  // size of one frame of the animation
  Vec2 size;
  // name of the animation
  animationName name;
public:
  // Default constructor
  Animation();
  // Use this constructor
  Animation(animationName aName, const sf::Texture& aTexture, uint8_t aFrameCount, uint8_t aAnimationSpeed);
  // Update animation
  void update();
  // Return true if the animation has ran for one cycle
  bool hasEnded() const;
  animationName getName() const;
  Vec2& getSize();
  sf::Sprite& getSprite();
  size_t getTotalFramesOfAnimationRan() const;
};
