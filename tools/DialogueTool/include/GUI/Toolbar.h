#pragma once

#include "global.h"

// main window toolbar
class Toolbar {

public:
	Toolbar();
	void update();

private:
	void showFileMenu();

	char m_npcFolder[100];
	char m_sqlFolder[100];
	char m_dialogueFolder[100];
};