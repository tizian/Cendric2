#include "Spells/BoomerangSpell.h"

void BoomerangSpell::load(const SpellData& data, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	init(data);
	Spell::load(data, mob, target);
}

void BoomerangSpell::init(const SpellData& data) {
	setSpriteOffset(sf::Vector2f(-19.f, 1.f));

	Animation* spellAnimation = new Animation();
	spellAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_spell_boomerang));
	spellAnimation->addFrame(sf::IntRect(0, data.skinNr * 9, 48, 9));

	addAnimation(GameObjectState::Idle, spellAnimation);

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(false);
}

