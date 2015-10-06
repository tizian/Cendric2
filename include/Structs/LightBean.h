#pragma once

#include <SFML/Graphics.hpp>

struct LightBean {
	LightBean() {};
	LightBean(const sf::Vector2f& center, const sf::Vector2f& radius) : center(center), radius(radius) {};
	LightBean(const sf::Vector2f& center, const sf::Vector2f& radius, float level) : center(center), radius(radius), brightness(brightness) {};
	LightBean(const sf::Vector2f& center, float radius) : center(center), radius(sf::Vector2f(radius, radius)) {};
	LightBean(const sf::Vector2f& center, float radius, float level) : center(center), radius(sf::Vector2f(radius, radius)), brightness(brightness) {};

	sf::Vector2f center;
	sf::Vector2f radius;
	float brightness = 1.f;
};