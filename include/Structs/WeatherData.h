#pragma once

#include <SFML/Graphics.hpp>

struct WeatherData final {
	float ambientDimming = 0.f;
	float lightDimming = 0.f;
	std::string weather = "";
};