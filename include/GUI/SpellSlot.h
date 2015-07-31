#pragma once

#include "global.h"
#include "GameObject.h"

#include "GUI/BitmapText.h"
#include "GUI/CircleSector.h"
#include "Structs/SpellBean.h"
#include "ResourceManager.h"

class SpellSlot : public GameObject
{
public:
	SpellSlot(const sf::Vector2f &center = sf::Vector2f(0.f, 0.f), const SpellColor &spellColor = SpellColor::VOID, bool active = true);

	void activate();
	void deactivate();

	void playAnimation(const sf::Time &cooldown);

	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;

	GameObjectType getConfiguredType() const override;

	static const float RADIUS;

private:
	sf::Texture *m_textureInactive = nullptr;
	sf::Texture *m_texture = nullptr;

	bool m_active = false;

	sf::Time m_cooldown;
	SpellColor m_spellColor;

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

	bool m_animating = false;
	sf::Time m_animationTime;
};