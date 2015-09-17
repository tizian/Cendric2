#pragma once

#include <SFML/Graphics.hpp>

struct LightBean
{
	LightBean() {};
	LightBean(const sf::Vector2f& _center, float _radiusX, float _radiusY) 
	{
		center = _center;
		radiusX = _radiusX;
		radiusY = _radiusY;
	}

	sf::Vector2f center;
	float radiusX;
	float radiusY;
};