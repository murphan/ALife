//
// Created by Emmet on 2/3/2023.
//

#ifndef ALIFE_SERIALIZER_H
#define ALIFE_SERIALIZER_H

#include <vector>

#include <json.hpp>

#include "types.h"

#include "environment/environment.h"
#include "environment/organism.h"

using json = nlohmann::json;

class StateSerializer {
private:

public:
	static auto serialize(i32 tick, Environment & environment, std::vector<Organism> & organisms) -> json;
};

#endif //ALIFE_SERIALIZER_H
