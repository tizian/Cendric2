#pragma once

#include "global.h"
#include "GameObject.h"

#include "GUI/BitmapText.h"
#include "ResourceManager.h"
#include "Enums/SpellID.h"

enum class BuffType {
	Food,
	Spell,
	Debuff
};

class BuffSlot : public GameObject {
public:
	BuffSlot(BuffType type, const sf::IntRect& textureLocation, const sf::Time& duration, SpellID id);

	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;
	void setPosition(const sf::Vector2f& pos) override;

	GameObjectType getConfiguredType() const override;
	SpellID getSpellID() const;

	static const float RADIUS;
	static const float MARGIN;

private:
	sf::Time m_duration;
	SpellID m_spellID;

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