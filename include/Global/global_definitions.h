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
#include <cassert>

// versioning
#define CENDRIC_VERSION_NR "1.0.0"

// Debug version
//#define DEBUG

// Create savegame and screenshot folders outside of the Cendric directory at locations determined by the operating system. (e.g. User/Documents/Cendric on Windows or ~/Library/Application Support/Cendric on Mac)
#define EXTERNAL_DOCUMENTS_FOLDER

// Resolve .app package internal resource path for mac builds
// #define APPLE_APP_BUILD

// Steam builds
//#define STEAM

// max frame time (in seconds)
#define MAX_FRAME_TIME 0.05f

// const canvas size (window can be rescaled though)
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

// fixed tile size (width and height) for level and map
#define TILE_SIZE_F 50.f
#define TILE_SIZE 50
