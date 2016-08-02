#pragma once

#include "global.h"

enum class DialogueNodeType {
	End, // End dialogue
	NPC,
	Choice,
	Cendric,
	Trade,
	Start, // equals reload and start again if chosen in combo
};

class DialogueNode;
class NodeCondition;

// This node acts as a link between nodes and their children.
struct LinkNode {
	~LinkNode();

	// adds the currently selected condition template to the 
	// condition string
	void addConditionTemplate();
	// links a new node of the current preselected node type to this node
	void linkNodeTemplate();
	// returns the tag of the next node (can be -1 for end or -2 for reload)
	int getNextTag();

	int nextType = static_cast<int>(DialogueNodeType::End);
	DialogueNode* nextNode = nullptr;
	bool isReload = false;

	NodeCondition* condition = nullptr;
	int currentPreselectedCondition = 0;
	int currentPreselectedNodetype = 0;
};

// A node in the dialogue, abstract class. It holds methods for exporting.
class DialogueNode {
public:
	DialogueNode(int tag);
	virtual ~DialogueNode();

	virtual std::string exportToLua(int indentationLevel) const = 0;
	virtual std::string exportToSQL() const { return ""; }
	virtual void addLinkNode(LinkNode* node);
	
	const std::string& getDescription();
	virtual DialogueNodeType getType() const = 0;
	virtual float getButtonHue() const = 0;
	int getTag() const;

	std::vector<LinkNode*>& getLinkNodes();

	static const char* NODE_TYPES;

protected:
	virtual std::string getName() const = 0;
	// start and choice nodes can have more than one child
	// other nodes will have at least and at most one child.
	std::vector<LinkNode*> m_children;
	
private:
	std::string m_description;
	int m_tag;
};