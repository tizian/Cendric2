#pragma once

// Graphic Engine SFML
#include <SFML/Graphics.hpp>

// std library
#include <iostream>

// TODO these values should not be const, but loaded from an .ini file.
#define WINDOW_WIDTH 1250
#define WINDOW_HEIGHT 750
#define BOTTOM_BORDER 100
// Framerate. Values < 30 give interesting results and should not be selected.
#define MAX_FRAME_RATE 80

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


