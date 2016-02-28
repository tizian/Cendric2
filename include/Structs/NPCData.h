#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

struct NPCData {
	// npc on map
	int objectID;
	std::string id;
	sf::Vector2f position;
	sf::FloatRect boundingBox;
	std::string spritesheetpath;
	std::string routineID;

	// dialogue
	bool talksActive;
	std::string dialogueID;
	sf::IntRect dialogueTexturePositon;
};

const struct NPCData DEFAULT_NPC =
{
	-1,
	"",
	sf::Vector2f(0.f, 0.f),
	sf::FloatRect(0.f, 0.f, 50.f, 50.f),
	"",
	"",

	false,
	"",
	sf::IntRect(0, 0, 250, 250) // that's cendrics dialogue sprite
};