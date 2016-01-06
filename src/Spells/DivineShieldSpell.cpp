#include "Spells/DivineShieldSpell.h"

DivineShieldSpell::DivineShieldSpell(const AttributeData& additionalResistance) : Spell() {
	m_additionalResistance = additionalResistance;
	m_lightObject = new LightObject(LightData(sf::Vector2f(), sf::Vector2f(100.f, 150.f), 0.2f));
}

void DivineShieldSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	setSpriteOffset(sf::Vector2f(-12.f, -12.f));

	Animation spellAnimation;
	spellAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_spell_divineshield));

	for (int i = 0; i < 8; i++) {
		spellAnimation.addFrame(sf::IntRect(i * 120, 0, 120, 120));
	}

	for (int i = 7; i > -1; i--) {
		spellAnimation.addFrame(sf::IntRect(i * 120, 0, 120, 120));
	}

	addAnimation(GameObjectState::Idle, spellAnimation);

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);

	Spell::load(bean, mob, target);
	m_mob->addAttributes(m_duration, m_additionalResistance);
	m_mob->addHeal(getHeal());
}

void DivineShieldSpell::update(const sf::Time& frameTime) {
	calculatePositionAccordingToMob(m_nextPosition);
	setPosition(m_nextPosition);

	MovableGameObject::update(frameTime);

	m_duration = m_duration - frameTime;

	if (m_duration.asMilliseconds() <= 0) {
		setDisposed();
	}
}

sf::Vector2f DivineShieldSpell::getConfiguredPositionOffset() const {
	return sf::Vector2f(-49.f, 0.f);
}

bool DivineShieldSpell::getConfiguredRotateSprite() const {
	return false;
}

void DivineShieldSpell::execOnHit(LevelMovableGameObject* target) {
	// nop
}

void DivineShieldSpell::setDisposed() {
	Spell::setDisposed();
	m_lightObject->setDisposed();
}

void DivineShieldSpell::setScreen(Screen* screen) {
	Spell::setScreen(screen);
	screen->addObject(m_lightObject);
}

void DivineShieldSpell::setPosition(const sf::Vector2f& pos) {
	Spell::setPosition(pos);
	m_lightObject->setPosition(pos + sf::Vector2f(getBoundingBox()->width / 2.f, getBoundingBox()->height / 2.f));
}