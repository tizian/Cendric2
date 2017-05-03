#include "Level/MOBBehavior/ScriptedBehavior/ScriptedBehaviorCallback.h"
#include "Level/MOBBehavior/ScriptedBehavior/ScriptedBehavior.h"
#include "CharacterCore.h"
#include "Level/Enemy.h"
#include "Screens/WorldScreen.h"
#include "Callbacks/WorldCallback.h"
#include "Structs/RoutineStep.h"

using namespace luabridge;

ScriptedBehaviorCallback::ScriptedBehaviorCallback(const std::string& luaPath, Enemy* enemy) {
	m_worldCallback = new WorldCallback(dynamic_cast<WorldScreen*>(enemy->getScreen()));
	m_enemy = enemy;
	m_luaPath = luaPath;
}

ScriptedBehaviorCallback::~ScriptedBehaviorCallback() {
}

void ScriptedBehaviorCallback::setScriptedBehavior(ScriptedBehavior* behavior) {
	m_scriptedBehavior = behavior;
	m_success = loadLua(m_luaPath, behavior);
}

bool ScriptedBehaviorCallback::loadLua(const std::string& path, ScriptedBehavior* behavior) {
	m_L = luaL_newstate();
	luaL_openlibs(m_L);
	m_worldCallback->bindFunctions(m_L);
	getGlobalNamespace(m_L)
		.beginClass<ScriptedBehaviorCallback>("Behavior")
		.addFunction("getPosX", &ScriptedBehaviorCallback::getPosX)
		.addFunction("getPosY", &ScriptedBehaviorCallback::getPosY)
		.addFunction("setPosition", &ScriptedBehaviorCallback::setPosition)
		.addFunction("say", &ScriptedBehaviorCallback::say)
		.addFunction("wait", &ScriptedBehaviorCallback::wait)
		.addFunction("setFacingLeft", &ScriptedBehaviorCallback::setFacingLeft)
		.addFunction("setFacingRight", &ScriptedBehaviorCallback::setFacingRight)
		.addFunction("setDisposed", &ScriptedBehaviorCallback::setDisposed)
		.addFunction("leaveLevel", &ScriptedBehaviorCallback::leaveLevel)
		.addFunction("addHint", &ScriptedBehaviorCallback::addHint)
		.addFunction("setMovingTarget", &ScriptedBehaviorCallback::setMovingTarget)
		.addFunction("resetMovingTarget", &ScriptedBehaviorCallback::resetMovingTarget)
		.addFunction("executeFightAnimation", &ScriptedBehaviorCallback::executeFightAnimation)
		.addFunction("gotoTile", &ScriptedBehaviorCallback::gotoTile)
		.addFunction("executeSpell", &ScriptedBehaviorCallback::executeSpell)
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

	LuaRef routine = getGlobal(m_L, "routine");
	if (routine.isFunction()) {
		m_isRoutineFunction = true;
		routine(this);
		m_isRoutineFunction = false;
	}

	return true;
}

void ScriptedBehaviorCallback::setPosition(int x, int y) {
	m_enemy->setPosition(sf::Vector2f((float)x, (float)y));
}

void ScriptedBehaviorCallback::gotoTile(float x, float y) {
	if (m_isRoutineFunction) {
		RoutineStep step;
		step.state = RoutineState::GoingTo;
		step.goal.x = x * TILE_SIZE_F;
		step.goal.y = (y + 1) * TILE_SIZE_F;
		m_scriptedBehavior->addRoutineStep(step);
	}
}

void ScriptedBehaviorCallback::setDisposed() {
	if (m_isRoutineFunction) {
		RoutineStep step;
		step.state = RoutineState::Disappearing;
		m_scriptedBehavior->addRoutineStep(step);
	}
	else {
		m_enemy->notifyKilled();
		m_enemy->notifyLooted();
	}
}

void ScriptedBehaviorCallback::executeFightAnimation() {
	if (m_isRoutineFunction) {
		RoutineStep step;
		step.state = RoutineState::Animation;
		m_scriptedBehavior->addRoutineStep(step);
	}
	else {
		m_enemy->executeDefaultFightAnimation(true);
	}
}

void ScriptedBehaviorCallback::executeSpell(int spell, int x, int y) {
	if (m_isRoutineFunction) {
		RoutineStep step;
		step.state = RoutineState::Spell;
		step.goal = sf::Vector2f(static_cast<float>(x), static_cast<float>(y));
		step.text = std::to_string(spell);
		m_scriptedBehavior->addRoutineStep(step);
	}
	else {
		m_enemy->executeSpell(spell, sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
	}
}

void ScriptedBehaviorCallback::setFacingLeft() {
	if (m_isRoutineFunction) {
		RoutineStep step;
		step.state = RoutineState::FacingDirection;
		step.goal = sf::Vector2f(-1.f, 0.f);
		m_scriptedBehavior->addRoutineStep(step);
	}
	else {
		m_enemy->setFacingRight(false);
	}
}

void ScriptedBehaviorCallback::setFacingRight() {
	if (m_isRoutineFunction) {
		RoutineStep step;
		step.state = RoutineState::FacingDirection;
		step.goal = sf::Vector2f(1.f, 0.f);
		m_scriptedBehavior->addRoutineStep(step);
	}
	else {
		m_enemy->setFacingRight(true);
	}
}

void ScriptedBehaviorCallback::leaveLevel() {
	if (m_isRoutineFunction) return;
	m_enemy->setDisposed();
}

void ScriptedBehaviorCallback::setMovingTarget(int x, int y) {
	if (m_isRoutineFunction) return;
	m_enemy->setMovingTarget(x, y);
}

void ScriptedBehaviorCallback::resetMovingTarget() {
	if (m_isRoutineFunction) return;
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
	if (m_isRoutineFunction) return;
	dynamic_cast<WorldScreen*>(m_enemy->getScreen())->notifyHintAdded(hint);
}

int ScriptedBehaviorCallback::getPosX() const {
	return static_cast<int>(std::floor(m_enemy->getPosition().x));
}

int ScriptedBehaviorCallback::getPosY() const {
	return static_cast<int>(std::floor(m_enemy->getPosition().y));
}

void ScriptedBehaviorCallback::say(const std::string& text, int seconds) {
	if (seconds < 1 || text.empty()) return;
	if (m_isRoutineFunction) {
		RoutineStep step;
		step.state = RoutineState::Saying;
		step.text = text;
		step.time = sf::seconds(static_cast<float>(seconds));
		m_scriptedBehavior->addRoutineStep(step);
	}
	else {
		m_scriptedBehavior->say(text, seconds);
	}
}

void ScriptedBehaviorCallback::wait(int seconds) {
	if (seconds < 1) return;
	if (m_isRoutineFunction) {
		RoutineStep step;
		step.state = RoutineState::Waiting;
		step.time = sf::seconds(static_cast<float>(seconds));
		m_scriptedBehavior->addRoutineStep(step);
	}
	else {
		m_scriptedBehavior->wait(seconds);
	}
}


