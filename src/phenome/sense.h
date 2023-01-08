//
// Created by Rosa on 1/7/2023.
//

#ifndef ALIFE_SENSE_H
#define ALIFE_SENSE_H

#include "types.h"

class Sense {
public:
	i32 x;
	i32 y;
	/* the body part code at this x, y in the organism */
	i32 sense;

	Sense(i32 x, i32 y, i32 sense);
};

#endif //ALIFE_SENSE_H
