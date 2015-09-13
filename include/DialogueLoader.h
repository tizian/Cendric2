#pragma once

#include "global.h"
#include "Dialogue.h"
#include "Enums/NPCState.h"
#include "Enums/QuestState.h"

#include "LuaBridge/LuaBridge.h"

class CharacterCore;

// helper class to load lua files for dialogues
// current class to mess around with lua
class DialogueLoader
{
public:
	DialogueLoader(Dialogue& dialogue, CharacterCore* core);
	void loadDialogue();

	// methods for lua
	const bool hasNeverTalkedTo(std::string npcID);
	const bool isQuestFinished(std::string questID);

private:
	Dialogue& m_dialogue;
	CharacterCore* m_core;
	
};