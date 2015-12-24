#pragma once

#include <SFML/Graphics.hpp>

struct LightData {
	LightData() {};
	LightData(const sf::Vector2f& center, const sf::Vector2f& radius) : center(center), radius(radius) {};
	LightData(const sf::Vector2f& center, const sf::Vector2f& radius, float level) : center(center), radius(radius), brightness(level) {};
	LightData(const sf::Vector2f& center, float radius) : center(center), radius(sf::Vector2f(radius, radius)) {};
	LightData(const sf::Vector2f& center, float radius, float level) : center(center), radius(sf::Vector2f(radius, radius)), brightness(level) {};

	sf::Vector2f center;
	sf::Vector2f radius;
	float brightness = 1.f;
};