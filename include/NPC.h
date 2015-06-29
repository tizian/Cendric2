#pragma once

#include "global.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "TextProvider.h"

#include "Enums/NpcID.h"

#include "GUI/BitmapText.h"

class MapMainCharacter;

// A npc on a map
class NPC : public GameObject
{
public:
	void load(MapMainCharacter* mainChar, NpcID id);
	void onMouseOver() override;
	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;
	void checkCollisionWithMainChar();

	void setTooltipText(const std::wstring& tooltip);
	// if set to true, the npc will begin a dialogue with cendric when he enters its bounding box.
	void setActiveTalking(bool talksActive);

	GameObjectType getConfiguredType() const override;

private:
	MapMainCharacter* m_mainChar;
	NpcID m_npcID;

	BitmapText m_tooltipText;
	sf::Time m_tooltipTime = sf::Time::Zero;
	bool m_talksActive;
};