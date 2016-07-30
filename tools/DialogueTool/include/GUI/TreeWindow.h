#pragma once

#include "global.h"

class NodeWindow;

// The tree window shows an overview of the nodes in a dialogue
class TreeWindow {

public:
	TreeWindow(NodeWindow* nodeWindow);
	void update();

private:
	NodeWindow* m_nodeWindow;
};