//
// Created by Emmet on 4/27/2023.
//
#include "weapon.h"

auto Weapon::tick(i32 x, i32 y, i32 index, Body::Cell & weapon, Environment & environment, OrganismGrid & organismGrid, std::vector<Organism> & organisms, Settings & settings) -> void {
	auto superAttack = !weapon.isModified() ? -1 : weapon.modifier();

	dealDamage(x + 1, y, superAttack, index, environment, organismGrid, organisms, settings);
	dealDamage(x, y + 1, superAttack, index, environment, organismGrid, organisms, settings);
	dealDamage(x - 1, y, superAttack, index, environment, organismGrid, organisms, settings);
	dealDamage(x, y - 1, superAttack, index, environment, organismGrid, organisms, settings);
}

auto Weapon::dealDamage(i32 x, i32 y, i32 superAttack, i32 index, Environment & environment, OrganismGrid & organismGrid, std::vector<Organism> & organisms, Settings & settings) -> void {
	auto && space = organismGrid.access(x, y);

	/* weapons break down food */
	if (space.isFilled() && space.cell().dead() && space.index() != index) {
		space.cell().setBroken(true);

		/* detach from organism */
		if (space.fromOrganism()) {
			environment.accessUnsafe(x, y).food = space.cell();
			space.cell() = Body::Cell::makeEmpty();
		}

		space = OrganismGrid::Space::makeEmpty();
		return;
	}

	/* attacking cells as parts of organism */
	if (!space.fromOrganism() || space.cell().dead()) return;
	auto defenderIndex = space.index();
	if (defenderIndex == index) return;


	auto blockLevel = armorDoesBlock(x + 0, y + 0, superAttack, true, defenderIndex, organismGrid) |
	                  armorDoesBlock(x + 1, y + 0, superAttack, false, defenderIndex, organismGrid) |
	                  armorDoesBlock(x + 0, y + 1, superAttack, false, defenderIndex, organismGrid) |
	                  armorDoesBlock(x - 1, y + 0, superAttack, false, defenderIndex, organismGrid) |
	                  armorDoesBlock(x + 0, y - 1, superAttack, false, defenderIndex, organismGrid);

	auto damageDealt = (blockLevel >> 1) & 0x1 ? 0 :
	                   blockLevel & 0x1 ? settings.weaponDamage - settings.armorPrevents :
	                   settings.weaponDamage;

	if (damageDealt > 0) {
		auto && defender = organisms[defenderIndex];
		defender.addEnergy(-damageDealt);

		if (defender.energy == 0) {
			killCell(defender, space.cell(), settings);
		}
	}
}

auto Weapon::armorDoesBlock(i32 x, i32 y, i32 superAttack, bool directAttack, i32 defenderIndex, OrganismGrid & organismGrid) -> BlockResult {
	auto && space = organismGrid.access(x, y);
	if (!space.fromOrganism() || space.index() != defenderIndex || space.cell().dead()) return BLOCK_NONE;

	auto bodyPart = space.cell().bodyPart();
	auto modifier = !space.cell().isModified() ? -1 : space.cell().modifier();

	auto isArmored = (bodyPart == BodyPart::ARMOR) ||
	                 (bodyPart == BodyPart::SCAFFOLD && modifier != -1 && directAttack);

	if (!isArmored) return BLOCK_NONE;

	/* for some reason intellisense fails to realize this variable is used */
	volatile auto superArmor = bodyPart == BodyPart::ARMOR ? modifier : -1;

	auto superCounters = superArmor == superAttack;

	if (directAttack) {
		if (superAttack == -1) {
			return superArmor == -1 ? BLOCK_FULL : BLOCK_FULL_BUT_COOLER;
		} else {
			if (superArmor == -1) {
				return BLOCK_PART;
			} else {
				return superCounters ? BLOCK_FULL : BLOCK_PART;
			}
		}
	} else {
		if (superAttack == -1) {
			return superArmor == -1 ? BLOCK_PART : BLOCK_FULL;
		} else {
			if (superArmor == -1) {
				return BLOCK_NONE;
			} else {
				return superCounters ? BLOCK_PART : BLOCK_NONE;
			}
		}
	}
}

auto Weapon::killCell(Organism & organism, Body::Cell & cell, Settings & settings) -> void {
	organism.phenome.onRemoveCell(cell, settings);
	cell.setDead(true);
}
