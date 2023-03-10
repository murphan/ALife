//
// Created by Emmet on 3/9/2023.
//

#ifndef ALIFE_SETTINGS_H
#define ALIFE_SETTINGS_H

#include "noise/noise.h"

class Settings {
public:
	i32 ageFactor;

	Noise factorNoises[3];
};

#endif //ALIFE_SETTINGS_H
