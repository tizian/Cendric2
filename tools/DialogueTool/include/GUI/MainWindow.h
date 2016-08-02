#pragma once

#include "global.h"

class Toolbar;
class TreeWindow;
class ErrorWindow;
class NodeWindow;
class TranslationWindow;

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
	TreeWindow* m_treeWindow;
	TranslationWindow* m_translationWindow;
	ErrorWindow* m_errorWindow;
	NodeWindow* m_nodeWindow;
};