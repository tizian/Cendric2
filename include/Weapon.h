#pragma once

#include "global.h"
#include "GameObject.h"
#include "ResourceManager.h"

class MainCharacter;

// A weapon cendric can equip
class Weapon : public GameObject
{
public:
	virtual void update(const sf::Time& frameTime) override;
	void loadWeapon(MainCharacter* mainChar);

	// the offset of the weapon, as seen from the upper mid of cendrics bounding box. The default is the position of the staff head
	virtual const sf::Vector2f getConfiguredPositionOffset() const;
	GameObjectType getConfiguredType() const override;

protected:
	bool m_isFacingRight;

private:
	MainCharacter* m_mainChar;

	// calculates position according to m_mainChar.
	void calculatePositionAccordingToMainChar(sf::Vector2f& position) const;
};