
#include "food.h"

Food::Food(Type type, u8 energy) : type(type), energy(energy), age(0) {}
Food::Food(Type type, u8 energy, u32 age) : type(type), energy(energy), age(age) {}

auto Food::tick() -> void {
	++age;
}

auto Food::getType() const -> Type {
	return type;
}

auto Food::getEnergy() const -> u8 {
	return energy;
}

auto Food::getAge() const -> u32 {
	return age;
}

auto Food::addEnergy(i32 energy) -> void {
	this->energy += energy;
}
