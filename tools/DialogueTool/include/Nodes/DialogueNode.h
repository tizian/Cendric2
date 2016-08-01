#pragma once

#include "global.h"

enum class DialogueNodeType {
	Start, // equals reload and start again if chosen in combo
	NPC,
	Choice,
	Cendric,
	Trade,
	End // End dialogue
};

// A node in the dialogue, abstract class. It holds methods for exporting.
class DialogueNode {
public:
	DialogueNode(int tag);
	virtual ~DialogueNode() {};

	virtual std::string exportToLua(int indentationLevel) const = 0;
	
	const std::string& getDescription();
	virtual DialogueNodeType getType() const = 0;
	int getTag() const;

	static const char* NODE_TYPES;

protected:
	virtual std::string getName() const = 0;

private:
	std::string m_description;
	int m_tag;
};