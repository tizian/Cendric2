#pragma once

#include "global.h"
#include "AnimatedGameObject.h"
#include "ResourceManager.h"
#include "LightObject.h"

class LevelMainCharacter;

// Equipment for cendric inside a level
class LevelEquipment : public AnimatedGameObject {
public:
	LevelEquipment() : AnimatedGameObject() {}
	~LevelEquipment();

	void update(const sf::Time& frameTime) override;
	void loadEquipment(LevelMainCharacter* mainChar);

	void setTexturePath(const std::string& texturePath);
	void setLightObject(LightObject* lightObject);

	GameObjectType getConfiguredType() const override;

	void setPosition(const sf::Vector2f& position) override;
	void setDisposed() override;
	void setScreen(Screen* screen) override;

	// this value determines whether the level equipment takes into account the sprite color of the main character
	void setCopyingMainCharColor(bool value);

private:
	LevelMainCharacter* m_mainChar;
	std::string m_texturePath;

	LightObject* m_lightObject = nullptr;
	sf::Vector2f m_lightObjectOffset;

	bool m_isFacingRight;

	// calculates position according to m_mainChar.
	void calculatePositionAccordingToMainChar(sf::Vector2f& position) const;

	bool m_isCopyingMainCharColor = true;
};