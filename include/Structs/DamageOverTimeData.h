#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Enums/DamageType.h"

struct DamageOverTimeData {
	DamageType damageType = DamageType::VOID;
	int damage = 0;
	sf::Time duration;
};