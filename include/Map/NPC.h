#pragma once

#include "global.h"
#include "Map/MapMovableGameObject.h"
#include "ResourceManager.h"
#include "TextProvider.h"

#include "GUI/BitmapText.h"
#include "Structs/NPCData.h"
#include "Map/NPCRoutine.h"

class MapScreen;
class MapMainCharacter;

// A npc on a map
class NPC final : public MapMovableGameObject {
public:
	NPC(MapScreen* mapScreen);
	void load(const NPCData& data);
	void onRightClick() override;
	void onLeftClick() override;
	void update(const sf::Time& frameTime) override;
	void checkCollisionWithMainChar();

	float getConfiguredMaxVelocityYUp() const override; 
	float getConfiguredMaxVelocityYDown() const override;
	float getConfiguredMaxVelocityX() const override;

	void setDialogueID(const std::string& id);
	// if set to true, the npc will begin a dialogue with cendric when he enters its bounding box.
	void setTalkingActive(bool active);
	// if set to true, the npc can be talked to (cendrics initiative)
	void setTalkingEnabled(bool enabled);
	// if set to false, the npcs routine will not reload if a condition has changed. 
	void setReloadEnabled(bool enabled);
	// this npc will reload its routine in the next update
	void notifyReloadNeeded();

	GameObjectType getConfiguredType() const override;
	const NPCData& getNPCData() const;

private:
	void reloadRoutine();

	NPCData m_NPCdata;
	NPCRoutine m_routine;
	MapMainCharacter* m_mainChar;

	const float TALKING_RANGE = 100.f;
	bool m_routineReloadNeeded = false;
	bool m_routineReloadEnabled = false;

	void trySetDialogue();
	void turnToMainchar();
};