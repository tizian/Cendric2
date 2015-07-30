#pragma once

// Graphic Engine SFML
#include <SFML/Graphics.hpp>

// std library
#include <iostream>
#include <cmath>
#include <functional>
// #define NDEBUG; // uncomment this for final version
#include <assert.h>

#define PI_F 3.14159265358979f
#define DEG_TO_RAD 57.2957795f
// TODO these values should not be const, but loaded from an .ini file.
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

// cendric color palette
#define CENDRIC_COLOR_WHITE sf::Color(255, 255, 255)
#define CENDRIC_COLOR_BLACK sf::Color(0, 0, 0)

#define CENDRIC_COLOR_GREY sf::Color(100, 100, 100)
#define CENDRIC_COLOR_PURPLE sf::Color(54, 11, 93, 255)
#define CENDRIC_COLOR_DARK_PURPLE sf::Color(41, 7, 63, 255)
#define CENDRIC_COLOR_LIGHT_PURPLE sf::Color(114, 66, 200, 255)

#define CENDRIC_COLOR_ELEMENTAL 	sf::Color(144, 33, 7)
#define CENDRIC_COLOR_TWILIGHT 		sf::Color(32, 43, 79)
#define CENDRIC_COLOR_NECROMANCY 	sf::Color(23, 53, 18)
#define CENDRIC_COLOR_DIVINE		sf::Color(178, 129, 52)
#define CENDRIC_COLOR_ILLUSION		sf::Color(78, 37, 75)

#define CENDRIC_COLOR_ELEMENTAL_INACTIVE 	sf::Color(143, 88, 76)
#define CENDRIC_COLOR_TWILIGHT_INACTIVE 	sf::Color(56, 61, 79)
#define CENDRIC_COLOR_NECROMANCY_INACTIVE 	sf::Color(46, 55, 43)
#define CENDRIC_COLOR_DIVINE_INACTIVE		sf::Color(179, 155, 116)
#define CENDRIC_COLOR_ILLUSION_INACTIVE		sf::Color(79, 58, 78)

const bool DEBUG_RENDERING = true;

class Logger;
class TextProvider;
class ResourceManager;
class InputController; 

// extern objects
extern ResourceManager* g_resourceManager;
extern InputController* g_inputController;
extern Logger* g_logger;
extern TextProvider* g_textProvider;


