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
	TalkingEnabled,
	TalkingDisabled,
	TalkingActive,
	TalkingInactive,
	FacingDirection
};

struct RoutineStep final {
	RoutineState state;
	sf::Time time;
	sf::Vector2f goal;
	std::string text;
};