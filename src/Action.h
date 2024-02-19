#pragma once

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
  ACTEND=1
};


class Action {
  //Name of this action
  actionName name;
  //Type of this action
  actionType type;
public:
  //Default constructor
  Action();
  //Use this constructor
  Action(const actionName aName, const actionType aType);
  const actionName getActionName() const;
  const actionType getActionType() const;
};
