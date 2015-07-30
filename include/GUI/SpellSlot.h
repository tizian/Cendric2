#pragma once

#include "global.h"
#include "GameObject.h"

#include "GUI/BitmapText.h"
#include "GUI/CircleSector.h"
#include "Structs/SpellBean.h"

class SpellSlot : public GameObject
{
public:
	SpellSlot(const SpellBean *spell, const sf::Vector2f &center);

	void playAnimation();

	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;

	GameObjectType getConfiguredType() const override;

private:
	const SpellBean *m_spell;

	sf::Color m_color;
	sf::Color m_colorBase;

	sf::CircleShape m_outerRing;
	sf::CircleShape m_coloredRingBase;
	CircleSector 	m_coloredRing;
	sf::CircleShape m_innerRing;
	sf::CircleShape m_inside;

	sf::CircleShape m_smallRingLeft1;
	sf::CircleShape m_smallRingLeft2;
	sf::CircleShape m_smallRingRight1;
	sf::CircleShape m_smallRingRight2;
	sf::CircleShape m_smallRingTop1;
	sf::CircleShape m_smallRingTop2;
	sf::CircleShape m_smallRingBottom1;
	sf::CircleShape m_smallRingBottom2;

	bool m_animating;
	sf::Time m_animationTime;
};