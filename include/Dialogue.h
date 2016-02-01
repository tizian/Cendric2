#pragma once

#include "global.h"
#include "ResourceManager.h"
#include "TextProvider.h"
#include "Structs/DialogueNode.h"

class WorldScreen;
class DialogueWindow;

// A dialogue with conditions, choices and text
class Dialogue {
public:
	void load(const std::string& id, WorldScreen* screen, DialogueWindow* window);
	const std::string& getID() const;
	void addNode(int tag, const DialogueNode& node);
	void setNextNode(int tag);
	void setRoot(int root);
	bool updateWindow();

private:
	WorldScreen* m_screen;
	DialogueWindow* m_window;
	DialogueNode* m_currentNode = nullptr;
	std::string m_id;
	std::map<int, DialogueNode> m_nodes;
};