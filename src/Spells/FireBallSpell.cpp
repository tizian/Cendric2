#include "Spells/FireBallSpell.h"

FireBallSpell::FireBallSpell() : Spell() {
	m_lightObject = new LightObject(LightData(sf::Vector2f(), 80.f, 0.8f));
}

void FireBallSpell::load(const SpellData& data, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	setSpriteOffset(sf::Vector2f(-20.f, -20.f));

	Animation* spellAnimation = new Animation();
	spellAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_spell_fireball));
	for (int i = 0; i < 4; ++i) {
		spellAnimation->addFrame(sf::IntRect(i * 50, data.skinNr * 50, 50, 50));
	}

	addAnimation(GameObjectState::Idle, spellAnimation);

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);

	Spell::load(data, mob, target);
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
