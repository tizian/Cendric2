#include "Nodes/NodeTranslation.h"
#include "ApplicationState.h"
#include "Dialogue.h"

NodeTranslation::NodeTranslation(int nodeTag) {
	strcpy(tag, ("DL_" + G_DIA->getNpcID() + "_" + std::to_string(nodeTag)).c_str());
	strcpy(englishTranslation, "");
	strcpy(germanTranslation, "");
	strcpy(swissgermanTranslation, "");
}