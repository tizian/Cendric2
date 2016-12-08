#pragma once

// Graphic & Sound Engine SFML
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// std library
#include <iostream>
#include <cmath>
#include <functional>
#include <set>
#include <map>
#include <vector>
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

inline float clamp(float v, float low, float high) {
	if (v < low) return low;
	if (v > high) return high;
	return v;
}

inline float linearTween(float t, float b, float c, float d) {
	return c * t / d + b;
}

inline float easeInOutQuad(float t, float b, float c, float d) {
	t /= d / 2;
	if (t < 1) return c / 2 * t * t + b;
	t--;
	return -c / 2 * (t *  (t - 2) - 1) + b;
}

inline float norm(const sf::Vector2f& v) {
	return std::sqrt(v.x * v.x + v.y * v.y);
}

inline float dist(const sf::Vector2f& v1, const sf::Vector2f& v2) {
	return norm(v1 - v2);
}

inline float randomFloat(float low, float high) {
	return low + static_cast<float> (rand()) / (static_cast<float> (RAND_MAX / (high - low)));
}

inline int round_int(float r) {
	return static_cast<int>((r > 0.0) ? (r + 0.5) : (r - 0.5));
}

inline bool epsIntersect(const sf::FloatRect& rect1, const sf::FloatRect& rect2) {
	sf::FloatRect intersection;
	if (!rect1.intersects(rect2, intersection)) return false;
	return (intersection.width > Epsilon && intersection.height > Epsilon);
}

template <typename T>
inline bool contains(const std::set<T>& c, T item) {
	return c.find(item) != c.end();
}

template <typename T, typename U>
inline bool contains(const std::map<T, U>& c, T item) {
	return c.find(item) != c.end();
}

template <typename T>
inline bool contains(const std::vector<T>& c, T item) {
	return std::find(c.begin(), c.end(), item) != c.end();
}

// convenience method for updating time values 
// The frame time is subtracted from the time but 
// if the time falls below sf::Time::Zero, it is set to sf::Time::Zero.
inline void updateTime(sf::Time& time, const sf::Time& frameTime) {	
	if (time == sf::Time::Zero) return;
	time -= frameTime;
	if (time < sf::Time::Zero) time = sf::Time::Zero;
}

// versioning
#define CENDRIC_VERSION_NR "0.3.2"

// const canvas size (window can be rescaled though)
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

// fixed tile size (width and height) for level and map
#define TILE_SIZE_F 50.f
#define TILE_SIZE 50

// color palette

// DON'T ever change white, black and transparent!!
extern const sf::Color COLOR_WHITE;
extern const sf::Color COLOR_BLACK;
extern const sf::Color COLOR_TRANSPARENT;

extern const sf::Color COLOR_TRANS_BLACK;
extern const sf::Color COLOR_TRANS_GREY;
extern const sf::Color COLOR_TRANS_WHITE;

extern const sf::Color COLOR_GOOD;
extern const sf::Color COLOR_NEUTRAL;
extern const sf::Color COLOR_BAD;

extern const sf::Color COLOR_DAMAGE_ALLY;
extern const sf::Color COLOR_DAMAGE_ENEMY;
extern const sf::Color COLOR_HEAL_ALLY;
extern const sf::Color COLOR_HEAL_ENEMY;

extern const sf::Color COLOR_DAMAGED;
extern const sf::Color COLOR_HEALED;
extern const sf::Color COLOR_INTERACTIVE;

extern const sf::Color COLOR_LIGHT_GREY;
extern const sf::Color COLOR_MEDIUM_GREY;
extern const sf::Color COLOR_GREY;
extern const sf::Color COLOR_DARK_GREY;
extern const sf::Color COLOR_LIGHT_PURPLE;
extern const sf::Color COLOR_MEDIUM_PURPLE;
extern const sf::Color COLOR_PURPLE;
extern const sf::Color COLOR_LIGHT_BROWN;
extern const sf::Color COLOR_DARK_BROWN;

extern const sf::Color COLOR_ELEMENTAL;
extern const sf::Color COLOR_TWILIGHT;
extern const sf::Color COLOR_NECROMANCY;
extern const sf::Color COLOR_DIVINE;

extern const sf::Color COLOR_ELEMENTAL_INACTIVE;
extern const sf::Color COLOR_TWILIGHT_INACTIVE;
extern const sf::Color COLOR_NECROMANCY_INACTIVE;
extern const sf::Color COLOR_DIVINE_INACTIVE;

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

extern std::string g_resourcePath;
extern std::string g_documentsPath;

// Platform independent resource path handling
inline std::string getResourcePath(const std::string& path) {
	return g_resourcePath + path;
}

// Platform independent document path handling
inline std::string getDocumentsPath(const std::string& path) {
	return g_documentsPath + path;
}