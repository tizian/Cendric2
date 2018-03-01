#pragma once

#include "global.h"
#include "Map/Map.h"
#include "Map/MapMainCharacter.h"
#include "Screens/WorldScreen.h"
#include "Screens/LoadingScreen.h"
#include "GUI/DialogueWindow.h"
#include "GUI/CookingWindow.h"

class MapScreen final : public WorldScreen {
public:
	MapScreen(const std::string& mapID, CharacterCore* core);
	
	void execUpdate(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) override;
	void renderEquipment(sf::RenderTarget& renderTarget);
	void setDialogue(NPC* npc);
	void setCooking();

	void execOnEnter() override;
	void execOnExit() override;
	void notifyConditionAdded(const Condition& condition) override;
	void notifyItemEquip(const std::string& itemID, ItemType type) override;
	

	const Map* getWorld() const override;
	const MapData* getWorldData() const override;
	MapMainCharacter* getMainCharacter() const override;
	bool exitWorld() override;
	void notifyBackFromMenu() override;
	void notifyWaypointUnlocked() const;

	// the part of the world that can be loaded safely async.
	void loadAsync() override;
	// the rest, that cannot be loaded async
	void loadSync() override;

private:
	// deletes all overlays and sets them null
	void clearOverlays();
	bool isOverlayActive() const;
	bool isOverlayVisible() const;
	void quicksave() override;
	void notifyEquipmentReload() override;

	void updateFogOfWar();

private:
	Map m_currentMap;
	std::string m_mapID;
	MapMainCharacter* m_mainChar = nullptr;
	DialogueWindow* m_dialogueWindow = nullptr;
	CookingWindow* m_cookingWindow = nullptr;

	void handleDialogueWindow(const sf::Time& frameTime);
	void handleCookingWindow(const sf::Time& frameTime);
};