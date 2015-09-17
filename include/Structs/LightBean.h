#pragma once

#include <SFML/Graphics.hpp>

struct LightBean
{
	LightBean() {};
	LightBean(const sf::Vector2f& center, const sf::Vector2f& radius) : center(center), radius(radius) {}
	LightBean(const sf::Vector2f& center, float radius) : center(center), radius(sf::Vector2f(radius, radius)) {}

	sf::Vector2f center;
	sf::Vector2f radius;
};