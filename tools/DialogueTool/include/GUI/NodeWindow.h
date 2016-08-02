#pragma once

#include "global.h"

class DialogueNode;
class TranslationWindow;

// The node window displays a specific node. It can handle all kinds of nodes. 
class NodeWindow {

public:
	NodeWindow(TranslationWindow* translationWindow);
	void update();

	void setNode(DialogueNode* node);

private:
	// start node
	void showStartNodeWindow();

	// choice node
	void showChoiceNodeWindow();

	// npc/cendric node
	void showTriggerableNode();

	// trade node
	void showTradeNodeWindow();

	DialogueNode* m_node = nullptr;
	TranslationWindow* m_translationWindow;
};