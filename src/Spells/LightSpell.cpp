#include "Spells/LightSpell.h"

LightSpell::LightSpell() : Spell() {
}

void LightSpell::load(const SpellBean& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	Spell::load(bean, mob, target);
	m_lightObject = new LightObject(LightBean(sf::Vector2f(), bean.range));
}

void LightSpell::setDisposed() {
	Spell::setDisposed();
	m_lightObject->setDisposed();
}

void LightSpell::setScreen(Screen* screen) {
	Spell::setScreen(screen);
	screen->addObject(m_lightObject);
}

void LightSpell::setPosition(const sf::Vector2f& pos) {
	Spell::setPosition(pos);
	if (m_lightObject != nullptr) m_lightObject->setPosition(pos + sf::Vector2f(0.f, -50.f));
}

