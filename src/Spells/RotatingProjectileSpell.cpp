#include "Spells/RotatingProjectileSpell.h"

void RotatingProjectileSpell::load(const SpellData& data, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	init(data);
	Spell::load(data, mob, target);
}

void RotatingProjectileSpell::load(const SpellData& data, LevelDynamicTile* tile, const sf::Vector2f& target) {
	init(data);
	Spell::load(data, tile, target);
}

void RotatingProjectileSpell::update(const sf::Time& frameTime) {
	Spell::update(frameTime);

	m_angle += 3.f * frameTime.asSeconds();
	setSpriteRotation(m_angle);
}

bool RotatingProjectileSpell::getConfiguredRotateSprite() const {
	return false;
}

void RotatingProjectileSpell::init(const SpellData& data) {
	setSpriteOffset(sf::Vector2f(0.f, 0.f));

	Animation* spellAnimation = new Animation();
	spellAnimation->setSpriteSheet(g_resourceManager->getTexture(data.spritesheetPath));
	spellAnimation->addFrame(sf::IntRect(0, data.skinNr * 45, 45, 45));

	addAnimation(GameObjectState::Idle, spellAnimation);

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(false);
}

