#pragma once

// Graphic & Sound Engine SFML
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// std library
#include <iostream>
#include <cmath>
#include <functional>
// #define NDEBUG; // uncomment this for final version
#include <assert.h>

#undef M_PI
#define M_PI         3.14159265358979323846f
#define M_TWOPI		 6.28318530717958647692f
#define INV_PI       0.31830988618379067154f
#define INV_TWOPI    0.15915494309189533577f

#define Epsilon 1e-3f

inline float radToDeg(float rad) {
	return rad * (180.f / M_PI);
}

inline float degToRad(float deg) {
	return deg * (M_PI / 180.f);
}

inline float lerp(float t, float v1, float v2) {
	return (1.f - t) * v1 + t * v2;
}

inline float norm(const sf::Vector2f& v) {
	return std::sqrt(v.x * v.x + v.y * v.y);
}

inline float dist(const sf::Vector2f& v1, const sf::Vector2f& v2) {
	return norm(v1 - v2);
}

inline bool epsIntersect(const sf::FloatRect& rect1, const sf::FloatRect& rect2) {
	sf::FloatRect intersection;
	if (!rect1.intersects(rect2, intersection)) return false;
	return (intersection.width > Epsilon && intersection.height > Epsilon);
}

// this function can be changed if the resource path is
// not the same as the executables path
inline std::string getPath(const std::string& path) {
	return path;
}

// versioning
#define CENDRIC_VERSION_NR "0.2.0"

// const canvas size (window can be rescaled though)
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

// fixed tile size (width and height) for level and map
#define TILE_SIZE_F 50.f
#define TILE_SIZE 50

// color palette

// DON'T ever change white, black and transparent!!
#define COLOR_WHITE			sf::Color(255, 255, 255)
#define COLOR_BLACK			sf::Color(0, 0, 0)
#define COLOR_TRANSPARENT	sf::Color(0, 0, 0, 0)

#define COLOR_TRANS_BLACK	sf::Color(0, 0, 0, 130)
#define COLOR_TRANS_GREY	sf::Color(150, 150, 150, 100)
#define COLOR_TRANS_WHITE	sf::Color(255, 255, 255, 100)

#define COLOR_GOOD			sf::Color(0, 255, 0)
#define COLOR_NEUTRAL		sf::Color(255, 255, 0)
#define COLOR_BAD			sf::Color(255, 0, 0)

#define COLOR_DAMAGED		sf::Color(255, 0, 0)
#define COLOR_HEALED		sf::Color(0, 255, 0)
#define COLOR_LOOTABLE		sf::Color(255, 0, 0)

#define COLOR_LIGHT_GREY	sf::Color(200, 200, 200)
#define COLOR_MEDIUM_GREY	sf::Color(150, 150, 150)
#define COLOR_GREY			sf::Color(100, 100, 100)
#define COLOR_DARK_GREY		sf::Color(50, 50, 50)
#define COLOR_PURPLE		sf::Color(54, 11, 93, 255)
#define COLOR_DARK_PURPLE	sf::Color(41, 7, 63, 255)
#define COLOR_LIGHT_PURPLE	sf::Color(197, 175, 206, 251)

#define COLOR_ELEMENTAL 	sf::Color(144, 33, 7)
#define COLOR_TWILIGHT 		sf::Color(32, 43, 79)
#define COLOR_NECROMANCY 	sf::Color(23, 53, 18)
#define COLOR_DIVINE		sf::Color(178, 129, 52)

#define COLOR_ELEMENTAL_INACTIVE 	sf::Color(143, 88, 76)
#define COLOR_TWILIGHT_INACTIVE 	sf::Color(56, 61, 79)
#define COLOR_NECROMANCY_INACTIVE 	sf::Color(46, 55, 43)
#define COLOR_DIVINE_INACTIVE		sf::Color(179, 155, 116)

class Logger;
class TextProvider;
class ResourceManager;
class InputController;
class DatabaseManager;

// extern objects
extern DatabaseManager* g_databaseManager;
extern ResourceManager* g_resourceManager;
extern InputController* g_inputController;
extern Logger* g_logger;
extern TextProvider* g_textProvider;
extern sf::RenderWindow* g_renderWindow;


