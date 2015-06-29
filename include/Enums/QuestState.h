#pragma once

// used for progress (character core)
enum class QuestState
{
	Void,
	Started,
	Completed,
	Failed,
	Aborted,
	MAX,
};