#pragma once

// used for progress (character core)
enum class QuestState {
	VOID,
	Started, // a quest is started when it is accepted
	Completed, // a quest is completed when the reward is received
	Failed, // a quest has failed when it cannot be completed anymore (quest-npc is killed or something)
	MAX,
};

inline QuestState resolveQuestState(const std::string& state) {
	if (state == "void") return QuestState::VOID;
	if (state == "started") return QuestState::Started;
	if (state == "completed") return QuestState::Completed;
	if (state == "failed") return QuestState::Failed;
	return QuestState::MAX;
}