//
// Created by Emmet on 2/6/2023.
//

export module BodyPart;

export enum BodyPart {
	NONE = 0,
	MOUTH = 1,
	MOVER = 2,
	PHOTOSYNTHESIZER = 3,
	WEAPON = 4,
	ARMOR = 5,
	EYE = 6,
	SCAFFOLD = 7,
};

export const char * BODY_PART_NAMES[] = {
	"None",
	"Mouth",
	"Mover",
	"Photosynthesizer",
	"Weapon",
	"Armor",
	"Eye",
	"Scaffold"
};
