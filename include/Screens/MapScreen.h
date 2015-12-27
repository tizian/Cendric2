#pragma once

#include "global.h"
#include "Map/Map.h"
#include "Map/MapMainCharacter.h"
#include "Screens/GameScreen.h"
#include "ResourceManager.h"
#include "Screens/LoadingScreen.h"
#include "GUI/DialogueWindow.h"
#include "GUI/CookingWindow.h"
#include "Map/MapInterface.h"

class MapScreen : public GameScreen {
public:
	MapScreen(const std::string& mapID, CharacterCore* core);
	// called by the loadingScreen
	void load();
	// loads lights that need a seperate rendertexture
	void loadForRenderTexture();

	Screen* update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) override;
	void setDialogue(const NPCData& data);
	void setCooking();

	void execOnEnter(const Screen* previousScreen) override;
	void execOnExit(const Screen* nextScreen) override;
	// getter for the main char as it lays in the movable go vector
	MapMainCharacter* getMainCharacter();

private:
	Map m_currentMap;
	std::string m_mapID;
	MapMainCharacter* m_mainChar;
	bool m_isOnLevelEntry = true;
	DialogueWindow* m_dialogueWindow = nullptr;
	CookingWindow* m_cookingWindow = nullptr;
};