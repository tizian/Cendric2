#include "Spells/ProjectileSpell.h"

void ProjectileSpell::load(const SpellData& data, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	init(data);
	Spell::load(data, mob, target);
}

void ProjectileSpell::load(const SpellData& data, LevelDynamicTile* tile, const sf::Vector2f& target) {
	init(data);
	Spell::load(data, tile, target);
}

void ProjectileSpell::init(const SpellData& data) {
	setSpriteOffset(sf::Vector2f(-35.f, -2.f));

	Animation* spellAnimation = new Animation();
	spellAnimation->setSpriteSheet(g_resourceManager->getTexture(data.spritesheetPath));
	spellAnimation->addFrame(sf::IntRect(0, data.skinNr * 15, 80, 15));

	addAnimation(GameObjectState::Idle, spellAnimation);

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(false);
}

