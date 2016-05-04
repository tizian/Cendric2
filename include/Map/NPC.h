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
class NPC : public virtual MapMovableGameObject {
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
	void reloadRoutine();

	GameObjectType getConfiguredType() const override;
	const NPCData& getNPCData() const;

private:
	NPCData m_NPCdata;
	NPCRoutine m_routine;
	MapMainCharacter* m_mainChar;

	const float TALKING_RANGE = 100.f;

	void trySetDialogue();
	void turnToMainchar();
};