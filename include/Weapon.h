#pragma once

#include "global.h"
#include "GameObject.h"
#include "ResourceManager.h"

class MainCharacter;

// A weapon cendric can equip
class Weapon : public GameObject
{
public:
	virtual void update(sf::Time& frameTime) override;
	void loadWeapon(MainCharacter* mainChar);

	// the offset of the spells start position, as seen from the upper mid of cendrics bounding box. The default is the position of the staff head
	virtual sf::Vector2f getConfiguredPositionOffset();

protected:
	bool m_isFacingRight;

private:
	MainCharacter* m_mainChar;

	// calculates position according to m_mainChar.
	void calculatePositionAccordingToMainChar(sf::Vector2f& position);
};