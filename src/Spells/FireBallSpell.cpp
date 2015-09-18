#include "Spells/FireBallSpell.h"

FireBallSpell::FireBallSpell() : Spell(), 
LightObject(LightBean(sf::Vector2f(), 50.f))
{
}

void FireBallSpell::load(const SpellBean& bean, LevelMovableGameObject* mob, const sf::Vector2f& target)
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

	Spell::load(bean, mob, target);
}

void FireBallSpell::render(sf::RenderTarget& renderTarget)
{
	Spell::render(renderTarget);
	LightObject::render(renderTarget);
}

void FireBallSpell::update(const sf::Time& frameTime)
{
	Spell::update(frameTime);
	LightObject::update(frameTime);
}

void FireBallSpell::setPosition(const sf::Vector2f& pos)
{
	LightObject::setPosition(pos);
	Spell::setPosition(pos);
}

GameObjectType FireBallSpell::getConfiguredType() const
{
	return Spell::getConfiguredType();
}