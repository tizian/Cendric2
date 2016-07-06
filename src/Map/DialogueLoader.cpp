#include "Map/DialogueLoader.h"
#include "CharacterCore.h"

using namespace std;
using namespace luabridge;

DialogueLoader::DialogueLoader(Dialogue& dialogue, CharacterCore* core) : m_dialogue(dialogue) {
	m_core = core;
}

DialogueLoader::~DialogueLoader() {
	delete m_currentNode;
}

void DialogueLoader::loadDialogue() {
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	getGlobalNamespace(L)
		.beginClass<DialogueLoader>("Dialogue")
		.addFunction("isQuestState", &DialogueLoader::isQuestState)
		.addFunction("isQuestComplete", &DialogueLoader::isQuestComplete)
		.addFunction("isConditionFulfilled", &DialogueLoader::isConditionFulfilled)
		.addFunction("isQuestConditionFulfilled", &DialogueLoader::isQuestConditionFulfilled)
		.addFunction("hasItem", &DialogueLoader::hasItem)
		.addFunction("getReputation", &DialogueLoader::getReputation)
		.addFunction("createCendricNode", &DialogueLoader::createCendricNode)
		.addFunction("createNPCNode", &DialogueLoader::createNPCNode)
		.addFunction("createChoiceNode", &DialogueLoader::createChoiceNode)
		.addFunction("createTradeNode", &DialogueLoader::createTradeNode)
		.addFunction("addChoice", &DialogueLoader::addChoice)
		.addFunction("changeQuestState", &DialogueLoader::changeQuestState)
		.addFunction("addQuestProgress", &DialogueLoader::addQuestProgress)
		.addFunction("addQuestDescription", &DialogueLoader::addQuestDescription)
		.addFunction("addConditionProgress", &DialogueLoader::addConditionProgress)
		.addFunction("addReputationProgress", &DialogueLoader::addReputationProgress)
		.addFunction("addHint", &DialogueLoader::addHint)
		.addFunction("addItem", &DialogueLoader::addItem)
		.addFunction("equipItem", &DialogueLoader::equipItem)
		.addFunction("removeItem", &DialogueLoader::removeItem)
		.addFunction("addGold", &DialogueLoader::addGold)
		.addFunction("removeGold", &DialogueLoader::removeGold)
		.addFunction("startLevel", &DialogueLoader::startLevel)
		.addFunction("startMap", &DialogueLoader::startMap)
		.addFunction("startCutscene", &DialogueLoader::startCutscene)
		.addFunction("learnSpell", &DialogueLoader::learnSpell)
		.addFunction("gotoNode", &DialogueLoader::gotoNode)
		.addFunction("setRoot", &DialogueLoader::setRoot)
		.addFunction("addNode", &DialogueLoader::addNode)
		.endClass();

	if (luaL_dofile(L, getResourcePath(m_dialogue.getID()).c_str()) != 0) {
		g_logger->logError("DialogeLoader", "Cannot read lua script: " + getResourcePath(m_dialogue.getID()));
		return;
	}

	LuaRef function = getGlobal(L, "loadDialogue");
	if (!function.isFunction()) {
		g_logger->logError("DialogeLoader", "Lua script: " + getResourcePath(m_dialogue.getID()) + " has no loadDialogue function.");
		return;
	}

	try {
		function(this);
	}
	catch (LuaException const& e) {
		g_logger->logError("DialogeLoader", "LuaException: " + std::string(e.what()));
	}
}

void DialogueLoader::addChoice(int nextTag, const std::string& text) {
	if (m_currentNode == nullptr || m_currentNode->type != DialogueNodeType::Choice) {
		g_logger->logError("DialogueLoader", "Cannot add choice: No choice node created.");
		return;
	}
	m_currentNode->choices.push_back(std::pair<std::string, int>(text, nextTag));
}

void DialogueLoader::changeQuestState(const std::string& questID, const std::string& state) {
	if (m_currentNode == nullptr) {
		g_logger->logError("DialogueLoader", "Cannot change state: no node created.");
		return;
	}
	if (questID.empty()) {
		g_logger->logError("DialogueLoader", "Quest ID cannot be empty.");
		return;
	}
	QuestState questState = resolveQuestState(state);
	if (questState == QuestState::VOID || questState == QuestState::MAX) {
		g_logger->logError("DialogueLoader", "Cannot change state: Quest state [" + state + "] does not exist.");
		return;
	}
	TriggerContent content(TriggerContentType::QuestStateChange);
	content.s1 = questID;
	content.i1 = static_cast<int>(questState);
	m_currentNode->content.push_back(content);
}

