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
	void loadEquipment();
	
	void setTexturePath(const std::string& texturePath);

	GameObjectType getConfiguredType() const override;

	void setPosition(const sf::Vector2f& position) override;

private:
	MapMainCharacter* m_mainChar;
	std::string m_texturePath;
};