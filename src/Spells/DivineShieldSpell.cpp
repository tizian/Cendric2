#include "Spells/DivineShieldSpell.h"

DivineShieldSpell::DivineShieldSpell() : Spell()
{
}

void DivineShieldSpell::load(const SpellBean& bean, LevelMovableGameObject* mob, sf::Vector2f target, float divergenceAngle)
{
	setSpriteOffset(sf::Vector2f(-12.f, -12.f));

	Animation spellAnimation;
	spellAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_spell_divineshield));
	
	for (int i = 0; i < 8; i++)
	{
		spellAnimation.addFrame(sf::IntRect(i * 120, 0, 120, 120));
	}

	for (int i = 7; i > -1; i--)
	{
		spellAnimation.addFrame(sf::IntRect(i * 120, 0, 120, 120));
	}

	addAnimation(GameObjectState::Idle, spellAnimation);

	setFrameTime(sf::seconds(0.1f));

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);

	Spell::load(bean, mob, target, 0);
}

const sf::Vector2f DivineShieldSpell::getConfiguredPositionOffset() const
{
	return sf::Vector2f(-49.f, 0.f);
}

bool DivineShieldSpell::getConfiguredIsAttachedToMob() const
{
	return true;
}

bool DivineShieldSpell::getConfiguredRotateSprite() const
{
	return false;
}