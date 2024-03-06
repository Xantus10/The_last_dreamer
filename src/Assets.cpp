#include "Assets.h"
#include <iostream>

Assets::Assets() {}


void Assets::addTexture(textureName name, std::string& path) {
  textures[name] = sf::Texture();
  if (!textures[name].loadFromFile(path)) {
    std::cout << "ERROR: Could not load texture from (" << path << ")\nAs a result of that some animations might not work\n";
    return;
  }
}

void Assets::addAnimation(animationName name, const Animation& animation) {
  animations[name] = animation;
}

void Assets::addSound(soundName name, std::string& path) {
  soundbuffers[name] = sf::SoundBuffer();
  sounds[name] = sf::Sound();
  if (!soundbuffers[name].loadFromFile(path)) {
    std::cout << "ERROR: Could not load sound from (" << path << ")\n";
    return;
  }
  sounds[name].setBuffer(soundbuffers[name]);
}

void Assets::addFont(fontName name, std::string& path) {
  fonts[name] = sf::Font();
  if (!fonts[name].loadFromFile(path)) {
    std::cout << "ERROR: Could not load font from (" << path << ")\n";
    return;
  }
}

void Assets::addMusic(musicName name, std::string& path) {
  if (!music[name].openFromFile(path)) {
    std::cout << "ERROR: Could not load music from (" << path << ")\n";
    return;
  }
}



sf::Texture& Assets::getTexture(const textureName name) {
  return textures[name];
}

Animation& Assets::getAnimation(const animationName name) {
  return animations[name];
}

sf::Sound& Assets::getSound(const soundName name) {
  return sounds[name];
}

sf::Font& Assets::getFont(const fontName name) {
  return fonts[name];
}

sf::Music& Assets::getMusic(const musicName name) {
  return music[name];
}
