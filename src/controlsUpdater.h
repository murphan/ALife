//
// Created by Emmet on 5/16/2023.
//

#ifndef ALIFE_CONTROLSUPDATER_H
#define ALIFE_CONTROLSUPDATER_H

#include "controls.h"
#include "environment/tree.h"

namespace ControlsUpdater {
	auto update(json & input, Controls & controls, Tree & tree) -> void;
}

#endif //ALIFE_CONTROLSUPDATER_H
