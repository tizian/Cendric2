#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Enums/DialogueID.h"

struct NPCBean
{
	// npc on map
	sf::Vector2f spriteOffset;
	sf::FloatRect boundingBox;
	std::vector<sf::IntRect> texturePositions;
	sf::Time frameTime;
	std::string name;

	// dialogue
	bool talksActive;
	DialogueID dialogueID;
	sf::IntRect dialogueTexturePositon;
};

const struct NPCBean DEFAULT_NPC =
{
	sf::Vector2f(),
	sf::FloatRect(),
	std::vector<sf::IntRect>(),
	sf::seconds(1),
	"",

	false,
	DialogueID::Void,
	sf::IntRect(0, 0, 250, 250) // that's cendrics dialogue sprite
};