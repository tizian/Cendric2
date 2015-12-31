#pragma once

#include "global.h"
#include "Spell.h"

class FlashSpell : public Spell {
public:
	FlashSpell();
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	void execOnHit(LevelMovableGameObject* target) override;

private:
	bool m_isFlashingRight;
	
	sf::Sprite m_startingSprite;
	sf::Sprite m_endingSprite;

	sf::Time m_flashingTime;
	const sf::Time FLASHING_TIME = sf::milliseconds(100);
	bool m_isVisible = true;

	LevelMovableGameObject* m_mob;
};