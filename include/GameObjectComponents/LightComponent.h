#pragma once

#include "global.h"
#include "GameObjectComponents/GameObjectComponent.h"
#include "World/LightObject.h"

// A game object component that holds a light object.
class LightComponent final : public GameObjectComponent {
public:
	LightComponent(LightData lightData, GameObject* parent);
	~LightComponent();

	void flipOffsetX(bool flipped);
	void flipOffsetY(bool flipped);
	void setVisible(bool visible);
	void setPosition(const sf::Vector2f& pos) override;

protected:
	LightObject* m_lightObject = nullptr;
	sf::Vector2f m_offset;
	bool m_isOffsetFlippedX = false;
	bool m_isOffsetFlippedY = false;
};