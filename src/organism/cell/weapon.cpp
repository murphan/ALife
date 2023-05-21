//
// Created by Emmet on 4/27/2023.
//

module;

#include <vector>

export module Weapon;

import Types;
import Organism;
import Settings;
import Cell;
import Environment;
import OrganismGrid;
import BodyPart;
import Space;

namespace Weapon {
	enum BlockResult {
		BLOCK_NONE = 0x0,
		BLOCK_PART = 0x1,
		BLOCK_FULL = 0x2,
		BLOCK_FULL_BUT_COOLER = 0x2,
	};

	export auto killCell(Organism & organism, Cell & cell) -> void {
		organism.phenome.onKilledCell(cell);
		cell.setDead(true);
	}

	auto armorDoesBlock(i32 x, i32 y, i32 superAttack, bool directAttack, i32 defenderIndex, OrganismGrid & organismGrid, std::vector<Organism> & organisms) -> BlockResult {
		auto && space = organismGrid.access(x, y);
		if (!space.fromOrganism() || space.index() != defenderIndex) return BLOCK_NONE;

		auto && cell = space.cell(organisms);
		if (cell.dead()) return BLOCK_NONE;

		auto bodyPart = cell.bodyPart();
		auto modifier = cell.modifier();

		auto isArmored = (bodyPart == BodyPart::ARMOR) ||
		                 (bodyPart == BodyPart::SCAFFOLD && modifier != -1 && directAttack);

		if (!isArmored) return BLOCK_NONE;

		auto superArmor = bodyPart == BodyPart::ARMOR ? modifier : -1;

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

	auto detachCell(Organism & organism, Environment & environment, Cell & cell) -> void {
		auto [x, y] = organism.absoluteXY(cell);
		environment.accessUnsafe(x, y).food = cell;
		organism.body().removeCell(cell);
	}

	auto dealDamage(i32 x, i32 y, i32 superAttack, i32 index, Environment & environment, OrganismGrid & organismGrid, std::vector<Organism> & organisms, Settings & settings) -> void {
		auto && space = organismGrid.access(x, y);

		/* weapons break down food */
		if (space.isFilled()) {
			auto && cell = space.cell(organisms, environment);
			if (cell.dead() && space.index() != index) {
				cell.setBroken(true);

				if (space.fromOrganism())
					detachCell(organisms[space.index()], environment, cell);

				space = Space::makeEmpty();
				return;
			}
		}

		/* attacking cells as parts of organism */
		if (!space.fromOrganism()) return;
		auto && cell = space.cell(organisms);
		if (cell.dead()) return;

		auto defenderIndex = space.index();
		if (defenderIndex == index) return;


		auto blockLevel = armorDoesBlock(x + 0, y + 0, superAttack, true, defenderIndex, organismGrid, organisms) |
		                  armorDoesBlock(x + 1, y + 0, superAttack, false, defenderIndex, organismGrid, organisms) |
		                  armorDoesBlock(x + 0, y + 1, superAttack, false, defenderIndex, organismGrid, organisms) |
		                  armorDoesBlock(x - 1, y + 0, superAttack, false, defenderIndex, organismGrid, organisms) |
		                  armorDoesBlock(x + 0, y - 1, superAttack, false, defenderIndex, organismGrid, organisms);

		auto damageDealt = (blockLevel >> 1) & 0x1 ? 0 :
		                   blockLevel & 0x1 ? settings.weaponDamage - settings.armorPrevents :
		                   settings.weaponDamage;

		if (damageDealt > 0) {
			auto && defender = organisms[defenderIndex];
			defender.addEnergy(-damageDealt);

			if (defender.energy == 0) {
				killCell(defender, cell);
			}
		}
	}

	export auto tick(i32 x, i32 y, i32 index, Cell & weapon, Environment & environment, OrganismGrid & organismGrid, std::vector<Organism> & organisms, Settings & settings) -> void {
		auto superAttack = !weapon.isModified() ? -1 : weapon.modifier();

		dealDamage(x + 1, y, superAttack, index, environment, organismGrid, organisms, settings);
		dealDamage(x, y + 1, superAttack, index, environment, organismGrid, organisms, settings);
		dealDamage(x - 1, y, superAttack, index, environment, organismGrid, organisms, settings);
		dealDamage(x, y - 1, superAttack, index, environment, organismGrid, organisms, settings);
	}
}
