#pragma once
#include <map>
#include <string>
#include "Animation.h"
#include <SFML/Audio.hpp>

//Enum of all textures (prefix TEX) (they correspond to animations)
enum textureName {
  TEXENEMYBASIC = 0,
  TEXENEMYDESTROY = 1,
  TEXENEMYWITHARMOR = 2,
  TEXFLOOR = 3,
  TEXFLOORFLOWERS = 4,
  TEXHEROBACK = 5,
  TEXHEROBACKRUN = 6,
  TEXHEROFACE = 7,
  TEXHEROFACERUN = 8,
  TEXHEROSIDE = 9,
  TEXHEROSIDERUN = 10,
  TEXSWORD = 11,
  TEXWATER = 12,
  TEXBORDER = 13,
  TEXBUSH = 14,
  TEXGRASS = 15,
  TEXTREE = 16,
  TEXTELEPORT = 17,
  TEXHPCONTAINER = 18,
  TEXTORCH = 19,
  TEXFINISH = 20,
  TEXRING = 21,
  TEXKEYGREEN = 22,
  TEXKEYPURPLE = 23,
  TEXKEYYELLOW = 24,
  TEXBLOCKGREEN = 25,
  TEXBLOCKPURPLE = 26,
  TEXBLOCKYELLOW = 27,
  TEXNIGHTCOIN = 28,
  TEXNIGHTPLATFORM = 29,
  TEXNIGHTWHEEL = 30,
  TEXNIGHTLEAK = 31,
  TEXHEROJUMP = 32
};

//Enum of all sounds (prefix SOUND)
enum soundName {
  SOUNDGOTHIT = 0,
  SOUNDHEARTBEAT = 1,
  SOUNDHERODEATH = 2,
  SOUNDPICKUP = 3
};

//Enum of all fonts (prefix FONT)
enum fontName {
  FONTANTONREG = 0,
  FONTINCONSOLATAREG = 1
};

// Enum of all music (prefix MUSIC)
enum musicName {

};

//Assets manager
class Assets {
  std::map<textureName, sf::Texture> textures; // map of textureNames to sf::Textures
  std::map<animationName, Animation> animations; // map of animationNames to Animations
  std::map<soundName, sf::SoundBuffer> soundbuffers; // map of soundbuffers (Texture->Sprite == SoundBuffer->Sound)
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

