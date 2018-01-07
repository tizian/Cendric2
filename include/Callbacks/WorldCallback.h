#pragma once

#include "global.h"
#include "LuaBridge/LuaBridge.h"
#include "Structs/TriggerContent.h"

class CharacterCore;
class WorldScreen;

// a callback class that holds queries to be executed by lua on the character core.
// and commands to be executed on a world screen
class WorldCallback final {
public:
	WorldCallback(WorldScreen* screen);
	~WorldCallback();

	// bind the functions to an existing lua state.
	void bindFunctions(luabridge::lua_State* luaState) const;

	// quest queries
	bool isQuestState(const std::string& questID, const std::string& state) const;
	bool isQuestComplete(const std::string& questID) const;
	bool isQuestConditionFulfilled(const std::string& quest, const std::string& condition) const;
	bool isQuestDescriptionUnlocked(const std::string& quest, int description) const;

	// other queries
	bool isConditionFulfilled(const std::string& conditionType, const std::string& condition) const;
	bool hasItem(const std::string& itemID, int amount) const; // this can also query gold. with item id "gold"
	bool isItemEquipped(const std::string& itemID) const; 
	bool isSpellLearned(int spellID);
	bool isSpellEquipped(int spellID);
	int getItemAmount(const std::string& itemID) const; // this can also query gold. with item id "gold"
	int getReputation(const std::string& fractionID) const;
	std::string getGuild() const;

	// commands to be executed by triggers
	void changeQuestState(const std::string& questID, const std::string& state) const;
	void addQuestProgress(const std::string& questID, const std::string& progress) const;
	void addQuestDescription(const std::string& questID, int descriptionID) const;
	void addConditionProgress(const std::string& conditionType, const std::string& condition) const;
	void addReputationProgress(const std::string& fractionID, int amount) const;
	void addHint(const std::string& hint) const;
	void addItem(const std::string& itemID, int amount) const;
	void equipItem(const std::string& itemID) const;
	void removeItem(const std::string& itemID, int amount) const;
	void addGold(int amount) const;
	void removeGold(int amount) const;
	void learnSpell(int spellID) const;
	void setGuild(const std::string& guild) const;
	void startLevel(const std::string& levelID, int x, int y) const;
	void startMap(const std::string& mapID, int x, int y) const;
	void setLevel(const std::string& levelID, int x, int y) const;
	void setMap(const std::string& mapID, int x, int y) const;
	void startCutscene(const std::string& cutsceneID) const;
	void unlockAchievement(const std::string& achievement) const;
	void notifyAchievementCore(const std::string& achievement) const;
	void setWeather(const std::string& mapId, const std::string& weather, int dimming) const;

	// this works only on a map
	void spawnNPC(const std::string& npcID, int x, int y) const;
	// this works only in a level
	void spawnEnemy(luabridge::lua_State* state) const;

private:
	CharacterCore* m_core;
	WorldScreen* m_screen;
};