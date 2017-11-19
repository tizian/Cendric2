#pragma once

#include "global.h"

class NodeWindow;
class InsertScriptParser;
class TranslationParser;

// main window toolbar
class Toolbar final {
public:
	Toolbar(NodeWindow* nodeWindow);
	~Toolbar();
	void update();
	void notifyNewDialogue();

private:
	void showFileMenu();
	void createDialogue();
	void loadFolderSaves();

	char m_npcFolder[100];
	char m_sqlFolder[100];
	char m_dialogueFolder[100];
	char m_dialogueID[50];
	char m_dialogueName[50];

	bool m_needReloadFolder = true;
	NodeWindow* m_nodeWindow;
	std::vector<const char*> m_folderSaves;

	InsertScriptParser* m_scriptParser;
	TranslationParser* m_translationParser;
};