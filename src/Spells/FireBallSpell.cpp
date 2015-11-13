#include "Spells/FireBallSpell.h"

FireBallSpell::FireBallSpell() : Spell() {
	m_lightObject = new LightObject(LightBean(sf::Vector2f(), 50.f));
}

void FireBallSpell::load(const SpellBean& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	setSpriteOffset(sf::Vector2f(-10.f, -10.f));

	Animation spellAnimation;
	spellAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_spell_fireball));
	spellAnimation.addFrame(sf::IntRect(0, 0, 30, 30));
	spellAnimation.addFrame(sf::IntRect(30, 0, 30, 30));

	addAnimation(GameObjectState::Idle, spellAnimation);

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);

	Spell::load(bean, mob, target);
	g_resourceManager->playSound(m_sound, ResourceID::Sound_spell_fireball);
}

void FireBallSpell::setDisposed() {
	Spell::setDisposed();
	m_lightObject->setDisposed();
	m_sound.stop();
}

void FireBallSpell::setScreen(Screen* screen) {
	Spell::setScreen(screen);
	screen->addObject(m_lightObject);
}

void FireBallSpell::setPosition(const sf::Vector2f& pos) {
	Spell::setPosition(pos);
	m_lightObject->setPosition(pos);
}
