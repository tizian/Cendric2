#include "GUI/Toolbar.h"
#include "ApplicationState.h"

Toolbar::Toolbar() {
	strcpy(m_npcFolder, G_CONF.npcFolder.c_str());
	strcpy(m_sqlFolder, G_CONF.sqlFolder.c_str());
	strcpy(m_dialogueFolder, G_CONF.dialogueFolder.c_str());
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
		ImGui::EndMainMenuBar();
	}
}

void Toolbar::showFileMenu() {
	if (ImGui::MenuItem("New Dialogue")) { g_state->requestNewDialogue(); }
	if (ImGui::MenuItem("Open Dialogue", "Ctrl+O")) {}
	if (ImGui::MenuItem("Save Dialogue", "Ctrl+S")) {}
	if (ImGui::MenuItem("Save Dialogue As ...")) {}
	if (ImGui::MenuItem("Export Dialogue ...")) {}
	ImGui::Separator();
	if (ImGui::BeginMenu("Options")) {
		if (ImGui::InputText("NPC Folder", m_npcFolder, IM_ARRAYSIZE(m_npcFolder))) { G_CONF.npcFolder = std::string(m_npcFolder); };
		if (ImGui::InputText("SQL Folder", m_sqlFolder, IM_ARRAYSIZE(m_sqlFolder))) { G_CONF.sqlFolder = std::string(m_sqlFolder); };
		if (ImGui::InputText("Dialogue Folder", m_dialogueFolder, IM_ARRAYSIZE(m_dialogueFolder))) { G_CONF.dialogueFolder = std::string(m_dialogueFolder); };
		ImGui::EndMenu();
	}
	if (ImGui::MenuItem("Quit", "Alt+F4")) { g_state->requestQuit();  }
}

