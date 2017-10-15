#include "Callbacks/WorldCallback.h"
#include "CharacterCore.h"
#include "Screens/WorldScreen.h"
#include "Screens/MapScreen.h"
#include "Screens/LevelScreen.h"
#include "Map/NPC.h"
#include "GlobalResource.h"

using namespace luabridge;

WorldCallback::WorldCallback(WorldScreen* screen) {
	m_core = screen->getCharacterCore();
	m_screen = screen;
}

WorldCallback::~WorldCallback() {
}

void WorldCallback::bindFunctions(lua_State* luaState) const {
	getGlobalNamespace(luaState)
		.beginClass<WorldCallback>("World")
		// queries
		.addFunction("isQuestState", &WorldCallback::isQuestState)
		.addFunction("isQuestComplete", &WorldCallback::isQuestComplete)
		.addFunction("isConditionFulfilled", &WorldCallback::isConditionFulfilled)
		.addFunction("isQuestConditionFulfilled", &WorldCallback::isQuestConditionFulfilled)
		.addFunction("isQuestDescriptionUnlocked", &WorldCallback::isQuestDescriptionUnlocked)
		.addFunction("hasItem", &WorldCallback::hasItem)
		.addFunction("isItemEquipped", &WorldCallback::isItemEquipped)
		.addFunction("isSpellLearned", &WorldCallback::isSpellLearned)
		.addFunction("getItemAmount", &WorldCallback::getItemAmount)
		.addFunction("getReputation", &WorldCallback::getReputation)
		.addFunction("getGuild", &WorldCallback::getGuild)
		// commands
		.addFunction("changeQuestState", &WorldCallback::changeQuestState)
		.addFunction("addQuestProgress", &WorldCallback::addQuestProgress)
		.addFunction("addQuestDescription", &WorldCallback::addQuestDescription)
		.addFunction("addConditionProgress", &WorldCallback::addConditionProgress)
		.addFunction("addReputationProgress", &WorldCallback::addReputationProgress)
		.addFunction("addHint", &WorldCallback::addHint)
		.addFunction("addItem", &WorldCallback::addItem)
		.addFunction("equipItem", &WorldCallback::equipItem)
		.addFunction("removeItem", &WorldCallback::removeItem)
		.addFunction("addGold", &WorldCallback::addGold)
		.addFunction("removeGold", &WorldCallback::removeGold)
		.addFunction("startLevel", &WorldCallback::startLevel)
		.addFunction("startMap", &WorldCallback::startMap)
		.addFunction("setLevel", &WorldCallback::setLevel)
		.addFunction("setMap", &WorldCallback::setMap)
		.addFunction("startCutscene", &WorldCallback::startCutscene)
		.addFunction("setGuild", &WorldCallback::setGuild)
		.addFunction("learnSpell", &WorldCallback::learnSpell)
		.addFunction("unlockAchievement", &WorldCallback::unlockAchievement)
		// spawners
		.addFunction("spawnNPC", &WorldCallback::spawnNPC)
		.addFunction("spawnEnemy", &WorldCallback::spawnEnemy)
		.endClass();
}

bool WorldCallback::isQuestState(const std::string& questID, const std::string& state) const {
	QuestState questState = resolveQuestState(state);
	if (questState == QuestState::MAX) {
		g_logger->logError("WorldCallback", "Quest State: [" + state + "] does not exist");
		return false;
	}
	if (questID.empty()) {
		g_logger->logError("WorldCallback", "Quest ID cannot be empty.");
		return false;
	}
	return m_core->getQuestState(questID) == questState;
}

bool WorldCallback::isQuestComplete(const std::string& questID) const {
	if (questID.empty()) {
		g_logger->logError("WorldCallback", "Quest ID cannot be empty.");
		return false;
	}
	return m_core->isQuestComplete(questID);
}

bool WorldCallback::isConditionFulfilled(const std::string& conditionType, const std::string& condition) const {
	if (condition.empty() || conditionType.empty()) {
		g_logger->logError("WorldCallback", "Condition and condition type cannot be empty.");
		return false;
	}
	return m_core->isConditionFulfilled(conditionType, condition);
}

bool WorldCallback::isQuestConditionFulfilled(const std::string& quest, const std::string& condition) const {
	if (quest.empty() || condition.empty()) {
		g_logger->logError("WorldCallback", "Quest ID and condition cannot be empty.");
		return false;
	}
	return m_core->isQuestConditionFulfilled(quest, condition);
}

