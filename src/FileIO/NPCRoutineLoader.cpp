#include "FileIO/NPCRoutineLoader.h"
#include "Map/NPC.h"
#include "Map/NPCRoutine.h"
#include "CharacterCore.h"

using namespace std;
using namespace luabridge;

NPCRoutineLoader::NPCRoutineLoader(NPCRoutine& routine, CharacterCore* core) : m_routine(routine) {
	m_core = core;
}

NPCRoutineLoader::~NPCRoutineLoader() {
}

void NPCRoutineLoader::loadRoutine(bool isInitial) {
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	getGlobalNamespace(L)
		.beginClass<NPCRoutineLoader>("NPCRoutine")
		.addFunction("wait", &NPCRoutineLoader::wait)
		.addFunction("goToTile", &NPCRoutineLoader::goToTile)
		.addFunction("setLooped", &NPCRoutineLoader::setLooped)
		.addFunction("isQuestState", &NPCRoutineLoader::isQuestState)
		.addFunction("isConditionFulfilled", &NPCRoutineLoader::isConditionFulfilled)
		.addFunction("setTilePosition", &NPCRoutineLoader::setTilePosition)
		.addFunction("setDisposed", &NPCRoutineLoader::setDisposed)
		.addFunction("setTalkingActive", &NPCRoutineLoader::setTalkingActive)
		.addFunction("setTalkingEnabled", &NPCRoutineLoader::setTalkingEnabled)
		.endClass();

	if (luaL_dofile(L, getPath(m_routine.getID()).c_str()) != 0) {
		g_logger->logError("NPCRoutineLoader", "Cannot read lua script: " + getPath(m_routine.getID()));
		return;
	}

	LuaRef function = getGlobal(L, "loadRoutine");
	if (!function.isFunction()) {
		g_logger->logError("NPCRoutineLoader", "Lua script: " + getPath(m_routine.getID()) + " has no loadRoutine function.");
		return;
	}

	try {
		m_isInitial = isInitial;
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
	step.goal.x = x * TILE_SIZE_F;
	step.goal.y = y * TILE_SIZE_F;
	m_routine.addStep(step);
}

void NPCRoutineLoader::setLooped(bool looped) {
	m_routine.setLooped(looped);
}

void NPCRoutineLoader::setTalkingActive(bool active) {
	if (!m_isInitial) return;
	m_routine.getNPC()->setTalkingActive(active);
}

void NPCRoutineLoader::setTalkingEnabled(bool enabled) {
	m_routine.getNPC()->setTalkingEnabled(enabled);
}

bool NPCRoutineLoader::isConditionFulfilled(const std::string& conditionType, const std::string& condition) const {
	if (condition.empty() || conditionType.empty()) {
		g_logger->logError("NPCRoutineLoader", "Condition and condition type cannot be empty.");
		return false;
	}
	return m_core->isConditionFulfilled(conditionType, condition);
}

bool NPCRoutineLoader::isQuestState(const std::string& questID, const std::string& state) const {
	QuestState questState = resolveQuestState(state);
	if (questState == QuestState::MAX) {
		g_logger->logError("NPCRoutineLoader", "Quest State: [" + state + "] does not exist");
		return false;
	}
	if (questID.empty()) {
		g_logger->logError("NPCRoutineLoader", "Quest ID cannot be empty.");
		return false;
	}
	return m_core->getQuestState(questID) == questState;
}

void NPCRoutineLoader::setTilePosition(float x, float y) {
	if (!m_isInitial) return;
	m_routine.getNPC()->setPosition(sf::Vector2f(x * TILE_SIZE_F, y * TILE_SIZE_F));
}

void NPCRoutineLoader::setDisposed() {
	m_routine.getNPC()->setDisposed();
}

