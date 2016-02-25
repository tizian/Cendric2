#include "Level/MOBBehavior/ScriptedBehavior/ScriptedBehaviorCallback.h"
#include "Level/MOBBehavior/ScriptedBehavior/ScriptedBehavior.h"
#include "CharacterCore.h"
#include "Level/Enemy.h"

using namespace std;
using namespace luabridge;

ScriptedBehaviorCallback::ScriptedBehaviorCallback(const std::string& luaPath, CharacterCore* core, Enemy* enemy) {
	m_core = core;
	m_enemy = enemy;

	m_success = loadLua(luaPath);
}

ScriptedBehaviorCallback::~ScriptedBehaviorCallback() {
	lua_close(m_L);
}

void ScriptedBehaviorCallback::setScriptedBehavior(ScriptedBehavior* behavior) {
	m_scriptedBehavior = behavior;
}


bool ScriptedBehaviorCallback::loadLua(const std::string& path) {
	m_L = luaL_newstate();
	luaL_openlibs(m_L);
	getGlobalNamespace(m_L)
		.beginClass<ScriptedBehaviorCallback>("Behavior")
		.addFunction("isQuestState", &ScriptedBehaviorCallback::isQuestState)
		.addFunction("isQuestComplete", &ScriptedBehaviorCallback::isQuestComplete)
		.addFunction("isConditionFulfilled", &ScriptedBehaviorCallback::isConditionFulfilled)
		.addFunction("getPosX", &ScriptedBehaviorCallback::getPosX)
		.addFunction("getPosY", &ScriptedBehaviorCallback::getPosY)
		.addFunction("say", &ScriptedBehaviorCallback::say)
		.addFunction("wait", &ScriptedBehaviorCallback::wait)
		.addFunction("setKilled", &ScriptedBehaviorCallback::setKilled)
		.addFunction("leaveLevel", &ScriptedBehaviorCallback::leaveLevel)
		.endClass();

	if (luaL_dofile(m_L, path.c_str()) != 0) {
		g_logger->logError("ScriptedBehaviorCallback", "Cannot read lua script: " + path);
		return false;
	}

	LuaRef update = getGlobal(m_L, "update");
	if (!update.isFunction()) {
		g_logger->logError("ScriptedBehaviorCallback", "Lua script: " + path + " has no update function.");
		return false;
	}

	return true;
}

void ScriptedBehaviorCallback::setKilled() {
	m_enemy->notifyKilled();
}

void ScriptedBehaviorCallback::leaveLevel() {
	m_enemy->setDisposed();
}

bool ScriptedBehaviorCallback::isLoaded() const {
	return m_success;
}

void ScriptedBehaviorCallback::update() {
	LuaRef update = getGlobal(m_L, "update");
	
	try {
		update(this);
	}
	catch (LuaException const& e) {
		g_logger->logError("ScriptedBehaviorCallback", "LuaException: " + std::string(e.what()));
	}
}

bool ScriptedBehaviorCallback::isQuestComplete(const std::string& questID) const {
	if (questID.empty()) {
		g_logger->logError("ScriptedBehaviorCallback", "Quest ID cannot be empty.");
		return false;
	}
	return m_core->isQuestComplete(questID);
}

bool ScriptedBehaviorCallback::isConditionFulfilled(const std::string& condition) const {
	if (condition.empty()) {
		g_logger->logError("ScriptedBehaviorCallback", "Condition cannot be empty.");
		return false;
	}
	return m_core->isConditionFulfilled(condition);
}

int ScriptedBehaviorCallback::getPosX() const {
	return static_cast<int>(std::floor(m_enemy->getPosition().x));
}

int ScriptedBehaviorCallback::getPosY() const {
	return static_cast<int>(std::floor(m_enemy->getPosition().y));
}

void ScriptedBehaviorCallback::say(const std::string& text, int seconds) {
	m_scriptedBehavior->say(text, seconds);
}

void ScriptedBehaviorCallback::wait(int seconds) {
	m_scriptedBehavior->wait(seconds);
}

bool ScriptedBehaviorCallback::isQuestState(const std::string& questID, const std::string& state) const {
	QuestState questState = resolveQuestState(state);
	if (questState == QuestState::VOID) {
		g_logger->logError("ScriptedBehaviorCallback", "Quest State: [" + state + "] does not exist");
		return false;
	}
	if (questID.empty()) {
		g_logger->logError("ScriptedBehaviorCallback", "Quest ID cannot be empty.");
		return false;
	}
	return m_core->getQuestState(questID) == questState;
}

