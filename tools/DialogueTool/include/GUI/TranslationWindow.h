#pragma once

#include "global.h"

struct NodeTranslation;

// The translation window shows translations for npc, cendric and choice nodes
class TranslationWindow final {

public:
	TranslationWindow();
	
	void setNodeTranslation(NodeTranslation* translation);
	void update();

private:
	NodeTranslation* m_translation = nullptr;
};