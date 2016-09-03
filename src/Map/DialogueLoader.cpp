#include "Map/DialogueLoader.h"
#include "CharacterCore.h"
#include "Screens/WorldScreen.h"

using namespace luabridge;

DialogueLoader::DialogueLoader(Dialogue& dialogue, WorldScreen* screen) : m_dialogue(dialogue) {
	m_worldCallback = new WorldCallback(screen);
}

DialogueLoader::~DialogueLoader() {
	delete m_currentNode;
	delete m_worldCallback;
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
		.addFunction("getItemAmount", &DialogueLoader::getItemAmount)
		.addFunction("getReputation", &DialogueLoader::getReputation)
		.addFunction("createCendricNode", &DialogueLoader::createCendricNode)
		.addFunction("createNPCNode", &DialogueLoader::createNPCNode)
		.addFunction("createChoiceNode", &DialogueLoader::createChoiceNode)
		.addFunction("createTradeNode", &DialogueLoader::createTradeNode)
		.addFunction("addChoice", &DialogueLoader::addChoice)
		.addFunction("addItemChoice", &DialogueLoader::addItemChoice)
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
	
	m_dialogue.setRoot(m_root);
}

void DialogueLoader::addChoice(int nextTag, const std::string& text) {
	if (m_currentNode == nullptr || m_currentNode->type != DialogueNodeType::Choice) {
		g_logger->logError("DialogueLoader", "Cannot add choice: No choice node created.");
		return;
	}
	ChoiceTranslation trans;
	trans.text = text;
	m_currentNode->choices.push_back(std::pair<ChoiceTranslation, int>(trans, nextTag));
}

void DialogueLoader::addItemChoice(int nextTag, const std::string& text, const std::string& itemID, int amount) {
	if (m_currentNode == nullptr || m_currentNode->type != DialogueNodeType::Choice) {
		g_logger->logError("DialogueLoader", "Cannot add choice: No choice node created.");
		return;
	}
	if (itemID.empty() || amount <= 0) {
		g_logger->logError("DialogueLoader", "Cannot add item choice: Item ID empty or amount <= 0.");
		return;
	}
	ChoiceTranslation trans;
	trans.text = text;
	trans.item.first = itemID;
	trans.item.second = amount;
	m_currentNode->choices.push_back(std::pair<ChoiceTranslation, int>(trans, nextTag));
}

void DialogueLoader::changeQuestState(const std::string& questID, const std::string& state) {
	if (m_currentNode == nullptr) {
		g_logger->logError("DialogueLoader", "Cannot change state: no node created.");
		return;
	}
	TriggerContent content = TriggerContent::changeQuestState(questID, state);
	if (content.isValid()) m_currentNode->content.push_back(TriggerContent::changeQuestState(questID, state));
}

void DialogueLoader::addQuestDescription(const std::string& questID, int descriptionID) {
	if (m_currentNode == nullptr) {
		g_logger->logError("DialogueLoader", "Cannot add quest description: no node created.");
		return;
	}
	TriggerContent content = TriggerContent::addQuestDescription(questID, descriptionID);
	if (content.isValid()) m_currentNode->content.push_back(content);
}

void DialogueLoader::addQuestProgress(const std::string& questID, const std::string& progress) {
	if (m_currentNode == nullptr) {
		g_logger->logError("DialogueLoader", "Cannot add quest progress: no node created.");
		return;
	}
	TriggerContent content = TriggerContent::addQuestProgress(questID, progress);
	if (content.isValid()) m_currentNode->content.push_back(content);
}

void DialogueLoader::addConditionProgress(const std::string& conditionType, const std::string& condition) {
	if (m_currentNode == nullptr) {
		g_logger->logError("DialogueLoader", "Cannot add condition progress: no node created.");
		return;
	}
	TriggerContent content = TriggerContent::addConditionProgress(conditionType, condition);
	if (content.isValid()) m_currentNode->content.push_back(content);
}

