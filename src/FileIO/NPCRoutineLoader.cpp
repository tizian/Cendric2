#include "FileIO/NPCRoutineLoader.h"
#include "Map/NPC.h"
#include "Map/NPCRoutine.h"
#include "Screens/WorldScreen.h"
#include "Callbacks/WorldCallback.h"

using namespace luabridge;

NPCRoutineLoader::NPCRoutineLoader(NPCRoutine& routine, WorldScreen* screen) : m_routine(routine) {
	m_worldCallback = new WorldCallback(screen);
}

NPCRoutineLoader::~NPCRoutineLoader() {
	delete m_worldCallback;
}

void NPCRoutineLoader::loadRoutine(bool isInitial) {
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	m_worldCallback->bindFunctions(L);
	getGlobalNamespace(L)
		.beginClass<NPCRoutineLoader>("NPCRoutine")
		.addFunction("wait", &NPCRoutineLoader::wait)
		.addFunction("goToTile", &NPCRoutineLoader::goToTile)
		.addFunction("setLooped", &NPCRoutineLoader::setLooped)
		.addFunction("setTilePosition", &NPCRoutineLoader::setTilePosition)
		.addFunction("setTilePositionForce", &NPCRoutineLoader::setTilePositionForce)
		.addFunction("setDisposed", &NPCRoutineLoader::setDisposed)
		.addFunction("setTalkingActive", &NPCRoutineLoader::setTalkingActive)
		.addFunction("setTalkingActiveForce", &NPCRoutineLoader::setTalkingActiveForce)
		.addFunction("setTalkingEnabled", &NPCRoutineLoader::setTalkingEnabled)
		.addFunction("setReloadEnabled", &NPCRoutineLoader::setReloadEnabled)
		.endClass();

	if (luaL_dofile(L, getResourcePath(m_routine.getID()).c_str()) != 0) {
		g_logger->logError("NPCRoutineLoader", "Cannot read lua script: " + getResourcePath(m_routine.getID()));
		return;
	}

	LuaRef function = getGlobal(L, "loadRoutine");
	if (!function.isFunction()) {
		g_logger->logError("NPCRoutineLoader", "Lua script: " + getResourcePath(m_routine.getID()) + " has no loadRoutine function.");
		return;
	}

	try {
		m_isInitial = isInitial;
		function(this, m_worldCallback);
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
	step.goal.x = x * TILE_SIZE_F;
	step.goal.y = y * TILE_SIZE_F;
	m_routine.addStep(step);
}

void NPCRoutineLoader::setLooped(bool looped) {
	m_routine.setLooped(looped);
}

void NPCRoutineLoader::setTalkingActive(bool active) {
	if (!m_isInitial) return;
	setTalkingActiveForce(active);
}

void NPCRoutineLoader::setTalkingActiveForce(bool active) {
	m_routine.getNPC()->setTalkingActive(active);
}

void NPCRoutineLoader::setTalkingEnabled(bool enabled) {
	m_routine.getNPC()->setTalkingEnabled(enabled);
}

void NPCRoutineLoader::setReloadEnabled(bool enabled) {
	m_routine.getNPC()->setReloadEnabled(enabled);
}

void NPCRoutineLoader::setTilePosition(float x, float y) {
	if (!m_isInitial) return;
	setTilePositionForce(x, y);
}

void NPCRoutineLoader::setTilePositionForce(float x, float y) {
	m_routine.getNPC()->setPosition(sf::Vector2f(x * TILE_SIZE_F, y * TILE_SIZE_F));
}

void NPCRoutineLoader::setDisposed() {
	RoutineStep step;
	step.state = RoutineState::Disappearing;
	m_routine.addStep(step);
}

