#include "Spells/ShackleSpell.h"

ShackleSpell::ShackleSpell(const sf::Time& stunDuration, int strength) : Spell() {
	m_stunDuration = stunDuration;
	m_strength = strength;
}

void ShackleSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	setSpriteOffset(sf::Vector2f(-10.f, -10.f));

	Animation spellAnimation;
	spellAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_spell_shackle));
	spellAnimation.addFrame(sf::IntRect(0, 0, 30, 30));
	spellAnimation.addFrame(sf::IntRect(30, 0, 30, 30));

	addAnimation(GameObjectState::Idle, spellAnimation);

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);

	Spell::load(bean, mob, target);
}

void ShackleSpell::execOnHit(LevelMovableGameObject *target) {
	if (Enemy* enemy = dynamic_cast<Enemy*>(target)) {
		// TODO: set DOT
		if (enemy->getMentalStrength() <= m_strength) {
			enemy->setStunned(m_stunDuration);
			setDisposed();
		}
	}
	// main character can't be stunned yet.
}