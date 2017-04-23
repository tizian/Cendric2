#pragma once

#include "global.h"
#include "AnimatedGameObject.h"
#include "ResourceManager.h"
#include "Structs/LightData.h"

class MapMainCharacter;
class LightComponent;

// Equipment for cendric on the map
class MapEquipment final : public AnimatedGameObject {
public:
	MapEquipment(MapMainCharacter* mainChar);

	void update(const sf::Time& frameTime) override;

	GameObjectType getConfiguredType() const override;

	void setLightComponent(const LightData& lightData);
	void setHasTexture();

private:
	MapMainCharacter* m_mainChar;
	bool m_hasTexture = false;

	LightComponent* m_lightComponent = nullptr;
};