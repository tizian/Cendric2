#pragma once

#include "global.h"

class WorldScreen;

// those can be triggered by triggers, dialogues, routines, npc behavior, etc.
enum class TriggerContentType {
	VOID,
	QuestStateChange,
	QuestConditionProgress,
	QuestDescriptionProgress,
	ConditionProgress,
	ReputationProgress,
	ItemChange,
	GoldChange,
	ItemEquip,
	Hint,
	MapEntry,
	LevelEntry,
	Cutscene,
	LearnSpell,
	LearnModifier,
	Weather,
	SetGuild,
	SetLevel,
	SetMap,
	SetForcedMap,
	AchievementUnlocked,
	MAX,
};

// holds a part of the trigger content.
// the trigger content type decides which of 
// the attributes are used.
struct TriggerContent final {
	TriggerContent() : type(TriggerContentType::VOID) {};
	TriggerContent(TriggerContentType type_) : type(type_) {};
	TriggerContentType type = TriggerContentType::VOID;
	std::string s1 = "";
	std::string s2 = "";
	int i1 = 0;
	int i2 = 0;

	bool isValid() const;

	// executes the trigger on the given world screen
	static void executeTrigger(const TriggerContent& content, WorldScreen* screen);

	// creates trigger content for the given events. 
	static TriggerContent changeQuestState(const std::string& questID, const std::string& state);
	static TriggerContent addQuestProgress(const std::string& questID, const std::string& progress);
	static TriggerContent addQuestDescription(const std::string& questID, int descriptionID);
	static TriggerContent addConditionProgress(const std::string& conditionType, const std::string& condition);
	static TriggerContent addReputationProgress(const std::string& fractionID, int amount);
	static TriggerContent addHint(const std::string& hint);
	static TriggerContent addItem(const std::string& itemID, int amount);
	static TriggerContent equipItem(const std::string& itemID);
	static TriggerContent removeItem(const std::string& itemID, int amount);
	static TriggerContent addGold(int amount);
	static TriggerContent removeGold(int amount);
	static TriggerContent learnSpell(int spellID);
	static TriggerContent setGuild(const std::string& guild);
	static TriggerContent startLevel(const std::string& levelID, int x, int y);
	static TriggerContent startMap(const std::string& mapID, int x, int y);
	static TriggerContent setLevel(const std::string& levelID, int x, int y);
	static TriggerContent setMap(const std::string& mapID, int x, int y);
	static TriggerContent setForcedMap(const std::string& mapID, int x, int y);
	static TriggerContent startCutscene(const std::string& cutsceneID);
	static TriggerContent unlockAchievement(const std::string& achievement);
};
