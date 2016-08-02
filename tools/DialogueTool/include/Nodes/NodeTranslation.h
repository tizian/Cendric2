#include "global.h"

#define MAX_CHOICE_SIZE 72
#define MAX_DIALOGUE_SIZE 360

// holds a translation for a node (npc, choice or cendric node)
struct NodeTranslation {
	NodeTranslation(int nodeTag);
	char tag[50];
	char englishTranslation[MAX_DIALOGUE_SIZE];
	char germanTranslation[MAX_DIALOGUE_SIZE];
	char swissgermanTranslation[MAX_DIALOGUE_SIZE];
};