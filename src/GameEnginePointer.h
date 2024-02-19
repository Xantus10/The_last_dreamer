#pragma once
#include <SFML/Graphics.hpp>
#include "Assets.h"

// Just a class to handle circullar dependance
class GameEnginePointer {
public:
  virtual Assets& getAssets() = 0;
  virtual sf::RenderWindow& getWindow() = 0;
  virtual void quit() = 0;
};
