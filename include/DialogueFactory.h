#pragma once

#include "global.h"
#include "Dialogue.h"


class CharacterCore;

// helper class to load lua files for dialogues
class DialogueFactory
{
public:
	void loadDialogue(Dialogue& dialogue, CharacterCore* core);
};