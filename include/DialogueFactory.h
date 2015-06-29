#pragma once

#include "global.h"
#include "Dialogue.h"

#include "Enums/DialogueID.h"

class CharacterCore;

// contains information for each and every dialogue
// will maybe be ported to a db-like structure (.csv)
class DialogueFactory
{
public:
	void loadDialogue(Dialogue& dialogue, CharacterCore* core);
};