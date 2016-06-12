#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

namespace particles {

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

#ifndef DEG_TO_RAD
#define DEG_TO_RAD M_PI / 180.0f
#endif

inline float randomFloat(float low, float high) {
	return low + static_cast<float> (rand()) / (static_cast<float> (RAND_MAX / (high - low)));
}

inline int randomInt(int low, int high) {
	return (rand() % static_cast<int>(high - low + 1)) + low;
}

inline sf::Color randomColor(const sf::Color &low, const sf::Color &high) {
	sf::Uint8 r, g, b, a;

	if (high.r <= low.r) {
		r = high.r;
	}
	else {
		r = (rand() % static_cast<int>(high.r - low.r + 1)) + low.r;
	}

	if (high.g <= low.g) {
		g = high.g;
	}
	else {
		g = (rand() % static_cast<int>(high.g - low.g + 1)) + low.g;
	}

	if (high.b <= low.b) {
		b = high.b;
	}
	else {
		b = (rand() % static_cast<int>(high.b - low.b + 1)) + low.b;
	}

	if (high.a <= low.a) {
		a = high.a;
	}
	else {
		a = (rand() % static_cast<int>(high.a - low.a + 1)) + low.a;
	}

	return { r, g, b, a };
}

inline sf::Vector2f randomVector2f(const sf::Vector2f &low, const sf::Vector2f &high) {
	float y = low.y + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (high.y - low.y)));
	float x = low.x + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (high.x - low.x)));

	return { x, y };
}

inline float dot(const sf::Vector2f &a, const sf::Vector2f &b) {
	return a.x * b.x + a.y * b.y;
}

inline float lerpFloat(float a, float b, float alpha) {
	return a * (1.0f - alpha) + b * alpha;
}

inline sf::Color lerpColor(const sf::Color &c1, const sf::Color &c2, float alpha) {
	sf::Uint8 r = (sf::Uint8)(c1.r * (1.0f - alpha) + c2.r * alpha);
	sf::Uint8 g = (sf::Uint8)(c1.g * (1.0f - alpha) + c2.g * alpha);
	sf::Uint8 b = (sf::Uint8)(c1.b * (1.0f - alpha) + c2.b * alpha);
	sf::Uint8 a = (sf::Uint8)(c1.a * (1.0f - alpha) + c2.a * alpha);

	return sf::Color(r, g, b, a);
}

}