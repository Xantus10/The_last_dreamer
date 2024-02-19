#include "Action.h"

Action::Action()
  : name(ACTNOOPERATION)
  , type(ACTEND) {}

Action::Action(const actionName aName, const actionType aType)
  : name(aName)
  , type(aType) {}

const actionName Action::getActionName() const {
  return name;
}

const actionType Action::getActionType() const {
  return type;
}
