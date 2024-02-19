#include "Animation.h"
#include <iostream>

Animation::Animation() {}

Animation::Animation(animationName aName, const sf::Texture& aTexture, uint8_t aFrameCount, uint8_t aAnimationSpeed)
  : name(aName)
  , sprite(aTexture)
  , frameCount(aFrameCount)
  , animationSpeed(aAnimationSpeed)
  , currentFrame(0) {
  size = Vec2((float)aTexture.getSize().x / frameCount, (float)aTexture.getSize().y);
  sprite.setOrigin(size.x / 2.0f, size.y / 2.0f);
  sprite.setTextureRect(sf::IntRect(0, 0, size.x, size.y));
}

void Animation::update() {
  currentFrame++;
  if (animationSpeed != 0) {
    if (currentFrame % animationSpeed == 0) {
      sprite.setTextureRect(sf::IntRect(((currentFrame / animationSpeed) % frameCount) * size.x, 0, size.x, size.y));
    }
  }
}

bool Animation::hasEnded() const {
  return currentFrame > ((frameCount-1) * animationSpeed);
}

animationName Animation::getName() const {
  return name;
}

Vec2& Animation::getSize() {
  return size;
}

sf::Sprite& Animation::getSprite() {
  return sprite;
}

size_t Animation::getTotalFramesOfAnimationRan() const {
  return currentFrame;
}
