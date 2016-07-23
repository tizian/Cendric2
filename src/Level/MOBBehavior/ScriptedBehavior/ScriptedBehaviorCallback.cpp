#include "Level/MOBBehavior/ScriptedBehavior/ScriptedBehaviorCallback.h"
#include "Level/MOBBehavior/ScriptedBehavior/ScriptedBehavior.h"
#include "CharacterCore.h"
#include "Level/Enemy.h"
#include "Screens/WorldScreen.h"
#include "Callbacks/WorldCallback.h"

using namespace std;
using namespace luabridge;

ScriptedBehaviorCallback::ScriptedBehaviorCallback(const std::string& luaPath, Enemy* enemy) {
	m_worldCallback = new WorldCallback(dynamic_cast<WorldScreen*>(enemy->getScreen()));
	m_enemy = enemy;

	m_success = loadLua(luaPath);
}

ScriptedBehaviorCallback::~ScriptedBehaviorCallback() {
}

void ScriptedBehaviorCallback::setScriptedBehavior(ScriptedBehavior* behavior) {
	m_scriptedBehavior = behavior;
}

bool ScriptedBehaviorCallback::loadLua(const std::string& path) {
	m_L = luaL_newstate();
	luaL_openlibs(m_L);
	m_worldCallback->bindFunctions(m_L);
	getGlobalNamespace(m_L)
		.beginClass<ScriptedBehaviorCallback>("Behavior")
		.addFunction("getPosX", &ScriptedBehaviorCallback::getPosX)
		.addFunction("getPosY", &ScriptedBehaviorCallback::getPosY)
		.addFunction("say", &ScriptedBehaviorCallback::say)
		.addFunction("wait", &ScriptedBehaviorCallback::wait)
		.addFunction("setDisposed", &ScriptedBehaviorCallback::setDisposed)
		.addFunction("leaveLevel", &ScriptedBehaviorCallback::leaveLevel)
		.addFunction("addHint", &ScriptedBehaviorCallback::addHint)
		.addFunction("setMovingTarget", &ScriptedBehaviorCallback::setMovingTarget)
		.addFunction("resetMovingTarget", &ScriptedBehaviorCallback::resetMovingTarget)
		.endClass();

	if (luaL_dofile(m_L, getResourcePath(path).c_str()) != 0) {
		
		g_logger->logError("ScriptedBehaviorCallback", "Cannot read lua script: " + getResourcePath(path));
		return false;
	}

	LuaRef update = getGlobal(m_L, "update");
	if (update.isFunction()) {
		m_hasUpdateFunc = true;
	}

	LuaRef onDeath = getGlobal(m_L, "onDeath");
	if (onDeath.isFunction()) {
		m_hasDeathFunc = true;
	}

	return true;
}

void ScriptedBehaviorCallback::setDisposed() {
	m_enemy->notifyKilled();
	m_enemy->notifyLooted();
}

void ScriptedBehaviorCallback::leaveLevel() {
	m_enemy->setDisposed();
}

void ScriptedBehaviorCallback::setMovingTarget(int x, int y) {
	m_enemy->setMovingTarget(x, y);
}

void ScriptedBehaviorCallback::resetMovingTarget() {
	m_enemy->resetMovingTarget();
}

bool ScriptedBehaviorCallback::isLoaded() const {
	return m_success;
}

void ScriptedBehaviorCallback::update() {
	if (!m_hasUpdateFunc) return;
	LuaRef updateFunc = getGlobal(m_L, "update");
	
	try {
		updateFunc(this, m_worldCallback);
	}
	catch (LuaException const& e) {
		g_logger->logError("ScriptedBehaviorCallback", "LuaException: " + std::string(e.what()));
	}
}

void ScriptedBehaviorCallback::onDeath() {
	if (!m_hasDeathFunc) return;
	LuaRef deathFunc = getGlobal(m_L, "onDeath");

	try {
		deathFunc(this, m_worldCallback);
	}
	catch (LuaException const& e) {
		g_logger->logError("ScriptedBehaviorCallback", "LuaException: " + std::string(e.what()));
	}
}

void ScriptedBehaviorCallback::addHint(const std::string& hint) {
	dynamic_cast<WorldScreen*>(m_enemy->getScreen())->notifyHintAdded(hint);
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


