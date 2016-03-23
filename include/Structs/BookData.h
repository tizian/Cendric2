#pragma once

#include "global.h"

struct BookPage {
	std::string title;
	std::string content;
};

struct BookData {
	int skinNr;
	sf::Vector2f position;
	std::string title;
	std::vector<BookPage> pages;
};