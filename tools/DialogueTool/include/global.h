#pragma once

#include "imgui.h"
#include "imgui-sfml.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#ifdef _WIN32
#define IM_NEWLINE "\r\n"
#include <direct.h>
#else
#define IM_NEWLINE "\n"
#endif

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))
#define IM_MAX(_A,_B)       (((_A) >= (_B)) ? (_A) : (_B))

// extern objects
class ApplicationState;
extern ApplicationState* g_state;

#define G_DIA g_state->getDialogue()
#define G_CONF g_state->getConfiguration()
#define ERROR(_MSG) g_state->setError(_MSG)

inline std::string tabs(int count) {
	if (count < 1) return "";
	std::string s;
	while (count--) { s += "\t"; }
	return s;
}