#pragma once

#include "global.h"
#include "World/AnimatedGameObject.h"
#include "ResourceManager.h"
#include "LightObject.h"
#include "Beans/ItemEquipmentBean.h"
#include "Beans/ItemEquipmentLightBean.h"
#include "Enums/ItemType.h"

class LevelMainCharacter;
class LightComponent;

// Equipment for cendric inside a level
class LevelEquipment final : public AnimatedGameObject {
public:
	LevelEquipment(LevelMainCharacter* mainChar);

	void load(const ItemEquipmentBean* itemBean, const ItemEquipmentLightBean* light, ItemType type);
	void update(const sf::Time& frameTime) override;
	
	GameObjectType getConfiguredType() const override;
	ItemType getItemType() const;

	void setPosition(const sf::Vector2f& position) override;

	// this value determines whether the level equipment takes into account the sprite color of the main character
	void setCopyingMainCharColor(bool value);

private:
	LevelMainCharacter* m_mainChar;
	bool m_hasTexture = false;
	ItemType m_itemType;

	void setLightComponent(const LightData& lightData);
	LightComponent* m_lightComponent = nullptr;
	bool m_isFacingRight;

	// calculates position according to m_mainChar.
	void calculatePositionAccordingToMainChar(sf::Vector2f& position) const;
	bool m_isCopyingMainCharColor = true;

	static const int EQ_SIZE;
};