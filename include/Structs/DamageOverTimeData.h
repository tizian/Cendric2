#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Enums/DamageType.h"

struct DamageOverTimeData final {
	DamageType damageType = DamageType::VOID;
	int damage = 0;
	sf::Time duration;
	bool isFeared = false;
	bool isStunned = false;
};