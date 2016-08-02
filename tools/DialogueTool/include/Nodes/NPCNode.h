#pragma once

#include "global.h"
#include "Nodes/DialogueNode.h"

struct NodeTranslation;
struct NodeTrigger;

// NPC Node, it holds a link to the next node, 0 to n triggers and a translation
class NpcNode : public virtual DialogueNode {
public:
	NpcNode();
	~NpcNode();

	std::string exportToLua(int indentationLevel) const override;
	std::string exportToSQL() const override;

	DialogueNodeType getType() const override;
	float getButtonHue() const override;
	NodeTranslation* getTranslation();
	std::vector<NodeTrigger*>& getTriggers();

	// adds a new trigger with the selected trigger template
	void addTriggerTemplate();
	int currentPreselectedTrigger = 0;

protected:
	std::string getName() const override { return "NPC Node"; }

private:
	NodeTranslation* m_translation;
	std::vector<NodeTrigger*> m_triggers;
};