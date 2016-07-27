#include "Callbacks/WorldCallback.h"
#include "CharacterCore.h"
#include "Screens/WorldScreen.h"
#include "Screens/MapScreen.h"
#include "Screens/LevelScreen.h"
#include "Map/NPC.h"
#include "ObjectFactory.h"
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
		.addFunction("hasItem", &WorldCallback::hasItem)
		.addFunction("getReputation", &WorldCallback::getReputation)
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
		.addFunction("startCutscene", &WorldCallback::startCutscene)
		.addFunction("learnSpell", &WorldCallback::learnSpell)
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

int WorldCallback::getReputation(const std::string& fractionID) const {
	FractionID frac = resolveFractionID(fractionID);
	if (frac == FractionID::VOID) {
		g_logger->logError("WorldCallback", "Reputation could not be queried, fraction id not recognized");
		return false;
	}
	return m_core->getReputation(frac);
}

bool WorldCallback::hasItem(const std::string& item, int amount) const {
	if (item.empty() || amount < 1) {
		g_logger->logError("WorldCallback", "Item key cannot be empty and amount has to be > 0");
		return false;
	}

	return m_core->hasItem(item, amount);
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

void WorldCallback::startLevel(const std::string& levelID, int x, int y) const {
	TriggerContent::executeTrigger(TriggerContent::startLevel(levelID, x, y), m_screen);
}

void WorldCallback::startMap(const std::string& mapID, int x, int y) const {
	TriggerContent::executeTrigger(TriggerContent::startMap(mapID, x, y), m_screen);
}

void WorldCallback::startCutscene(const std::string& cutsceneID) const {
	TriggerContent::executeTrigger(TriggerContent::startCutscene(cutsceneID), m_screen);
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

void WorldCallback::spawnEnemy(int enemyID, int x, int y) const {
	LevelScreen* levelScreen = dynamic_cast<LevelScreen*>(m_screen);
	if (levelScreen == nullptr) {
		g_logger->logError("WorldCallback", "A enemy can only be spawned in a level.");
		return;
	}

	EnemyID id = static_cast<EnemyID>(enemyID);
	if (id <= EnemyID::VOID || id >= EnemyID::MAX) {
		g_logger->logError("WorldCallback", "Enemy id: " + std::to_string(enemyID) + " is not valid.");
		return;
	}

	Enemy* enemy = nullptr;
	enemy = ObjectFactory::Instance()->createEnemy(id, levelScreen->getWorld(), levelScreen);
	if (enemy == nullptr) {
		g_logger->logError("WorldCallback", "Enemy was could not be spawned, unknown id.");
		return;
	}

	// calculate loot.
	std::map<std::string, int> loot;
	int gold = 0;

	enemy->insertRespawnLoot(loot, gold);
	enemy->setLoot(loot, gold);

	enemy->setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
	enemy->setObjectID(-1);
	enemy->setUnique(false);
	enemy->setBoss(false);
	enemy->setDebugBoundingBox(sf::Color::Magenta);
	
	levelScreen->addObject(enemy);
}



