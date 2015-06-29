#pragma once

#include "global.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Enums/LevelEquipmentID.h"

class LevelMainCharacter;

// Equipment for cendric inside a level
class LevelEquipment : public GameObject
{
public:
	LevelEquipment() : GameObject() {}
	~LevelEquipment();

	void update(const sf::Time& frameTime) override;
	void loadEquipment(LevelMainCharacter* mainChar, LevelEquipmentID id);

	void setTextureID(ResourceID id);

	// this is only used by weapons: the offset of the weapon, as seen from the upper mid of cendrics bounding box. The default is the position of the staff head
	virtual const sf::Vector2f getConfiguredPositionOffset() const;
	GameObjectType getConfiguredType() const override;

protected:
	bool m_isFacingRight;

private:
	LevelMainCharacter* m_mainChar;
	LevelEquipmentID m_equipmentID;
	ResourceID m_resourceID;

	// calculates position according to m_mainChar.
	void calculatePositionAccordingToMainChar(sf::Vector2f& position) const;
};