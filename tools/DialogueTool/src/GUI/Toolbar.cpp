#include "GUI/Toolbar.h"
#include "GUI/NodeWindow.h"
#include "Nodes/StartNode.h"
#include "ApplicationState.h"
#include "Dialogue.h"
#include "FileIO/InsertScriptParser.h"
#include "FileIO/TranslationParser.h"

#define NPC_PREFIX "npc_"

#ifdef _WIN32
#include "dirent/dirent.h"
#else
#include <dirent.h>
#endif

inline bool ends_with(const std::string& value, const std::string& ending) {
	if (ending.size() > value.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

Toolbar::Toolbar(NodeWindow* nodeWindow) {
	m_nodeWindow = nodeWindow;
	strcpy(m_npcFolder, G_CONF.npcFolder.c_str());
	strcpy(m_sqlFolder, G_CONF.sqlFolder.c_str());
	strcpy(m_dialogueFolder, G_CONF.dialogueFolder.c_str());
	strcpy(m_dialogueID, NPC_PREFIX);

	m_scriptParser = new InsertScriptParser();
	m_translationParser = new TranslationParser();
}

Toolbar::~Toolbar() {
	for (int i = 0; i < m_folderSaves.size(); i++) {
		delete[] m_folderSaves[i];
	}
	m_folderSaves.clear();
	delete m_scriptParser;
}

void Toolbar::notifyNewDialogue() {
	if (G_DIA == nullptr) {
		m_nodeWindow->setNode(nullptr);
	}
	else {
		m_nodeWindow->setNode(G_DIA->getStartNode());
		strcpy(m_dialogueName, G_DIA->getDialogueName().c_str());
		strcpy(m_dialogueID, G_DIA->getNpcID().c_str());
	}
}

void Toolbar::update() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			showFileMenu();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit")) {
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Parse")) {
			m_scriptParser->showParseMenu();
			m_translationParser->showParseMenu();
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void Toolbar::showFileMenu() {
	if (ImGui::BeginMenu("New Dialogue")) {
		ImGui::InputText("NPC ID", m_dialogueID, IM_ARRAYSIZE(m_dialogueID));
		if (ImGui::Button("Create")) { createDialogue(); };
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Load Dialogue ...")) {
		if (m_needReloadFolder) {
			loadFolderSaves();
			m_needReloadFolder = false;
		}
		if (m_folderSaves.empty()) {
			ImGui::Text("No saves found...");
		}
		else {
			static int listbox_item_current = 1;
			ImGui::ListBox("", &listbox_item_current, &m_folderSaves[0], static_cast<int>(m_folderSaves.size()), 5);
			if (ImGui::Button("Load")) {
				std::string trimmed = getFileName(m_folderSaves.at(listbox_item_current));
				g_state->loadDialogue(trimmed);
				notifyNewDialogue();
			};
		}
		ImGui::EndMenu();
	}
	else {
		m_needReloadFolder = true;
	}
	if (ImGui::MenuItem("Save Dialogue", "Ctrl+S")) { if (G_DIA != nullptr) G_DIA->exportToDia(); }
	if (ImGui::BeginMenu("Save Dialogue As ...")) {
		if (G_DIA != nullptr) {
			ImGui::InputText("File name", m_dialogueName, IM_ARRAYSIZE(m_dialogueName));
			if (ImGui::Button("Save As")) {
				std::string trimmed = getFileName(m_dialogueName);
				strcpy(m_dialogueName, trimmed.c_str());
				G_DIA->setDialogueName(m_dialogueName);
				G_DIA->exportToDia();
			};
		}
		ImGui::EndMenu();
	}
	if (ImGui::MenuItem("Export Dialogue ...")) { if (G_DIA != nullptr) G_DIA->exportDialogue(); }
	ImGui::Separator();
	if (ImGui::BeginMenu("Options")) {
		if (ImGui::InputText("NPC Folder", m_npcFolder, IM_ARRAYSIZE(m_npcFolder))) { G_CONF.npcFolder = std::string(m_npcFolder); };
		if (ImGui::InputText("SQL Folder", m_sqlFolder, IM_ARRAYSIZE(m_sqlFolder))) { G_CONF.sqlFolder = std::string(m_sqlFolder); };
		if (ImGui::InputText("Dialogue Folder", m_dialogueFolder, IM_ARRAYSIZE(m_dialogueFolder))) { G_CONF.dialogueFolder = std::string(m_dialogueFolder); };
		ImGui::EndMenu();
	}
	if (ImGui::MenuItem("Quit", "Alt+F4")) { g_state->requestQuit();  }
}

void Toolbar::createDialogue() {
	if (std::string(m_dialogueID).compare(0, std::string(NPC_PREFIX).size(), NPC_PREFIX) != 0) {
		std::string newID = (NPC_PREFIX + std::string(m_dialogueID)).substr(0, 49);
		strcpy(m_dialogueID, newID.c_str());
	}
	g_state->requestNewDialogue(std::string(m_dialogueID));
	notifyNewDialogue();
}

void Toolbar::loadFolderSaves() {
	for (int i = 0; i < m_folderSaves.size(); i++) {
		delete[] m_folderSaves[i];
	}
	m_folderSaves.clear();

	DIR* dir;
	struct dirent* de;

	dir = opendir(G_CONF.dialogueFolder.c_str());
	int nr = 0;
	std::vector<std::string> folderSaves;
	while (dir) {
		de = readdir(dir);
		if (!de) break;
		if (de->d_type == DT_DIR) {
			// we don't consider directory folders
			continue;
		}
		if (!ends_with(std::string(de->d_name), ".dia")) {
			// we only consider .dia files in this folder
			LOG("[DialogueLoader] There is a file of the wrong type in the dialogue folder: " + std::string(de->d_name));
			continue;
		}
		
		std::string location = std::string(de->d_name);
		folderSaves.push_back(location);
	}

	for (int index = 0; index < folderSaves.size(); ++index) {
		char* newChar = new char[strlen(folderSaves.at(index).c_str()) + 1];
		strcpy(newChar, folderSaves.at(index).c_str());
		m_folderSaves.push_back(newChar);
	}

	closedir(dir);
}