bool WorldCallback::isQuestDescriptionUnlocked(const std::string& quest, int description) const {
	if (quest.empty()) {
		g_logger->logError("WorldCallback", "Quest ID cannot be empty.");
		return false;
	}
	return m_core->isQuestDescriptionUnlocked(quest, description);
}

int WorldCallback::getReputation(const std::string& fractionID) const {
	FractionID frac = resolveFractionID(fractionID);
	if (frac == FractionID::VOID) {
		g_logger->logError("WorldCallback", "Reputation could not be queried, fraction id not recognized");
		return false;
	}
	return m_core->getReputation(frac);
}

std::string WorldCallback::getGuild() const {
	if (m_core->getData().guild == FractionID::VOID) {
		return "void";
	}
	return EnumNames::getFractionIDName(m_core->getData().guild);
}

bool WorldCallback::hasItem(const std::string& itemID, int amount) const {
	if (itemID.empty() || amount < 1) {
		g_logger->logError("WorldCallback", "Item key cannot be empty and amount has to be > 0");
		return false;
	}

	return m_core->hasItem(itemID, amount);
}

bool WorldCallback::isItemEquipped(const std::string& itemID) const {
	if (itemID.empty()) {
		g_logger->logError("WorldCallback", "Item key cannot be empty ");
		return false;
	}

	return m_core->isItemEquipped(itemID);
}

bool WorldCallback::isSpellLearned(int spellID) {
	return m_core->isSpellLearned(static_cast<SpellID>(spellID));
}

bool WorldCallback::isSpellEquipped(int spellID) {
	return m_core->isSpellEquipped(static_cast<SpellID>(spellID));
}

int WorldCallback::getItemAmount(const std::string& itemID) const {
	return m_core->getItemAmount(itemID);
}

void WorldCallback::changeQuestState(const std::string& questID, const std::string& state) const {
	TriggerContent::executeTrigger(TriggerContent::changeQuestState(questID, state), m_screen);
}

void WorldCallback::addQuestProgress(const std::string& questID, const std::string& progress) const {
	TriggerContent::executeTrigger(TriggerContent::addQuestProgress(questID, progress), m_screen);
}

void WorldCallback::addQuestDescription(const std::string& questID, int descriptionID) const {
	TriggerContent::executeTrigger(TriggerContent::addQuestDescription(questID, descriptionID), m_screen);
}

void WorldCallback::addConditionProgress(const std::string& conditionType, const std::string& condition) const {
	TriggerContent::executeTrigger(TriggerContent::addConditionProgress(conditionType, condition), m_screen);
}

void WorldCallback::addReputationProgress(const std::string& fractionID, int amount) const {
	TriggerContent::executeTrigger(TriggerContent::addReputationProgress(fractionID, amount), m_screen);
}

void WorldCallback::addHint(const std::string& hint) const {
	TriggerContent::executeTrigger(TriggerContent::addHint(hint), m_screen);
}

void WorldCallback::addItem(const std::string& itemID, int amount) const {
	TriggerContent::executeTrigger(TriggerContent::addItem(itemID, amount), m_screen);
}

void WorldCallback::equipItem(const std::string& itemID) const {
	TriggerContent::executeTrigger(TriggerContent::equipItem(itemID), m_screen);
}

void WorldCallback::removeItem(const std::string& itemID, int amount) const {
	TriggerContent::executeTrigger(TriggerContent::removeItem(itemID, amount), m_screen);
}

void WorldCallback::addGold(int amount) const {
	TriggerContent::executeTrigger(TriggerContent::addGold(amount), m_screen);
}

void WorldCallback::removeGold(int amount) const {
	TriggerContent::executeTrigger(TriggerContent::removeGold(amount), m_screen);
}

void WorldCallback::learnSpell(int spellID) const {
	TriggerContent::executeTrigger(TriggerContent::learnSpell(spellID), m_screen);
}

void WorldCallback::setGuild(const std::string& guild) const {
	TriggerContent::executeTrigger(TriggerContent::setGuild(guild), m_screen);
}

void WorldCallback::startLevel(const std::string& levelID, int x, int y) const {
	TriggerContent::executeTrigger(TriggerContent::startLevel(levelID, x, y), m_screen);
}

void WorldCallback::startMap(const std::string& mapID, int x, int y) const {
	TriggerContent::executeTrigger(TriggerContent::startMap(mapID, x, y), m_screen);
}

