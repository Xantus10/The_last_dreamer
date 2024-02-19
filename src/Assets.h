#pragma once
#include <map>
#include <string>
#include "Animation.h"
#include <SFML/Audio.hpp>

//Enum of all textures (prefix TEX) (they correspond to animations)
enum textureName {
  
};

//Enum of all sounds (prefix SOUND)
enum soundName {
  
};

//Enum of all fonts (prefix FONT)
enum fontName {
  FONTBAUHS93,
  FONTCONSOLA
};

// Enum of all music (prefix MUSIC)
enum musicName {

};

//Assets manager
class Assets {
  std::map<textureName, sf::Texture> textures; // map of textureNames to sf::Textures
  std::map<animationName, Animation> animations; // map of animationNames to Animations
  std::map<soundName, sf::Sound> sounds; // map of soundNames to sf::Sound
  std::map<fontName, sf::Font> fonts; // map of fontNames to sf::Font
  std::map<musicName, sf::Music> music; // map of musicNames to sf::Music
public:
  Assets(); // Default constructor (use it)

  void addTexture(textureName name, std::string& path);
  void addAnimation(animationName name, const Animation& animation);
  void addSound(soundName name, std::string& path);
  void addFont(fontName name, std::string& path);
  void addMusic(musicName name, std::string& path);

  sf::Texture& getTexture(const textureName name);
  Animation& getAnimation(const animationName name);
  sf::Sound& getSound(const soundName name);
  sf::Font& getFont(const fontName name);
  sf::Music& getMusic(const musicName name);
};
