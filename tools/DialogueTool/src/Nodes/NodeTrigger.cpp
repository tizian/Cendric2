#include "Nodes/NodeTrigger.h"

const char* NodeTrigger::TRIGGER_TYPES = "Change Quest State\0Add Quest Progress\0Add Quest Description\0Add Condition Progress\0Add Reputation Progress\0Add Hint\0Add Item\0Equip Item\0Remove Item\0Add Gold\0Remove Gold\0Learn Spell\0Start Level\0Start Map\0Start Cutscene\0Go To Node\0\0";

NodeTrigger::NodeTrigger(TriggerType type_) {
	type = type_;
	strcpy(rawTrigger, "");
}

std::string NodeTrigger::exportToLua() const {
	if (type == TriggerType::Raw) {
		return rawTrigger;
	}

	switch (type) {
	case TriggerType::ChangeQuestState:
		return "DL:changeQuestState(\"" + s1 + "\", \"" + s2 + "\")";
	case TriggerType::AddQuestProgress:
		return "DL:addQuestProgress(\"" + s1 + "\", \"" + s2 + "\")";
	case TriggerType::AddQuestDescription:
		return "DL:addQuestDescription(\"" + s1 + "\", " + std::to_string(i1) + ")";
	case TriggerType::AddConditionProgress:
		return "DL:addConditionProgress(\"" + s1 + "\", \"" + s2 + "\")";
	case TriggerType::AddReputationProgress:
		return "DL:addReputationProgress(\"" + s1 + "\", " + std::to_string(i1) + ")";
	case TriggerType::AddHint:
		return "DL:addHint(\"" + s1 + "\")";
	case TriggerType::AddItem:
		return "DL:addItem(\"" + s1 + "\", " + std::to_string(i1) + ")";
	case TriggerType::EquipItem:
		return "DL:equipItem(\"" + s1 + "\")";
	case TriggerType::AddGold:
		return "DL:addGold(" + std::to_string(i1) + ")";
	case TriggerType::RemoveGold:
		return "DL:removeGold(" + std::to_string(i1) + ")";
	case TriggerType::LearnSpell:
		return "DL:learnSpell(" + std::to_string(i1) + ")";
	case TriggerType::StartLevel:
		return "DL:startLevel(\"" + s1 + "\", " + std::to_string(i1) + ", " + std::to_string(i2) + ")";
	case TriggerType::StartMap:
		return "DL:startMap(\"" + s1 + "\", " + std::to_string(i1) + ", " + std::to_string(i2) + ")";
	case TriggerType::StartCutscene:
		return "DL:startCutscene(\"" + s1 + "\")";
	case TriggerType::gotoNode:
		return "DL:gotoNode(" + std::to_string(i1) + ")";
	default:
		break;
	}

	return "--[[ ERROR: node trigger could not be parsed --]]";
}