void WorldCallback::setLevel(const std::string& levelID, int x, int y) const {
	TriggerContent::executeTrigger(TriggerContent::setLevel(levelID, x, y), m_screen);
}

void WorldCallback::setMap(const std::string& mapID, int x, int y) const {
	TriggerContent::executeTrigger(TriggerContent::setMap(mapID, x, y), m_screen);
}

void WorldCallback::startCutscene(const std::string& cutsceneID) const {
	TriggerContent::executeTrigger(TriggerContent::startCutscene(cutsceneID), m_screen);
}

void WorldCallback::unlockAchievement(const std::string& achievement) const {
	TriggerContent::executeTrigger(TriggerContent::unlockAchievement(achievement), m_screen);
}

void WorldCallback::spawnNPC(const std::string& npcID, int x, int y) const {
	MapScreen* mapScreen = dynamic_cast<MapScreen*>(m_screen);
	if (mapScreen == nullptr) {
		g_logger->logError("WorldCallback", "A npc can only be spawned on a map.");
		return;
	}

	NPCData data = DEFAULT_NPC;
	data.id = npcID;
	data.calculateDefaultFromID();
	data.position.x = static_cast<float>(x);
	data.position.y = static_cast<float>(y);

	NPC* mapNPC = new NPC(mapScreen);
	mapNPC->setScreen(mapScreen);
	mapNPC->load(data);
	mapScreen->addObject(mapNPC);
}

void WorldCallback::spawnEnemy(lua_State* state) const {
	LevelScreen* levelScreen = dynamic_cast<LevelScreen*>(m_screen);
	if (levelScreen == nullptr) {
		g_logger->logError("WorldCallback", "A enemy can only be spawned in a level.");
		return;
	}

	luabridge::LuaRef param = luabridge::LuaRef::fromStack(state, 2);
	if (!param.isTable()) {
		g_logger->logError("WorldCallback", "spawnEnemy() has no table argument in luascript!");
		return;
	}

	// resolve and set enemy ID
	LuaRef idRef = param["id"];
	if (!idRef.isNumber()) {
		g_logger->logError("WorldCallback", "spawnEnemy() has no id!");
		return;
	}

	EnemyID id = static_cast<EnemyID>(idRef.cast<int>());
	if (id <= EnemyID::VOID || id >= EnemyID::MAX) {
		g_logger->logError("WorldCallback", "Enemy id: " + std::to_string(static_cast<int>(id)) + " is not valid.");
		return;
	}

	// calculate loot.
	std::map<std::string, int> loot;
	int gold = 0;

	LuaRef lootRef = param["loot"];
	if (lootRef.isTable()) {
		int i = 1;
		LuaRef lootElement = lootRef[i];
		while (!lootElement.isNil()) {
			LuaRef itemIDRef = lootElement["id"];
			LuaRef amountRef = lootElement["amount"];

			if (!itemIDRef.isString() || !amountRef.isNumber()) {
				g_logger->logError("WorldCallback", "Enemy was could not be spawned, loot item id or loot amount has wrong type.");
				return;
			}

			std::string itemID = itemIDRef.cast<std::string>();
			if (itemID.compare("gold") == 0) {
				gold = amountRef.cast<int>();
			}
			else {
				loot.insert({ itemIDRef.cast<std::string>(), amountRef.cast<int>() });
			}

			lootElement = lootRef[++i];
		}
	}

	// calculate position
	LuaRef positionRef = param["position"];
	if (!positionRef.isTable()) {
		g_logger->logError("WorldCallback", "spawnEnemy() has no position!");
		return;
	}

	LuaRef posXRef = positionRef["x"];
	LuaRef posYRef = positionRef["y"];

	if (!posXRef.isNumber() || !posYRef.isNumber()) {
		g_logger->logError("WorldCallback", "spawnEnemy() wrong position types for x and y!");
		return;
	}

	// resolve luapath
	LuaRef lpath = param["luapath"];

	Enemy* enemy = levelScreen->spawnEnemy(id, sf::Vector2f(posXRef.cast<float>(), posYRef.cast<float>()));
	if (enemy == nullptr) {
		g_logger->logError("WorldCallback", "Enemy could not be spawned, unknown id.");
		return;
	}
	
	if (lpath.isString()) {
		enemy->setScriptedBehavior(lpath.cast<std::string>());
	}
}
