#include "Level/BossLevel.h"
#include "Screens/WorldScreen.h"
#include "Callbacks/WorldCallback.h"

using namespace luabridge;

BossLevel::BossLevel(WorldScreen* screen) {
	m_worldCallback = new WorldCallback(screen);
}

BossLevel::~BossLevel() {
}

bool BossLevel::loadLua(const std::string& luaPath) {
	m_L = luaL_newstate();
	luaL_openlibs(m_L);
	m_worldCallback->bindFunctions(m_L);

	if (luaL_dofile(m_L, getResourcePath(luaPath).c_str()) != 0) {
		g_logger->logError("BossLevel", "Cannot read lua script: " + getResourcePath(luaPath));
		return false;
	}

	LuaRef onWin = getGlobal(m_L, "onWin");
	if (!onWin.isFunction()) {
		g_logger->logError("BossLevel", "No onWin function found in " + getResourcePath(luaPath));
		return false;
	}

	LuaRef onLose = getGlobal(m_L, "onLose");
	if (!onLose.isFunction()) {
		g_logger->logError("BossLevel", "No onLose function found in " + getResourcePath(luaPath));
		return false;
	}
	
	return true;
}

void BossLevel::executeOnWin() const {
	LuaRef onWin = getGlobal(m_L, "onWin");

	try {
		onWin(m_worldCallback);
	}
	catch (LuaException const& e) {
		g_logger->logError("BossLevel", "LuaException: " + std::string(e.what()));
	}
}

void BossLevel::executeOnLose() const {
	LuaRef onLose = getGlobal(m_L, "onLose");

	try {
		onLose(m_worldCallback);
	}
	catch (LuaException const& e) {
		g_logger->logError("BossLevel", "LuaException: " + std::string(e.what()));
	}
}
