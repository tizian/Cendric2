#pragma once

#include "global.h"
#include "ResourceManager.h"
#include "TextProvider.h"
#include "Structs/DialogueNode.h"

class WorldScreen;
class DialogueWindow;

// A dialogue with conditions, choices and text
class Dialogue final {
public:
	void reload(const std::string& id, WorldScreen* screen, DialogueWindow* window);
	const std::string& getID() const;
	void addNode(int tag, const DialogueNode& node);
	void setNextNode(int tag);
	void setRoot(int root);
	bool updateWindow();
	// returns whether the current dialogue is a choice node and one of them is an ending option
	bool isEndable() const;
	bool hasNode(int tag) const;
	bool isTradeNode(int nodeTag) const;

private:
	WorldScreen* m_screen;
	DialogueWindow* m_window;
	DialogueNode* m_currentNode;
	std::string m_id;
	std::map<int, DialogueNode> m_nodes;
};