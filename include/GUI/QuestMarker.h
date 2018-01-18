#pragma once

#include "global.h"
#include "World/GameObject.h"
#include "Structs/QuestData.h"

// a quest marker on a quest entry or on the minimap
class QuestMarker final : public GameObject {
public:
	QuestMarker(const QuestData& questData);

	void setPosition(const sf::Vector2f& pos) override;
	void render(sf::RenderTarget& renderTarget) override;

	bool isActive();
	void setActive(bool active);
	
	static const float SIZE;

private:
	sf::Sprite m_sprite;
	QuestData m_questData;

	bool m_isActive;
};