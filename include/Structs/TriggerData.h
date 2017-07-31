#pragma once

#include <SFML/Graphics.hpp>

#include "Structs/TriggerContent.h"

struct TriggerData final {
	std::string worldID;
	sf::FloatRect triggerRect;
	// the object ID, used to save which triggers are already triggered and won't reappear again.
	int objectID;
	// if this is true, the trigger will reappear after the next level/map reload after it has been triggered
	bool isPersistent = false;
	// if this is true, the player has to additionally press the "up" key to trigger. Used for level exits.
	bool isKeyGuarded = false;
	// is the condition fulfilled? This property gets updated when a condition changes.
	bool isTriggerable = false;
	// is this trigger forced? if true, it gets immediately executed even if the player spawns on it
	bool isForced = false;
	// trigger conditions
	std::vector<Condition> conditions;
	// the content of this trigger, will get executed on trigger.
	std::vector<TriggerContent> content;
};