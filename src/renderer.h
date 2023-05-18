//
// Created by Emmet on 4/11/2023.
//

#ifndef ALIFE_RENDERER_H
#define ALIFE_RENDERER_H

#include <vector>
#include "util/types.h"

#include "environment/organismGrid.h"
#include "environment/environment.h"
#include "organism/organism.h"
#include "controls.h"

class Renderer {
public:
	static auto render(
		const Environment & environment,
		const std::vector<Organism> & organisms,
		const Controls & controls
	) -> std::vector<u8>;
};

#endif //ALIFE_RENDERER_H
