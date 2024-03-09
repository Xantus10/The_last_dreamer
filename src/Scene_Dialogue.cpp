#include "Scene_Dialogue.h"
#include <fstream>
#include <iostream>

void Scene_Dialogue::init() {
  currentImgIndex = 0;
  currentInstructionIndex = 0;
  imgTextures.reserve(sizeof(sf::Texture) * 3);
  imgAnimations.reserve(sizeof(Animation) * 3);
  instructions.reserve(sizeof(DialogueInstruction) * 20); // Maybe change from 20 to smth else
  std::ifstream file(levelPath);
  file >> nextLevel.levelName >> nextLevel.levelPath >> nextLevel.levelType;
  std::string imgPath;
  for (int i = 0; i < 3; i++) {
    file >> imgPath;
    imgTextures.push_back(sf::Texture());
    if (!imgTextures[i].loadFromFile(imgPath)) {
      std::cout << "ERROR: Could not load texture from (" << imgPath << ")\nAs a result of that some animations might not work\n";
    }
    imgAnimations.push_back(Animation((animationName)0, imgTextures[i], 1, 0));
    imgAnimations[i].getSprite().setPosition(game->getWindow().getSize().x / 2.0f, 420); // Maybe change the y pos
  }
  char lineId;
  std::string tmp;
  std::string message;
  while (file >> lineId) {
    file >> tmp;
    if (lineId == 'S') {
      while (tmp != "\"") {
        message += tmp;
        message += " ";
        file >> tmp;
      }
    } else {
      message = tmp;
    }
    instructions.push_back({ lineId, message });
    message = "";
  }
  
  for (int k = 0; k < sf::Keyboard::Enter; k++) {
    registerAction(k, ACTSELECT);
  }
  sProcessNext();
  game->getWindow().setView(game->getWindow().getDefaultView());
}

void Scene_Dialogue::sProcessNext() {
  if (sCheckEnd()) {
    hasEnded = true;
    return;
  }
  if (instructions[currentInstructionIndex].instructionId == 'D') {
    currentImgIndex = std::stoi(instructions[currentInstructionIndex].rest);
    currentInstructionIndex++;
    sProcessNext();
  } else if (instructions[currentInstructionIndex].instructionId == 'S') {
    messageText = sf::Text(instructions[currentInstructionIndex].rest, game->getAssets().getFont(FONTINCONSOLATAREG), 24);
    messageText.setFillColor(sf::Color(244, 244, 244));
    messageText.setOrigin(messageText.getLocalBounds().width / 2.0f, messageText.getLocalBounds().height / 2.0f);
    messageText.setPosition(game->getWindow().getSize().x/2.0f, 820); // Change y if necessary
    currentInstructionIndex++;
  }
}

void Scene_Dialogue::sRender() {
  auto& window = game->getWindow();
  window.clear();
  window.draw(imgAnimations[currentImgIndex].getSprite());
  window.draw(messageText);
  window.display();
}

bool Scene_Dialogue::sCheckEnd() {
  return currentInstructionIndex == instructions.size();
}

void Scene_Dialogue::sDoAction(const Action& action) {
  if (action.getActionType() == ACTSTART) {
    sProcessNext();
  }
}

Scene_Dialogue::Scene_Dialogue(GameEnginePointer* aGame, std::string& aPath)
  : Scene(aGame, aPath, 'I') {
  init();
}

void Scene_Dialogue::update() {
  sRender();
}
