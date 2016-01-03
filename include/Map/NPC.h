#pragma once

#include "global.h"
#include "Map/MapMovableGameObject.h"
#include "ResourceManager.h"
#include "TextProvider.h"

#include "GUI/BitmapText.h"
#include "Structs/NPCData.h"
#include "Map/NPCRoutine.h"

class MapMainCharacter;

// A npc on a map
class NPC : virtual public MapMovableGameObject {
public:
	NPC(Map* map) : MapMovableGameObject(map) {}
	void load(MapMainCharacter* mainChar, const NPCData& data);
	void onMouseOver() override;
	void onInteractKey() override;
	void onRightClick() override;
	void renderAfterForeground(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;
	void setPosition(const sf::Vector2f& pos) override;
	void checkCollisionWithMainChar();

	float getConfiguredMaxVelocityYUp() const override;
	float getConfiguredMaxVelocityYDown() const override;
	float getConfiguredMaxVelocityX() const override;

	void setTooltipText(const std::string& tooltip);
	// if set to true, the npc will begin a dialogue with cendric when he enters its bounding box.
	void setTalksActive(bool talksActive);
	void setDialogueID(const std::string& id);

	GameObjectType getConfiguredType() const override;

private:
	MapMainCharacter* m_mainChar;
	NPCData m_NPCdata;
	NPCRoutine m_routine;

	BitmapText m_tooltipText;
	sf::Time m_tooltipTime = sf::Time::Zero;
	const float TALKING_RANGE = 30.f;

	void turnToMainchar();
};