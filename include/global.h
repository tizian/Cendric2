#pragma once

// Graphic Engine SFML
#include <SFML/Graphics.hpp>

// std library
#include <iostream>

#define PI_F 3.14159265358979f
#define DEG_TO_RAD 57.2957795f
// TODO these values should not be const, but loaded from an .ini file.
#define WINDOW_WIDTH 1250
#define WINDOW_HEIGHT 750
#define BOTTOM_BORDER 100
// Framerate. Values < 30 give interesting results and should not be selected.
#define MAX_FRAME_RATE 80

// cendric color palette
#define CENDRIC_COLOR_BLACK sf::Color::Black
#define CENDRIC_COLOR_PURPLE sf::Color(54, 11, 93, 255)
#define CENDRIC_COLOR_LIGHT_PURPLE sf::Color(114, 66, 200, 255)

const bool DEBUG_RENDERING = false;

class Logger;
class TextProvider;
class ResourceManager;
class InputController; 

// extern objects
extern ResourceManager* g_resourceManager;
extern InputController* g_inputController;
extern Logger* g_logger;
extern TextProvider* g_textProvider;