void DialogueLoader::addQuestDescription(const std::string& questID, int descriptionID) {
	if (m_currentNode == nullptr) {
		g_logger->logError("DialogueLoader", "Cannot add quest description: no node created.");
		return;
	}
	if (questID.empty()) {
		g_logger->logError("DialogueLoader", "Quest ID cannot be empty.");
		return;
	}
	if (descriptionID < 1) {
		g_logger->logError("DialogueLoader", "Description ID must be > 0");
		return;
	}
	TriggerContent content(TriggerContentType::QuestDescriptionProgress);
	content.s1 = questID;
	content.i1 = descriptionID;
	m_currentNode->content.push_back(content);
}

void DialogueLoader::addQuestProgress(const std::string& questID, const std::string& progress) {
	if (m_currentNode == nullptr) {
		g_logger->logError("DialogueLoader", "Cannot add quest progress: no node created.");
		return;
	}
	if (questID.empty() || progress.empty()) {
		g_logger->logError("DialogueLoader", "Quest ID and quest progress cannot be empty.");
		return;
	}
	TriggerContent content(TriggerContentType::QuestConditionProgress);
	content.s1 = questID;
	content.s2 = progress;
	m_currentNode->content.push_back(content);
}

void DialogueLoader::addConditionProgress(const std::string& conditionType, const std::string& condition) {
	if (m_currentNode == nullptr) {
		g_logger->logError("DialogueLoader", "Cannot add condition progress: no node created.");
		return;
	}
	if (condition.empty() || conditionType.empty()) {
		g_logger->logError("DialogueLoader", "Condition and condition type cannot be empty.");
		return;
	}
	TriggerContent content(TriggerContentType::ConditionProgress);
	content.s1 = conditionType;
	content.s2 = condition;
	m_currentNode->content.push_back(content);
}

void DialogueLoader::addReputationProgress(const std::string& fractionID, int amount) {
	if (m_currentNode == nullptr) {
		g_logger->logError("DialogueLoader", "Cannot add reputation: no node created.");
		return;
	}
	FractionID fraction = resolveFractionID(fractionID);
	if (fraction == FractionID::VOID || amount < 0) {
		g_logger->logError("DialogueLoader", "Fraction id not recognized or reputation amount < 0");
		return;
	}

	TriggerContent content(TriggerContentType::ReputationProgress);
	content.i1 = static_cast<int>(fraction);
	content.i2 = amount;
	m_currentNode->content.push_back(content);
}

void DialogueLoader::addHint(const std::string& hint) {
	if (m_currentNode == nullptr) {
		g_logger->logError("DialogueLoader", "Cannot add hint: no node created.");
		return;
	}
	if (hint.empty()) {
		g_logger->logError("DialogueLoader", "Hint cannot be empty.");
		return;
	}
	TriggerContent content(TriggerContentType::Hint);
	content.s1 = hint;
	m_currentNode->content.push_back(content);
}

bool DialogueLoader::isQuestComplete(const std::string& questID) {
	if (questID.empty()) {
		g_logger->logError("DialogueLoader", "Quest ID cannot be empty.");
		return false;
	}
	return m_core->isQuestComplete(questID);
}

bool DialogueLoader::isConditionFulfilled(const std::string& conditionType, const std::string& condition) const {
	if (condition.empty() || conditionType.empty()) {
		g_logger->logError("DialogueLoader", "Condition and condition type cannot be empty.");
		return false;
	}
	return m_core->isConditionFulfilled(conditionType, condition);
}

bool DialogueLoader::isQuestConditionFulfilled(const std::string& quest, const std::string& condition) const {
	if (quest.empty() || condition.empty()) {
		g_logger->logError("DialogueLoader", "Quest ID and condition cannot be empty.");
		return false;
	}
	return m_core->isQuestConditionFulfilled(quest, condition);
}

int DialogueLoader::getReputation(const std::string& fractionID) const {
	FractionID frac = resolveFractionID(fractionID);
	if (frac == FractionID::VOID) {
		g_logger->logError("DialogueLoader", "Reputation could not be queried, fraction id not recognized");
		return false;
	}
	return m_core->getReputation(frac);
}

bool DialogueLoader::hasItem(const std::string& item, int amount) const {
	if (item.empty() || amount < 1) {
		g_logger->logError("DialogueLoader", "Item key cannot be empty and amount has to be > 0");
		return false;
	}

	return m_core->hasItem(item, amount);
}

