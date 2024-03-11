#include "Action.h"

Action::Action()
  : name(ACTNOOPERATION)
  , type(ACTEND)
  , eventptr(nullptr) {}

Action::Action(const actionName aName, const actionType aType)
  : name(aName)
  , type(aType)
  , eventptr(nullptr) {}

Action::Action(const actionType aType, sf::Event* aEventPtr)
  : name(ACTNOOPERATION)
  , type(aType)
  , eventptr(aEventPtr) {}

const actionName Action::getActionName() const {
  return name;
}

const actionType Action::getActionType() const {
  return type;
}

const sf::Event* Action::getEvent() const {
  return eventptr;
}
