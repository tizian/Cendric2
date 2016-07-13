#pragma once

#include "global.h"
#include "AnimatedGameObject.h"
#include "ResourceManager.h"

class MapMainCharacter;
class LightComponent;

// Equipment for cendric on the map
class MapEquipment : public AnimatedGameObject {
public:
	MapEquipment(MapMainCharacter* mainChar);
	~MapEquipment() {};

	void update(const sf::Time& frameTime) override;

	GameObjectType getConfiguredType() const override;
	
private:
	MapMainCharacter* m_mainChar;
};