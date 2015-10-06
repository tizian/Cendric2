#pragma once

#include "global.h"
#include "GameObject.h"

#include "GUI/BitmapText.h"
#include "ResourceManager.h"

enum class BuffType {
	Food,
	Spell
};

class BuffSlot : public GameObject {
public:
	BuffSlot(BuffType type, const sf::IntRect& textureLocation, const sf::Time& duration);

	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;
	void setPosition(const sf::Vector2f& pos) override;

	GameObjectType getConfiguredType() const override;

	static const float RADIUS;
	static const float MARGIN;

private:
	sf::Time m_duration;

	sf::CircleShape m_outerRing;
	sf::CircleShape m_innerRing;
	sf::CircleShape m_inside;

	BitmapText m_durationText;

	// used for flashing
	static const sf::Time FLASHING_TIME;
	static const sf::Time FLASHING_INTERVAL;
	bool m_isVisible;
	sf::Time m_timeUntilFlash;
};