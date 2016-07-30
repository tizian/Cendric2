#include "Nodes/Condition.h"

const char* Condition::CONDITION_TYPES[] = { 
	"Condition", 
	"QuestState", 
	"QuestComplete", 
	"HasItem"
};

std::string Condition::exportToLua() const {
	switch (type) {
	case ConditionType::Condition:
		return "DL:isConditionFulfilled(\"" + s1 + "\", \"" + s2 + "\")";
	case ConditionType::HasItem:
		return "DL:hasItem(\"" + s1 + "\", " + std::to_string(i1) + ")";
	case ConditionType::QuestComplete:
		return "DL:isQuestComplete(\"" + s1 + "\")";
	case ConditionType::QuestState:
		return "DL:isQuestState(\"" + s1 + "\", \"" + s2 + "\")";
	default:
		break;
	}

	return "--[[ ERROR: condition could not be parsed --]]";
}