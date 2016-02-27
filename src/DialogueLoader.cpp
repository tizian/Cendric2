#include "DialogueLoader.h"
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
		.addFunction("isNPCState", &DialogueLoader::isNPCState)
		.addFunction("isQuestState", &DialogueLoader::isQuestState)
		.addFunction("isQuestComplete", &DialogueLoader::isQuestComplete)
		.addFunction("isConditionFulfilled", &DialogueLoader::isConditionFulfilled)
		.addFunction("createCendricNode", &DialogueLoader::createCendricNode)
		.addFunction("createNPCNode", &DialogueLoader::createNPCNode)
		.addFunction("createChoiceNode", &DialogueLoader::createChoiceNode)
		.addFunction("createTradeNode", &DialogueLoader::createTradeNode)
		.addFunction("addChoice", &DialogueLoader::addChoice)
		.addFunction("changeNPCState", &DialogueLoader::changeNPCState)
		.addFunction("changeQuestState", &DialogueLoader::changeQuestState)
		.addFunction("addQuestProgress", &DialogueLoader::addQuestProgress)
		.addFunction("addConditionProgress", &DialogueLoader::addConditionProgress)
		.addFunction("addItem", &DialogueLoader::addItem)
		.addFunction("removeItem", &DialogueLoader::removeItem)
		.addFunction("addGold", &DialogueLoader::addGold)
		.addFunction("removeGold", &DialogueLoader::removeGold)
		.addFunction("setRoot", &DialogueLoader::setRoot)
		.addFunction("addNode", &DialogueLoader::addNode)
		.endClass();

	if (luaL_dofile(L, m_dialogue.getID().c_str()) != 0) {
		g_logger->logError("DialogeLoader", "Cannot read lua script: " + m_dialogue.getID());
		return;
	}

	LuaRef function = getGlobal(L, "loadDialogue");
	if (!function.isFunction()) {
		g_logger->logError("DialogeLoader", "Lua script: " + m_dialogue.getID() + " has no loadDialogue function.");
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

void DialogueLoader::changeNPCState(const std::string& npcID, const std::string& state) {
	if (m_currentNode == nullptr) {
		g_logger->logError("DialogueLoader", "Cannot change state: no node created.");
		return;
	}
	if (npcID.empty()) {
		g_logger->logError("DialogueLoader", "Npc ID cannot be empty.");
		return;
	}
	NPCState npcState = resolveNPCState(state);
	if (npcState == NPCState::VOID) {
		g_logger->logError("DialogueLoader", "Cannot change state: Npc state [" + state + "] does not exist.");
		return;
	}

	m_currentNode->npcStates.insert({ npcID, npcState });
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
	if (questState == QuestState::VOID) {
		g_logger->logError("DialogueLoader", "Cannot change state: Quest state [" + state + "] does not exist.");
		return;
	}

	m_currentNode->questStates.insert({ questID, questState });
}

void DialogueLoader::addQuestProgress(const std::string& questID, const std::string& progress) {
	if (m_currentNode == nullptr) {
		g_logger->logError("DialogueLoader", "Cannot add quest progress: no node created.");
		return;
	}
	if (questID.empty()) {
		g_logger->logError("DialogueLoader", "Quest ID cannot be empty.");
		return;
	}
	if (m_currentNode->questProgress.find(questID) != m_currentNode->questProgress.end()) {
		g_logger->logError("DialogueLoader", "Cannot add more than one quest progress per node and quest.");
		return;
	}
	m_currentNode->questProgress.insert({ questID, progress });
}

void DialogueLoader::addConditionProgress(const std::string& condition) {
	if (m_currentNode == nullptr) {
		g_logger->logError("DialogueLoader", "Cannot add condition progress: no node created.");
		return;
	}
	if (condition.empty()) {
		g_logger->logError("DialogueLoader", "Condition cannot be empty.");
		return;
	}
	m_currentNode->conditionProgress.insert(condition);
}

bool DialogueLoader::isNPCState(const std::string& npcID, const std::string& state) const {
	NPCState npcState = resolveNPCState(state);
	if (npcState == NPCState::VOID) {
		g_logger->logError("DialogueLoader", "Npc State: [" + state + "] does not exist");
		return false;
	}
	if (npcID.empty()) {
		g_logger->logError("DialogueLoader", "Npc ID cannot be empty.");
		return false;
	}
	return m_core->getNPCState(npcID) == npcState;
}

bool DialogueLoader::isQuestComplete(const std::string& questID) {
	if (questID.empty()) {
		g_logger->logError("DialogueLoader", "Quest ID cannot be empty.");
		return false;
	}
	return m_core->isQuestComplete(questID);
}

bool DialogueLoader::isConditionFulfilled(const std::string& condition) const {
	if (condition.empty()) {
		g_logger->logError("DialogueLoader", "Condition cannot be empty.");
		return false;
	}
	return m_core->isConditionFulfilled(condition);
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
	m_currentNode->itemChanges.insert({ itemID, amount });
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
	m_currentNode->itemChanges.insert({ itemID, -amount });
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
	m_currentNode->goldChanges = amount;
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
	m_currentNode->goldChanges = -amount;
}

bool DialogueLoader::isQuestState(const std::string& questID, const std::string& state) const {
	QuestState questState = resolveQuestState(state);
	if (questState == QuestState::VOID) {
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

void DialogueLoader::createTradeNode(int tag, int nextTag, const std::string& text) {
	if (m_currentNode != nullptr) {
		g_logger->logError("DialogueLoader", "Cannot add a new node to the dialogue if the old node is not yet added. Call \"addNode()\" to add that old node first.");
		return;
	}
	m_currentNode = new DialogueNode();
	m_currentNode->type = DialogueNodeType::Trade;
	m_currentNode->tag = tag;
	m_currentNode->nextTag = nextTag;
	m_currentNode->text = text;
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