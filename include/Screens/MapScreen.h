#pragma once

#include "global.h"
#include "Map.h"
#include "MapMainCharacter.h"
#include "Screen.h"
#include "ResourceManager.h"
#include "Screens/LoadingScreen.h"
#include "GUI/DialogueWindow.h"

#include "Enums/MapID.h"

class MapScreen : public Screen
{
public:
	MapScreen(MapID mapID, CharacterCore* core);
	// called by the loadingScreen
	void load();

	Screen* update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) override;
	void setDialogue(NPCID npc, DialogueID id);

	void execOnEnter(const Screen* previousScreen) override;
	void execOnExit(const Screen* nextScreen) override;

private:
	Map m_currentMap;
	MapID m_mapID;
	MapMainCharacter* m_mainChar;
	bool m_isOnLevelEntry = true;
	DialogueWindow* m_dialogueWindow = nullptr;
};