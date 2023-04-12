//
// Created by Emmet on 4/11/2023.
//

#ifndef ALIFE_RENDERER_H
#define ALIFE_RENDERER_H

#include <vector>
#include "../types.h"

#include "environment.h"
#include "organism.h"

class Renderer {
public:
	static auto render(Environment & environment, std::vector<Organism> & organisms) -> std::vector<u8>;
};

#endif //ALIFE_RENDERER_H
