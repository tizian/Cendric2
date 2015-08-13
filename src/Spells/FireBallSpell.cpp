#include "Spells/FireBallSpell.h"

FireBallSpell::FireBallSpell() : Spell()
{
}

void FireBallSpell::load(const SpellBean& bean, LevelMovableGameObject* mob, const sf::Vector2f& target, float divergenceAngle)
{
	setSpriteOffset(sf::Vector2f(-10.f, -10.f));

	Animation spellAnimation;
	spellAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_spell_fireball));
	spellAnimation.addFrame(sf::IntRect(0, 0, 30, 30));
	spellAnimation.addFrame(sf::IntRect(30, 0, 30, 30));

	addAnimation(GameObjectState::Idle, spellAnimation);

	setFrameTime(sf::seconds(0.1f));

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);

	Spell::load(bean, mob, target, divergenceAngle);
}