#include "Spells/FlashSpell.h"

FlashSpell::FlashSpell() : Spell() {
}

void FlashSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	Spell::load(bean, mob, target);
	m_isFlashingRight = mob->getIsFacingRight();
	m_mob = mob;
	m_flashingTime = FLASHING_TIME;

	sf::Vector2f position(m_mob->getPosition());
	position.x += (-60 + m_mob->getBoundingBox()->width / 2.f);
	position.y += (-60 + m_mob->getBoundingBox()->height / 2.f);

	m_startingSprite.setTexture(*g_resourceManager->getTexture(ResourceID::Texture_spell_flash));
	m_startingSprite.setTextureRect(sf::IntRect(0, 0, 120, 120));
	m_startingSprite.setPosition(position);

	position.x = m_isFlashingRight ? position.x + bean.range : position.x - bean.range;

	m_endingSprite.setTexture(*g_resourceManager->getTexture(ResourceID::Texture_spell_flash));
	m_endingSprite.setTextureRect(sf::IntRect(120, 0, 120, 120));
	m_endingSprite.setPosition(position);
}

void FlashSpell::update(const sf::Time& frameTime) {
	Spell::update(frameTime);
	GameObject::updateTime(m_flashingTime, frameTime);
	if (m_flashingTime == sf::Time::Zero) {
		m_isVisible = !m_isVisible;
		m_flashingTime = FLASHING_TIME;
	}
}

void FlashSpell::render(sf::RenderTarget& target) {
	if (m_isVisible) {
		target.draw(m_startingSprite);
		target.draw(m_endingSprite);
	}
}

void FlashSpell::execOnHit(LevelMovableGameObject* target) {
	// this spell only hurts once
	m_damageType = DamageType::VOID;
}
