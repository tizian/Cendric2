#pragma once

#include "SFML/Graphics.hpp"

#include <iostream>
#include <vector>

struct CutsceneImage final {
	std::string imagePath;
	sf::Vector2f velocity;
};

struct CutsceneText  final {
	sf::Time time;
	std::string text;
	bool centered;
};

struct CutsceneStep  final {
	std::vector<CutsceneImage> images;
	std::vector<CutsceneText> texts;
	sf::Time fadeTime;
};

struct CutsceneData final {
	std::string id = "";
	std::string musicPath = "";
	std::string levelID = "";
	std::string mapID = "";
	sf::Vector2f wordPosition;
	std::vector<CutsceneStep> steps;
};