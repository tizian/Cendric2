#include "Structs/NPCData.h"
#include "GlobalResource.h"

void NPCData::calculateDefaultFromID() {
	if (dialogueID.empty())
		dialogueID = GlobalResource::NPC_FOLDER + id + "/dl_" + id + ".lua";

	if (routineID.empty())
		routineID = GlobalResource::NPC_FOLDER + id + "/ru_" + id + ".lua";

	if (spritesheetpath.empty())
		spritesheetpath = GlobalResource::NPC_FOLDER + id + "/spritesheet_" + id + ".png";

	if (textType.empty())
		textType = "dl_" + id;

	if (dialoguetexture.empty())
		dialoguetexture = GlobalResource::NPC_FOLDER + id + "/" + id + ".png";
}