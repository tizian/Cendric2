#pragma once

#include "global.h"

// main window toolbar
class Toolbar {

public:
	void update();

private:
	void showFileMenu();

	char m_sqlFolderPath[100];
	char m_dialogueFolderPath[100];
	char m_npcFolderPath[100];
};