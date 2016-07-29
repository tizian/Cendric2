#pragma once

#include "global.h"

class Toolbar;

// top level controller over the main window
// holds the toolbar, the tree window and the node windows
class MainWindow {
public:
	MainWindow();
	~MainWindow();

	void update();
	void render();

private:
	Toolbar* m_toolbar;
};