#pragma once

#include "global.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "TextProvider.h"

#include "Enums/DialogueID.h"

#include "GUI/BitmapText.h"

class MapMainCharacter;

// A npc on a map
class NPC : public GameObject
{
public:
	NPC() : GameObject() {}
	void load(MapMainCharacter* mainChar, const std::string& id);
	void onMouseOver() override;
	void onInteractKey() override;
	void onRightClick() override;
	void renderAfterForeground(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;
	void checkCollisionWithMainChar();

	void setTooltipText(const std::wstring& tooltip);
	// if set to true, the npc will begin a dialogue with cendric when he enters its bounding box.
	void setTalksActive(bool talksActive);
	void setDialogueID(DialogueID id);

	GameObjectType getConfiguredType() const override;

private:
	MapMainCharacter* m_mainChar;
	std::string m_npcID;

	BitmapText m_tooltipText;
	sf::Time m_tooltipTime = sf::Time::Zero;
	bool m_talksActive;
	DialogueID m_dialogueID;
	const float TALKING_RANGE = 100.f;
};