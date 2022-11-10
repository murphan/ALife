//
// Created by Emmet on 11/10/2022.
//

#ifndef ALIFE_ACTION_H
#define ALIFE_ACTION_H

#include "types.h"

class Action {
	public:
		enum ActionType {
			MOVE_RANDOMLY,
			MOVE_TOWARD,
			MOVE_AWAY
		};

		Action(ActionType, i32, i32);

		static const char * actionNames[3];

		ActionType actionType;
		i32 priority;
		i32 triggerSense;
};

#endif //ALIFE_ACTION_H
