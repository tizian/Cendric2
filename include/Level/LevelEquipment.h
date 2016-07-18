#pragma once

#include "global.h"
#include "AnimatedGameObject.h"
#include "ResourceManager.h"
#include "LightObject.h"

class LevelMainCharacter;
class LightComponent;

// Equipment for cendric inside a level
class LevelEquipment : public AnimatedGameObject {
public:
	LevelEquipment(LevelMainCharacter* mainChar);
	~LevelEquipment() {};

	void update(const sf::Time& frameTime) override;
	void setLightComponent(const LightData& lightData);

	void setHasTexture();

	GameObjectType getConfiguredType() const override;

	void setPosition(const sf::Vector2f& position) override;

	// this value determines whether the level equipment takes into account the sprite color of the main character
	void setCopyingMainCharColor(bool value);
	// calculates position according to m_mainChar.
	void calculatePositionAccordingToMainChar(sf::Vector2f& position) const;

private:
	LevelMainCharacter* m_mainChar;
	bool m_hasTexture = false;

	LightComponent* m_lightComponent = nullptr;
	bool m_isFacingRight;

	bool m_isCopyingMainCharColor = true;
};