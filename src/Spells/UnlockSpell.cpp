#include "Spells/UnlockSpell.h"

UnlockSpell::UnlockSpell(int strength) : Spell() {
	m_strength = strength;
}

void UnlockSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	setSpriteOffset(sf::Vector2f(-10.f, 0.f));

	Animation* spellAnimation = new Animation(sf::seconds(10.f));
	spellAnimation->setSpriteSheet(g_resourceManager->getTexture(bean.spritesheetPath));
	spellAnimation->addFrame(sf::IntRect(0, 0, 30, 10));

	addAnimation(GameObjectState::Idle, spellAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(false);

	Spell::load(bean, mob, target);
}

int UnlockSpell::getStrength() const {
	return m_strength;
}