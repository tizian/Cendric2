#pragma once

#include "global.h"
#include "GameObject.h"

#include "ResourceManager.h"
#include "GUI/SpellSlot.h"

// class for the spell slot clone that spawns when a spell slot is dragged around.
class SpellSlotClone : public GameObject {
public:
	SpellSlotClone(const SpellSlot* original);

	void render(sf::RenderTarget& renderTarget) override;

	void setPosition(const sf::Vector2f& pos) override;

	GameObjectType getConfiguredType() const override;
	const SpellSlot* getOriginal() const;

private:
	void init();

	const SpellSlot* m_original;
	SpellType m_spellType;
	SpellID m_spellID;
	sf::IntRect m_textureRect;

	sf::Color m_color;
	sf::Color m_colorBase;

	sf::CircleShape m_outerRing;
	sf::CircleShape m_coloredRingBase;
	CircleSector 	m_coloredRing;
	sf::CircleShape m_innerRing;
	sf::CircleShape m_insideBase;
	sf::CircleShape m_inside;

	sf::CircleShape m_smallRingLeft1;
	sf::CircleShape m_smallRingLeft2;
	sf::CircleShape m_smallRingRight1;
	sf::CircleShape m_smallRingRight2;
	sf::CircleShape m_smallRingTop1;
	sf::CircleShape m_smallRingTop2;
	sf::CircleShape m_smallRingBottom1;
	sf::CircleShape m_smallRingBottom2;
};