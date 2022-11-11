//
// Created by Anna on 11/9/2022.
//

#ifndef ALIFE_ENVIRONMENT_H
#define ALIFE_ENVIRONMENT_H

#include "src/Environment/MapCell.h"

namespace std {


    class Environment {
        public:
            Environment(int);

            int cells;
            MapCell map;
    };

} // std

#endif //ALIFE_ENVIRONMENT_H
