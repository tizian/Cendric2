#include "Nodes/NodeTranslation.h"
#include "ApplicationState.h"
#include "Dialogue.h"

NodeTranslation::NodeTranslation(int nodeTag, const std::string& tag_) {
	strcpy(tag, ("DL_" + tag_ + "_" + std::to_string(nodeTag)).c_str());
	strcpy(englishTranslation, "");
	strcpy(germanTranslation, "");
	strcpy(swissgermanTranslation, "");
	strcpy(itemID, "");
}