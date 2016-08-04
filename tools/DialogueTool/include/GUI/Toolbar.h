#pragma once

#include "global.h"

// main window toolbar
class Toolbar {

public:
	Toolbar();
	void update();

private:
	void showFileMenu();
	void createDialogue();

	char m_npcFolder[100];
	char m_sqlFolder[100];
	char m_dialogueFolder[100];
	char m_dialogueID[50];
	char m_dialogueName[50];
};