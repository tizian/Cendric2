#pragma once

#include "global.h"
#include "GameObject.h"
#include "ResourceManager.h"

class LevelMainCharacter;

// Equipment for cendric inside a level
class LevelEquipment : public GameObject
{
public:
	LevelEquipment() : GameObject() {}
	~LevelEquipment();

	void update(const sf::Time& frameTime) override;
	void loadEquipment(LevelMainCharacter* mainChar);

	void setTexturePath(const std::string& texturePath);

	GameObjectType getConfiguredType() const override;

private:
	LevelMainCharacter* m_mainChar;
	std::string m_texturePath;

	bool m_isFacingRight;

	// calculates position according to m_mainChar.
	void calculatePositionAccordingToMainChar(sf::Vector2f& position) const;
};