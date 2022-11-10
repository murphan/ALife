//
// Created by Emmet on 11/10/2022.
//

#include "action.h"

Action::Action(ActionType type, i32 priority, i32 triggerSense):
	actionType(type), priority(priority), triggerSense(triggerSense) {}

const char * Action::actionNames[3] = {
	"Move Randomly",
	"Move Toward",
	"Move Away",
};