#pragma once

// used for progress (character core)
enum class NPCState
{
	VOID,
	Never_talked,
	Passive,
	Angry,
	MAX,
};

inline NPCState resolveNPCState(const std::string& state)
{
	if (state.compare("never_talked") == 0) return NPCState::Never_talked;
	if (state.compare("passive") == 0) return NPCState::Passive;
	if (state.compare("angry") == 0) return NPCState::Angry;
	return NPCState::VOID;
}