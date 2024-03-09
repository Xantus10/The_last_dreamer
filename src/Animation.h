#pragma once
#include "Vec2.h"
#include <SFML/Graphics.hpp>

//Enum of all animation names (prefix ANIM)
enum animationName {
  ANIMENEMYBASIC,
  ANIMENEMYDESTROY,
  ANIMENEMYWITHARMOR,
  ANIMFLOOR,
  ANIMFLOORFLOWERS,
  ANIMHEROBACK,
  ANIMHEROBACKRUN,
  ANIMHEROFACE,
  ANIMHEROFACERUN,
  ANIMHEROSIDE,
  ANIMHEROSIDERUN,
  ANIMSWORD,
  ANIMWATER,
  ANIMBORDER,
  ANIMBUSH,
  ANIMGRASS,
  ANIMTREE,
  ANIMTELEPORT,
  ANIMHPCONTAINER,
  ANIMTORCH
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