void DialogueLoader::addItem(const std::string& itemID, int amount) {
	if (m_currentNode == nullptr) {
		g_logger->logError("DialogueLoader", "Cannot add item change: no node created.");
		return;
	}
	if (itemID.empty()) {
		g_logger->logError("DialogueLoader", "Item ID cannot be empty.");
		return;
	}
	if (amount <= 0) {
		g_logger->logError("DialogueLoader", "amount cannot be negative.");
		return;
	}
	TriggerContent content(TriggerContentType::ItemChange);
	content.s1 = itemID;
	content.i1 = amount;
	m_currentNode->content.push_back(content);
}

void DialogueLoader::equipItem(const std::string& itemID) {
	if (m_currentNode == nullptr) {
		g_logger->logError("DialogueLoader", "Cannot add equipment item: no node created.");
		return;
	}
	if (itemID.empty()) {
		g_logger->logError("DialogueLoader", "Item ID cannot be empty.");
		return;
	}
	TriggerContent content(TriggerContentType::ItemEquip);
	content.s1 = itemID;
	m_currentNode->content.push_back(content);
}


void DialogueLoader::removeItem(const std::string& itemID, int amount) {
	if (m_currentNode == nullptr) {
		g_logger->logError("DialogueLoader", "Cannot add item change: no node created.");
		return;
	}
	if (itemID.empty()) {
		g_logger->logError("DialogueLoader", "Item ID cannot be empty.");
		return;
	}
	if (amount <= 0) {
		g_logger->logError("DialogueLoader", "amount cannot be negative.");
		return;
	}
	TriggerContent content(TriggerContentType::ItemChange);
	content.s1 = itemID;
	content.i1 = -amount;
	m_currentNode->content.push_back(content);
}

void DialogueLoader::addGold(int amount) {
	if (m_currentNode == nullptr) {
		g_logger->logError("DialogueLoader", "Cannot add gold change: no node created.");
		return;
	}
	if (amount <= 0) {
		g_logger->logError("DialogueLoader", "amount cannot be negative.");
		return;
	}
	TriggerContent content(TriggerContentType::GoldChange);
	content.i1 = amount;
	m_currentNode->content.push_back(content);
}

void DialogueLoader::removeGold(int amount) {
	if (m_currentNode == nullptr) {
		g_logger->logError("DialogueLoader", "Cannot add gold change: no node created.");
		return;
	}
	if (amount <= 0) {
		g_logger->logError("DialogueLoader", "amount cannot be negative.");
		return;
	}
	TriggerContent content(TriggerContentType::GoldChange);
	content.i1 = -amount;
	m_currentNode->content.push_back(content);
}

void DialogueLoader::learnSpell(int id) {
	if (m_currentNode == nullptr) {
		g_logger->logError("DialogueLoader", "Cannot learn spell: no node created.");
		return;
	}
	SpellID spellID = static_cast<SpellID>(id);
	if (spellID <= SpellID::VOID || spellID >= SpellID::MAX) {
		g_logger->logError("DialogueLoader", "Cannot learn spell, spell ID not recognized.");
		return;
	}
	TriggerContent content(TriggerContentType::LearnSpell);
	content.i1 = id;
	m_currentNode->content.push_back(content);
}

void DialogueLoader::startLevel(const std::string& levelID, int x, int y) {
	if (m_currentNode == nullptr) {
		g_logger->logError("DialogueLoader", "Cannot start level: no node created.");
		return;
	}
	if (levelID.empty() || x <= 0 || y <= 0) {
		g_logger->logError("DialogueLoader", "Level ID cannot be empty and the spawn position (x and y) must be > 0");
		return;
	}
	TriggerContent content(TriggerContentType::LevelEntry);
	content.s1 = levelID;
	content.i1 = x;
	content.i2 = y;
	m_currentNode->content.push_back(content);
}

void DialogueLoader::startMap(const std::string& mapID, int x, int y) {
	if (m_currentNode == nullptr) {
		g_logger->logError("DialogueLoader", "Cannot start map: no node created.");
		return;
	}
	if (mapID.empty() || x <= 0 || y <= 0) {
		g_logger->logError("DialogueLoader", "Map ID cannot be empty and the spawn position (x and y) must be > 0");
		return;
	}
	TriggerContent content(TriggerContentType::MapEntry);
	content.s1 = mapID;
	content.i1 = x;
	content.i2 = y;
	m_currentNode->content.push_back(content);
}

