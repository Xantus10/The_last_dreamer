#pragma once
#include "Scene.h"

struct DialogueInstruction {
  char instructionId;
  std::string rest;
};

class Scene_Dialogue : public Scene {
  std::vector<sf::Texture> imgTextures;
  std::vector<Animation> imgAnimations;
  std::vector<DialogueInstruction> instructions;
  sf::Text messageText;
  bool released = false;

  uint16_t currentImgIndex;
  uint16_t currentInstructionIndex;


  void sProcessNext();
  void sRender(); // Render the image nad text
  bool sCheckEnd(); // Check for end of scene
public:
  void init(); // Initialize
  void sDoAction(const Action& action); // Just to notify, when any key has been pressed
  Scene_Dialogue(GameEnginePointer* aGame, std::string& aPath); // Constructor
  void update(); // Update
};
