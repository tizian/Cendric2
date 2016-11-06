#pragma once

#include "global.h"

// overkill party timed feature

struct PartyData {
	int currentID;
	sf::Time timeLeft;
};

class PartyHandler {
public:
	PartyHandler() {}

	// add file io
	// add db managing (only read)

	PartyData& getData() { return m_data; }


private:
	PartyData m_data;
};