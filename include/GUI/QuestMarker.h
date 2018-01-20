#pragma once

#include "global.h"
#include "World/GameObject.h"
#include "Structs/QuestData.h"

class CharacterCore;
class TooltipComponent;

// a quest marker on a quest entry or on the minimap
class QuestMarker : public GameObject {
public:
	QuestMarker(const QuestData& questData, const CharacterCore* core);
	virtual ~QuestMarker() {}

	void setPosition(const sf::Vector2f& pos) override;
	void render(sf::RenderTarget& renderTarget) override;

	bool isActive() const;
	virtual void setActive(bool active);

	GameObjectType getConfiguredType() const override { return GameObjectType::_Interface; }
	
	static const int SIZE;

protected:
	QuestData m_questData;
	const CharacterCore* m_characterCore;
	TooltipComponent* m_tooltipComponent;

private:
	sf::Sprite m_sprite;
	bool m_isActive;
};