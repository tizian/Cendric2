#pragma once

#include "global.h"
#include "World/AnimatedGameObject.h"
#include "ResourceManager.h"
#include "World/LightObject.h"
#include "Beans/ItemEquipmentBean.h"
#include "Beans/ItemEquipmentLightBean.h"
#include "Beans/ItemEquipmentParticleBean.h"
#include "Enums/ItemType.h"

class LevelMainCharacter;
class LightComponent;
class ParticleComponent;
struct ParticleComponentData;

// Equipment for cendric inside a level
class LevelEquipment final : public AnimatedGameObject {
public:
	LevelEquipment(LevelMainCharacter* mainChar);

	void load(const ItemEquipmentBean* itemBean, ItemType type);
	void loadComponents(const ItemEquipmentLightBean* light, const ItemEquipmentParticleBean* particles);
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
	void setParticleComponent(const ParticleComponentData& particleData, const sf::Vector2f& offset, const sf::Vector2f& goalOffset);
	LightComponent* m_lightComponent = nullptr;
	ParticleComponent* m_particleComponent = nullptr;
	bool m_isFacingRight;
	sf::Vector2f m_position;

	void calculatePositionAccordingToMainChar(sf::Vector2f& position) const;
	void updateParticlesVisibility() const;
	bool m_isCopyingMainCharColor = true;

	static const int EQ_SIZE;
};