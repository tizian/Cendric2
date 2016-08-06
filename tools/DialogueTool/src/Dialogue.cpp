#include "Dialogue.h"
#include "Nodes/StartNode.h"
#include "Nodes/ChoiceNode.h"
#include "Nodes/NPCNode.h"
#include "FileIO/ConfigurationIO.h"
#include "FileIO/DialogueIO.h"
#include "ApplicationState.h"

#include <sstream>
#include <iostream>
#include <fstream>

#include <sys/types.h>
#include <sys/stat.h>

Dialogue::Dialogue(const std::string& npcID) {
	m_npcID = npcID;
	m_dialogueName = npcID;
	m_startNode = new StartNode();
}

Dialogue::Dialogue(const std::string& npcID, const std::string& dialogueName, StartNode* startNode) {
	assert(startNode != nullptr);
	m_npcID = npcID;
	m_dialogueName = dialogueName;
	m_startNode = startNode;
	recursiveCalculateUsedTags(m_startNode);
}

Dialogue::~Dialogue() {
	delete m_startNode;
}

void Dialogue::setDialogueName(const std::string& dialogueName) {
	m_dialogueName = dialogueName;
}

StartNode* Dialogue::getStartNode() {
	return m_startNode;
}

const std::string& Dialogue::getNpcID() const {
	return m_npcID;
}

const std::string& Dialogue::getDialogueName() const {
	return m_dialogueName;
}

int Dialogue::generateTag() {
	int tag = 1;
	while (m_usedTags.find(tag) != m_usedTags.end()) {
		++tag;
	}
	m_usedTags.insert(tag);
	return tag;
}

void Dialogue::freeTag(int tag) {
	if (tag < 1) return;
	auto pos = m_usedTags.find(tag);
	if (pos == m_usedTags.end()) return;
	m_usedTags.erase(pos);
}

void Dialogue::recursiveCalculateUsedTags(DialogueNode* node) {
	if (node == nullptr) return;
	m_usedTags.insert(node->getTag());

	for (auto child : node->getLinkNodes()) {
		recursiveCalculateUsedTags(child->nextNode);
	}
}

bool Dialogue::exportToDia() {
	// first create a directory
	std::string folderPath = G_CONF.dialogueFolder;
	int nError = 0;
#if defined(_WIN32)
	nError = _mkdir(folderPath.c_str()); // windows style mkdir
#else 
	nError = mkdir(sPath.c_str(), 0733); // non-Windows style mkdir
#endif
	if (nError != 0) {
		struct stat info;
		if (stat(folderPath.c_str(), &info) != 0) {
			ERROR("[Dialogue]: Unable to create directory: " + folderPath);
			return false;
		}
	}

	std::string filePath = folderPath + "/" + m_dialogueName + ".dia";
	if (DialogueIO::saveDialogue(filePath, *this)) {
		LOG("[Dialogue]: Dialogue written successfully to file: " + filePath);
		return true;
	}
	return false;
}

bool Dialogue::exportDialogue() {
	// first create a directory
	std::string folderPath = G_CONF.npcFolder + "/" + m_npcID;
	int nError = 0;
#if defined(_WIN32)
	nError = _mkdir(folderPath.c_str()); // windows style mkdir
#else 
	nError = mkdir(sPath.c_str(), 0733); // non-Windows style mkdir
#endif
	if (nError != 0) {
		struct stat info;
		if (stat(folderPath.c_str(), &info) != 0) {
			ERROR("[Dialogue]: Unable to create directory: " + folderPath);
			return false;
		}
	}

	// then create the file  (lua export)
	std::string luaPath = folderPath + "/dl_" + m_npcID + ".lua";
	std::ofstream dialogueFile(luaPath, std::ios::trunc);
	if (dialogueFile.is_open()) {
		dialogueFile << exportToLua();
		dialogueFile.close();
	}
	else {
		ERROR("[Dialogue]: Lua export: Unable to open file: " + luaPath);
		return false;
	}

	LOG("[Dialogue]: Lua export complete to file: " + luaPath);

	// then the sql export
	std::string sqlPath = G_CONF.sqlFolder + "/insert_text_dl_" + m_npcID + "_utf8.sql";
	std::ofstream sqlFile(sqlPath, std::ios::trunc);
	if (sqlFile.is_open()) {
		sqlFile << exportToSQL();
		sqlFile.close();
	}
	else {
		ERROR("[Dialogue]: SQL export: Unable to open file: " + sqlPath);
		return false;
	}

	LOG("[Dialogue]: SQL export complete to file: " + sqlPath);
	return true;
}

void Dialogue::recursiveLuaExport(DialogueNode* node, std::string& stack, int indentationLevel) const {
	if (node == nullptr) return;

	stack.append("\n");
	stack.append(node->exportToLua(indentationLevel));
	stack.append("\n");

	for (auto child : node->getLinkNodes()) {
		
		if (child->condition == nullptr) {
			recursiveLuaExport(child->nextNode, stack, indentationLevel);
		}
		else {
			stack.append(tabs(indentationLevel) + "if (" + child->condition->exportToLua() + ") then \n");
			recursiveLuaExport(child->nextNode, stack, indentationLevel + 1);
			stack.append(tabs(indentationLevel) + "end\n\n");
		}
	}
}

std::string Dialogue::exportToLua() const {
	std::stringstream ss;
	int indentationLevel = 0;
	ss << "-- Dialogue for NPC \"" << m_npcID << "\"\n";

	ss << "loadDialogue = function(DL) \n";

	std::string stack = "";
	recursiveLuaExport(m_startNode, stack, indentationLevel + 1);
	ss << stack;

	ss << "end";
	return ss.str();
}

void Dialogue::recursiveSQLExport(DialogueNode* node, std::string& stack) const {
	if (node == nullptr) return;

	stack.append(node->exportToSQL());

	for (auto child : node->getLinkNodes()) {
		stack.append(child->exportToSQL());
	}

	for (auto child : node->getLinkNodes()) {
		recursiveSQLExport(child->nextNode, stack);
	}
}

std::string Dialogue::exportToSQL() const {
	std::string stack = "";
	recursiveSQLExport(m_startNode, stack);
	return stack;
}