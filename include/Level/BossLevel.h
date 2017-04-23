#pragma once

#include "global.h"

#include "LuaBridge/LuaBridge.h"

class WorldScreen;
class WorldCallback;

// the bosslevel part of a level, including lua callback
class BossLevel final {
public:
	BossLevel(WorldScreen* screen);
	~BossLevel();

	bool loadLua(const std::string& luaPath);

	void executeOnWin() const;
	void executeOnLose() const;

private:
	WorldCallback* m_worldCallback;
	luabridge::lua_State* m_L = nullptr;
};