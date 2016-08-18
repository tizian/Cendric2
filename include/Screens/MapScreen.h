#pragma once

#include "global.h"
#include "Map/Map.h"
#include "Map/MapMainCharacter.h"
#include "Screens/WorldScreen.h"
#include "ResourceManager.h"
#include "Screens/LoadingScreen.h"
#include "GUI/DialogueWindow.h"
#include "GUI/CookingWindow.h"
#include "Map/MapInterface.h"

class MapScreen : public WorldScreen {
public:
	MapScreen(const std::string& mapID, CharacterCore* core);
	// called by the loadingScreen
	void load();
	// loads lights that need a seperate rendertexture
	void loadForRenderTexture();

	void execUpdate(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) override;
	void renderEquipment(sf::RenderTarget& renderTarget);
	void setDialogue(NPC* npc);
	void setCooking();

	void execOnEnter(const Screen* previousScreen) override;
	void execOnExit(const Screen* nextScreen) override;
	void notifyConditionAdded(const std::string& conditionType, const std::string& condition) override;
	void notifyItemEquip(const std::string& itemID, ItemType type) override;

	const Map* getWorld() const override;
	const MapData* getWorldData() const override;
	MapMainCharacter* getMainCharacter() const override;
	bool exitWorld() override;
	void notifyBackFromMenu() override;

private:
	// deletes all overlays and sets them null
	void clearOverlays();
	bool isOverlayActive();
	bool isOverlayVisible();
	void quicksave() override;

	void updateFogOfWar();

private:
	Map m_currentMap;
	std::string m_mapID;
	MapMainCharacter* m_mainChar;
	DialogueWindow* m_dialogueWindow = nullptr;
	CookingWindow* m_cookingWindow = nullptr;

	void handleDialogueWindow(const sf::Time& frameTime);
	void handleCookingWindow(const sf::Time& frameTime);
};