void DialogueLoader::startCutscene(const std::string& cutsceneID) {
	if (m_currentNode == nullptr) {
		g_logger->logError("DialogueLoader", "Cannot start cutscene: no node created.");
		return;
	}
	if (cutsceneID.empty()) {
		g_logger->logError("DialogueLoader", "Cutscene ID cannot be empty");
		return;
	}
	TriggerContent content(TriggerContentType::Cutscene);
	content.s1 = cutsceneID;
	m_currentNode->content.push_back(content);
}

bool DialogueLoader::isQuestState(const std::string& questID, const std::string& state) const {
	QuestState questState = resolveQuestState(state);
	if (questState == QuestState::MAX) {
		g_logger->logError("DialogueLoader", "Quest State: [" + state + "] does not exist");
		return false;
	}
	if (questID.empty()) {
		g_logger->logError("DialogueLoader", "Quest ID cannot be empty.");
		return false;
	}
	return m_core->getQuestState(questID) == questState;
}

void DialogueLoader::createCendricNode(int tag, int nextTag, const std::string& text) {
	if (m_currentNode != nullptr) {
		g_logger->logError("DialogueLoader", "Cannot add a new node to the dialogue if the old node is not yet added. Call \"addNode()\" to add that old node first.");
		return;
	}
	m_currentNode = new DialogueNode();
	m_currentNode->type = DialogueNodeType::CendricTalking;
	m_currentNode->tag = tag;
	m_currentNode->nextTag = nextTag;
	m_currentNode->text = text;
}

void DialogueLoader::createNPCNode(int tag, int nextTag, const std::string& text) {
	if (m_currentNode != nullptr) {
		g_logger->logError("DialogueLoader", "Cannot add a new node to the dialogue if the old node is not yet added. Call \"addNode()\" to add that old node first.");
		return;
	}
	m_currentNode = new DialogueNode();
	m_currentNode->type = DialogueNodeType::NPCTalking;
	m_currentNode->tag = tag;
	m_currentNode->nextTag = nextTag;
	m_currentNode->text = text;
}

void DialogueLoader::createTradeNode(int tag, int nextTag) {
	if (m_currentNode != nullptr) {
		g_logger->logError("DialogueLoader", "Cannot add a new node to the dialogue if the old node is not yet added. Call \"addNode()\" to add that old node first.");
		return;
	}
	m_currentNode = new DialogueNode();
	m_currentNode->type = DialogueNodeType::Trade;
	m_currentNode->tag = tag;
	m_currentNode->nextTag = nextTag;
}

void DialogueLoader::createChoiceNode(int tag) {
	if (m_currentNode != nullptr) {
		g_logger->logError("DialogueLoader", "Cannot add a new node to the dialogue if the old node is not yet added. Call \"addNode()\" to add that old node first.");
		return;
	}
	m_currentNode = new DialogueNode();
	m_currentNode->type = DialogueNodeType::Choice;
	m_currentNode->tag = tag;
}

void DialogueLoader::gotoNode(int tag) {
	if (m_currentNode == nullptr) {
		g_logger->logError("DialogueLoader", "Cannot add goto, no node created.");
		return;
	}
	if (tag < 0) {
		g_logger->logError("DialogueLoader", "Cannot goto, node tag has to be >= 0");
		return;
	}
	m_currentNode->reloadTag = tag;
}

void DialogueLoader::addNode() {
	if (m_currentNode == nullptr) {
		g_logger->logError("DialogueLoader", "Cannot add current node, no node created.");
		return;
	}
	if (m_currentNode->type == DialogueNodeType::Choice && m_currentNode->choices.empty()) {
		g_logger->logError("DialogueLoader", "Cannot add current node, a choice node cannot exist without choices.");
		return;
	}
	if (m_currentNode->type != DialogueNodeType::Choice && m_currentNode->nextTag == m_currentNode->tag) {
		g_logger->logWarning("DialogueLoader", "Loop detected in dialogue. Tag cannot equal next tag.");
	}
	if (m_currentNode->tag < 0) {
		g_logger->logWarning("DialogueLoader", "Tag of node should not be negative. -1 is reserved to end a dialogue.");
	}
	m_dialogue.addNode(m_currentNode->tag, *m_currentNode);
	delete m_currentNode;
	m_currentNode = nullptr;
}

void DialogueLoader::setRoot(int tag) {
	m_dialogue.setRoot(tag);
}