#pragma once

#include "global.h"

class NodeWindow;
class DialogueNode;

// The tree window shows an overview of the nodes in a dialogue
class TreeWindow {

public:
	TreeWindow(NodeWindow* nodeWindow);
	void update();

private:
	void recursiveShowNode(DialogueNode* node, int depth);
	NodeWindow* m_nodeWindow;
};