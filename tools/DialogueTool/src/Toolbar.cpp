#include "Toolbar.h"

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
	if (ImGui::MenuItem("New Dialogue")) {}
	if (ImGui::MenuItem("Open Dialogue", "Ctrl+O")) {}
	if (ImGui::MenuItem("Save Dialogue", "Ctrl+S")) {}
	if (ImGui::MenuItem("Save Dialogue As ...")) {}
	if (ImGui::MenuItem("Export Dialogue ...")) {}
	ImGui::Separator();
	if (ImGui::BeginMenu("Options")) {
		ImGui::InputText("NPC Folder", m_npcFolderPath, IM_ARRAYSIZE(m_npcFolderPath));
		ImGui::InputText("SQL Folder", m_sqlFolderPath, IM_ARRAYSIZE(m_sqlFolderPath));
		ImGui::InputText("Dialogue Folder", m_dialogueFolderPath, IM_ARRAYSIZE(m_dialogueFolderPath));
		ImGui::EndMenu();
	}
	if (ImGui::MenuItem("Quit", "Alt+F4")) {}
}

