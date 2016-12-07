#include "Nodes/Condition.h"

const char* Condition::CONDITION_TYPES =
"Condition\0Quest State\0Quest Complete\0Has Item\0Get Reputation\0Get Guild\0Is Spell Learned\0\0";

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
	case ConditionType::GetReputation:
		return "DL:getReputation(\"" + s1 + "\")";
	case ConditionType::GetGuild:
		return "DL:getGuild()";
	case ConditionType::IsSpellLearned:
		return "DL:isSpellLearned(\"" + std::to_string(i1) + "\")";
	default:
		break;
	}

	return "--[[ ERROR: condition could not be parsed --]]";
}