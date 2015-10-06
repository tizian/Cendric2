#pragma once

#include "global.h"
#include "Map.h"
#include "MapMainCharacter.h"
#include "Screens/GameScreen.h"
#include "ResourceManager.h"
#include "Screens/LoadingScreen.h"
#include "GUI/DialogueWindow.h"
#include "MapInterface.h"

class MapScreen : public GameScreen {
public:
	MapScreen(const std::string& mapID, CharacterCore* core);
	// called by the loadingScreen
	void load();
	// loads lights that need a seperate rendertexture
	void loadForRenderTexture();

	Screen* update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) override;
	void setDialogue(const NPCBean& bean);

	void execOnEnter(const Screen* previousScreen) override;
	void execOnExit(const Screen* nextScreen) override;

private:
	Map m_currentMap;
	std::string m_mapID;
	MapMainCharacter* m_mainChar;
	bool m_isOnLevelEntry = true;
	DialogueWindow* m_dialogueWindow = nullptr;
};