#include "Dialogue.h"
#include "Nodes/StartNode.h"
#include "Nodes/ChoiceNode.h"
#include "Nodes/NPCNode.h"
#include "FileIO/ConfigurationIO.h"
#include "ApplicationState.h"

#include <sstream>
#include <iostream>
#include <fstream>

#include <sys/types.h>
#include <sys/stat.h>

Dialogue::Dialogue(const std::string& npcID) {
	m_npcID = npcID;
	m_startNode = new StartNode();
}

Dialogue::~Dialogue() {
	delete m_startNode;
}

StartNode* Dialogue::getStartNode() const {
	return m_startNode;
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

	// then create the file
	std::string path = folderPath + "/dl_" + m_npcID + ".lua";
	std::ofstream dialogueFile(path, std::ios::trunc);
	if (dialogueFile.is_open()) {
		dialogueFile << exportToLua();
		dialogueFile.close();
	}
	else {
		ERROR("[Dialogue]: Unable to open file: " + path);
		return false;
	}
	return true;
}

std::string Dialogue::exportToLua() const {
	std::stringstream ss;
	int indentationLevel = 0;
	ss << "-- Dialogue for NPC \"" << m_npcID << "\"\n";

	ss << "loadDialogue = function(DL) \n";

	ss << m_startNode->exportToLua(indentationLevel + 1);

	ss << "end";
	return ss.str();
}