#pragma once

#include "global.h"
#include "World/GameObject.h"
#include "Structs/QuestData.h"

class CharacterCore;
class TooltipWindowComponent;

enum class QuestMarkerState {
	Inactive,
	Unreachable,
	InProgress,
	Completed,
};

// a quest marker on a quest entry or on the minimap
class QuestMarker : public GameObject {
public:
	QuestMarker(const QuestData& questData, CharacterCore* core);
	virtual ~QuestMarker() {}

	void setPosition(const sf::Vector2f& pos) override;
	void render(sf::RenderTarget& renderTarget) override;

	bool isActive() const;
	void setActive(bool active);

	GameObjectType getConfiguredType() const override { return _Interface; }
	
	static const int SIZE;

	static std::vector<QuestMarkerData> getCurrentStepData(const QuestData& data, CharacterCore* core);

protected:
	QuestData m_questData;
	CharacterCore* m_characterCore;
	TooltipWindowComponent* m_tooltipComponent;

	QuestMarkerState m_markerState;

	virtual void updateQuestMarkerState();
	virtual void execSetActive() {};

private:
	sf::Sprite m_sprite;
	bool m_isActive;
};