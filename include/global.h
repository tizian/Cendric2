#pragma once

#include "Global/global_definitions.h"
#include "Global/global_math.h"
#include "Global/global_color.h"

#define NOP []() {}

#define CLEAR_VECTOR(V) for (auto it : V) {delete it;}; V.clear();

// extern objects
class Logger;
class TextProvider;
class ResourceManager;
class InputController;
class DatabaseManager;

extern DatabaseManager* g_databaseManager;
extern ResourceManager* g_resourceManager;
extern InputController* g_inputController;
extern Logger* g_logger;
extern TextProvider* g_textProvider;
extern sf::RenderTexture* g_renderTexture;

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
