#pragma once

#include <SFML/Graphics.hpp>

struct LightData final {
	LightData() {};
	LightData(const sf::Vector2f& center, const sf::Vector2f& radius) : center(center), radius(radius) {};
	LightData(const sf::Vector2f& center, const sf::Vector2f& radius, float brightness) : center(center), radius(radius), brightness(brightness) {};
	LightData(const sf::Vector2f& center, float radius) : center(center), radius(sf::Vector2f(radius, radius)) {};
	LightData(const sf::Vector2f& center, float radius, float brightness) : center(center), radius(sf::Vector2f(radius, radius)), brightness(brightness) {};

	sf::Vector2f center;
	sf::Vector2f radius;
	float brightness = 1.f;
};