#pragma once

#include <SFML/Window.hpp>

//Enum of all actions (prefix ACT)
enum actionName {
  ACTNOOPERATION,
  ACTUP,
  ACTDOWN,
  ACTLEFT,
  ACTRIGHT,
  ACTJUMP,
  ACTATTACK,
  ACTSELECT,
  ACTQUIT,
  ACTPAUSE
};

//Enum of type of input (press/release) (prefix ACT)
enum actionType {
  ACTSTART=0,
  ACTEND=1,
  ACTMOUSEDOWN=2,
  ACTMOUSEUP=3
};


class Action {
  //Name of this action
  actionName name;
  //Type of this action
  actionType type;
  //Optional event ptr
  sf::Event* eventptr;
public:
  //Default constructor
  Action();
  //Constructor for keypresses
  Action(const actionName aName, const actionType aType);
  //Constructor for mouse events
  Action(const actionType aType, sf::Event* aEventPtr);
  const actionName getActionName() const;
  const actionType getActionType() const;
  const sf::Event* getEvent() const;
};
