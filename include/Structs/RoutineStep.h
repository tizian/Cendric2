#pragma once

#include "global.h"

enum class RoutineState {
	VOID,
	Waiting,
	GoingTo,
	Saying,
	Animation,
	Disappearing,
	ReloadLock,
	ReloadUnlock,
	FacingDirection
};

struct RoutineStep {
	RoutineState state;
	sf::Time time;
	sf::Vector2f goal;
	std::string text;
};