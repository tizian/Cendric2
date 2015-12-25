#include "FileIO/NPCRoutineLoader.h"
#include "Map/NPC.h"
#include "Map/NPCRoutine.h"

using namespace std;
using namespace luabridge;

NPCRoutineLoader::NPCRoutineLoader(NPCRoutine& routine) : m_routine(routine) {
}

NPCRoutineLoader::~NPCRoutineLoader() {
}

void NPCRoutineLoader::loadRoutine() {
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	getGlobalNamespace(L)
		.beginClass<NPCRoutineLoader>("NPCRoutine")
		.addFunction("wait", &NPCRoutineLoader::wait)
		.addFunction("goToTile", &NPCRoutineLoader::goToTile)
		.addFunction("setLooping", &NPCRoutineLoader::setLooping)
		.endClass();

	if (luaL_dofile(L, m_routine.getID().c_str()) != 0) {
		g_logger->logError("NPCRoutineLoader", "Cannot read lua script: " + m_routine.getID());
		return;
	}

	LuaRef function = getGlobal(L, "loadRoutine");
	if (!function.isFunction()) {
		g_logger->logError("NPCRoutineLoader", "Lua script: " + m_routine.getID() + " has no loadRoutine function.");
		return;
	}

	try {
		function(this);
	}
	catch (LuaException const& e) {
		g_logger->logError("NPCRoutineLoader", "LuaException: " + std::string(e.what()));
	}

	LuaRef velocity = getGlobal(L, "velocity");
	if (velocity.isNumber()) {
		m_routine.setVelocity(velocity.cast<float>());
	}
}

void NPCRoutineLoader::wait(int milliseconds) {
	RoutineStep step;
	step.state = RoutineState::Waiting;
	step.time = sf::milliseconds(milliseconds);
	m_routine.addStep(step);
}

void NPCRoutineLoader::goToTile(float x, float y) {
	RoutineStep step;
	step.state = RoutineState::GoingTo;
	step.goal.x = x * 50.f;
	step.goal.y = y * 50.f;
	m_routine.addStep(step);
}

void NPCRoutineLoader::setLooping(bool looping) {
	m_routine.setLooping(looping);
}
