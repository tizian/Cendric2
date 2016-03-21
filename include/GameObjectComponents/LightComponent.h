#pragma once

#include "global.h"
#include "GameObjectComponents/GameObjectComponent.h"
#include "LightObject.h"

// A game object component that holds a light object.
class LightComponent : public GameObjectComponent {
public:
	LightComponent(LightData lightData, GameObject* parent);
	~LightComponent();

	void flipLightOffsetX(bool flipped);
	void setVisible(bool visible);
	void setPosition(const sf::Vector2f& pos) override;

protected:
	LightObject* m_lightObject = nullptr;
	sf::Vector2f m_lightOffset;
	bool m_isLightOffsetFlippedX = false;
};