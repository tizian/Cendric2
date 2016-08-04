#pragma once

#include "imgui.h"
#include "imgui-sfml.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#ifdef _WIN32
	#include <direct.h>
#endif

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))
#define IM_MAX(_A,_B)       (((_A) >= (_B)) ? (_A) : (_B))

// extern objects
class ApplicationState;
extern ApplicationState* g_state;

#define G_DIA g_state->getDialogue()
#define G_CONF g_state->getConfiguration()
#define ERROR(_MSG) g_state->setLogMessage(_MSG, sf::Color::Red)
#define LOG(_MSG) g_state->setLogMessage(_MSG, sf::Color::White)

inline std::string tabs(int count) {
	if (count < 1) return "";
	std::string s;
	while (count--) { s += "\t"; }
	return s;
}

inline std::string duplicateApostrophs(const std::string& in) {
	std::string out;
	for (auto c : in) {
		if (c == '\'') {
			out += c;
		}
		out += c;
	}

	return out;
}

inline std::string getFileName(const std::string filePath) {
	std::string fileName = filePath;
	const size_t last_slash_idx = fileName.find_last_of("\\/");
	if (std::string::npos != last_slash_idx) {
		fileName.erase(0, last_slash_idx + 1);
	}
	const size_t period_idx = fileName.rfind('.');
	if (std::string::npos != period_idx) {
		fileName.erase(period_idx);
	}
	return fileName;
}