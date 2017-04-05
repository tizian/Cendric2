#pragma once

#include "global.h"
#include "Spells/Spell.h"

class ParticleComponent;

class FlashSpell : public Spell {
public:
	FlashSpell();

	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	void execOnHit(LevelMovableGameObject* target) override;
	bool getConfiguredRotateSprite() const override;

private:
	bool m_isFlashingRight;

	sf::Time m_flashingTime;
	const sf::Time FLASHING_TIME = sf::milliseconds(100);
	bool m_isVisible = true;
	bool m_isHurting = true;

	sf::Time m_flashDuration;
	const sf::Time FLASH_DURATION = sf::milliseconds(500);

	void loadComponents();
	ParticleComponent* m_pc;
	LevelMovableGameObject* m_mob;
};