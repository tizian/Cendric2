#pragma once

#include "global.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "TextProvider.h"

#include "GUI/BitmapText.h"
#include "Structs/NPCBean.h"

class MapMainCharacter;

// A npc on a map
class NPC : public GameObject {
public:
	NPC() : GameObject() {}
	void load(MapMainCharacter* mainChar, const NPCBean& bean);
	void onMouseOver() override;
	void onInteractKey() override;
	void onRightClick() override;
	void renderAfterForeground(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;
	void checkCollisionWithMainChar();

	void setTooltipText(const std::string& tooltip);
	// if set to true, the npc will begin a dialogue with cendric when he enters its bounding box.
	void setTalksActive(bool talksActive);
	void setDialogueID(const std::string& id);

	GameObjectType getConfiguredType() const override;

private:
	MapMainCharacter* m_mainChar;
	NPCBean m_bean;

	BitmapText m_tooltipText;
	sf::Time m_tooltipTime = sf::Time::Zero;
	const float TALKING_RANGE = 30.f;
};