//
// Created by Emmet on 2/6/2023.
//

#ifndef ALIFE_BODYPART_H
#define ALIFE_BODYPART_H

enum BodyPart {
	NONE = 0,
	MOUTH = 1,
	MOVER = 2,
	PHOTOSYNTHESIZER = 3,
	WEAPON = 4,
	ARMOR = 5,
	EYE = 6,
	SCAFFOLD = 7,
};

constexpr static const char * BODY_PART_NAMES[] = {
	"None",
	"Mouth",
	"Mover",
	"Photosynthesizer",
	"Weapon",
	"Armor",
	"Eye",
	"Scaffold"
};

#endif //ALIFE_BODYPART_H
