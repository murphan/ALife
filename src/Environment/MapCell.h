//
// Created by Anna on 11/9/2022.
//

#ifndef ALIFE_MAPCELL_H
#define ALIFE_MAPCELL_H

#include "src/Environment/Food.h"

namespace std {

    class MapCell {
    public:
        int temp;
        int light;
        Food food;
        static int type;
        static int isWall;

        MapCell(int inTemp, int inLight);


    };

} // std

#endif //ALIFE_MAPCELL_H
