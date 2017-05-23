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

	// used to resolve a light string (x offset, y offset, width, height, brightness)
	// and write into the light data. Returns whether it was successful.
	static bool resolveLightString(const std::string& lightString, LightData& lightData) {
		size_t pos = 0;
		std::string value = lightString;

		if ((pos = value.find(",")) == std::string::npos) {
			return false;
		}
		lightData.center.x = (float)std::stof(value.substr(0, pos));
		value.erase(0, pos + 1);

		if ((pos = value.find(",")) == std::string::npos) {
			return false;
		}
		lightData.center.y = (float)std::stof(value.substr(0, pos));
		value.erase(0, pos + 1);

		if ((pos = value.find(",")) == std::string::npos) {
			return false;
		}
		lightData.radius.x = (float)std::stof(value.substr(0, pos));
		value.erase(0, pos + 1);

		if ((pos = value.find(",")) == std::string::npos) {
			return false;
		}
		lightData.radius.y = (float)std::stof(value.substr(0, pos));
		value.erase(0, pos + 1);

		lightData.brightness = (float)std::stof(value);
		return true;
	}
};