void DialogueLoader::addReputationProgress(const std::string& fractionID, int amount) {
	if (m_currentNode == nullptr) {
		g_logger->logError("DialogueLoader", "Cannot add reputation: no node created.");
		return;
	}
	TriggerContent content = TriggerContent::addReputationProgress(fractionID, amount);
	if (content.isValid()) m_currentNode->content.push_back(content);
}

void DialogueLoader::addHint(const std::string& hint) {
	if (m_currentNode == nullptr) {
		g_logger->logError("DialogueLoader", "Cannot add hint: no node created.");
		return;
	}
	TriggerContent content = TriggerContent::addHint(hint);
	if (content.isValid()) m_currentNode->content.push_back(content);
}

void DialogueLoader::addItem(const std::string& itemID, int amount) {
	if (m_currentNode == nullptr) {
		g_logger->logError("DialogueLoader", "Cannot add item change: no node created.");
		return;
	}
	TriggerContent content = TriggerContent::addItem(itemID, amount);
	if (content.isValid()) m_currentNode->content.push_back(content);
}

void DialogueLoader::equipItem(const std::string& itemID) {
	if (m_currentNode == nullptr) {
		g_logger->logError("DialogueLoader", "Cannot add equipment item: no node created.");
		return;
	}
	TriggerContent content = TriggerContent::equipItem(itemID);
	if (content.isValid()) m_currentNode->content.push_back(content);
}


void DialogueLoader::removeItem(const std::string& itemID, int amount) {
	if (m_currentNode == nullptr) {
		g_logger->logError("DialogueLoader", "Cannot add item change: no node created.");
		return;
	}
	TriggerContent content = TriggerContent::removeItem(itemID, amount);
	if (content.isValid()) m_currentNode->content.push_back(content);
}

void DialogueLoader::addGold(int amount) {
	if (m_currentNode == nullptr) {
		g_logger->logError("DialogueLoader", "Cannot add gold change: no node created.");
		return;
	}
	TriggerContent content = TriggerContent::addGold(amount);
	if (content.isValid()) m_currentNode->content.push_back(content);
}

void DialogueLoader::removeGold(int amount) {
	if (m_currentNode == nullptr) {
		g_logger->logError("DialogueLoader", "Cannot add gold change: no node created.");
		return;
	}
	TriggerContent content = TriggerContent::removeGold(amount);
	if (content.isValid()) m_currentNode->content.push_back(content);
}

void DialogueLoader::learnSpell(int id) {
	if (m_currentNode == nullptr) {
		g_logger->logError("DialogueLoader", "Cannot learn spell: no node created.");
		return;
	}
	TriggerContent content = TriggerContent::learnSpell(id);
	if (content.isValid()) m_currentNode->content.push_back(content);
}

void DialogueLoader::startLevel(const std::string& levelID, int x, int y) {
	if (m_currentNode == nullptr) {
		g_logger->logError("DialogueLoader", "Cannot start level: no node created.");
		return;
	}
	TriggerContent content = TriggerContent::startLevel(levelID, x, y);
	if (content.isValid()) m_currentNode->content.push_back(content);
}

void DialogueLoader::startMap(const std::string& mapID, int x, int y) {
	if (m_currentNode == nullptr) {
		g_logger->logError("DialogueLoader", "Cannot start map: no node created.");
		return;
	}
	TriggerContent content = TriggerContent::startMap(mapID, x, y);
	if (content.isValid()) m_currentNode->content.push_back(content);
}

void DialogueLoader::startCutscene(const std::string& cutsceneID) {
	if (m_currentNode == nullptr) {
		g_logger->logError("DialogueLoader", "Cannot start cutscene: no node created.");
		return;
	}
	TriggerContent content = TriggerContent::startCutscene(cutsceneID);
	if (content.isValid()) m_currentNode->content.push_back(content);
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
		g_logger->logWarning("DialogueLoader", "Tag of node should not be negative. -1 and -2 are reserved to end a dialogue.");
	}
	m_dialogue.addNode(m_currentNode->tag, *m_currentNode);
	delete m_currentNode;
	m_currentNode = nullptr;
}

void DialogueLoader::setRoot(int tag) {
	m_root = tag;
}
