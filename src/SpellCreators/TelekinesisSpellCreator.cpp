#include "SpellCreators/TelekinesisSpellCreator.h"
#include "Screens/LevelScreen.h"

TelekinesisSpellCreator::TelekinesisSpellCreator(const SpellBean& spellBean, LevelMovableGameObject* owner) : SpellCreator(spellBean, owner) {
}

void TelekinesisSpellCreator::executeSpell(const sf::Vector2f& target) {
	SpellBean spellBean = m_spellBean;
	updateDamage(spellBean);

	TelekinesisSpell* newSpell = new TelekinesisSpell();
	newSpell->load(spellBean, m_owner, target);
	m_screen->addObject(newSpell);

	m_owner->setFightAnimationTime();
}

void TelekinesisSpellCreator::addRangeModifier(int level) {
	SpellCreator::addRangeModifier(level);
	m_spellBean.duration = sf::seconds(m_spellBean.range / m_spellBean.startVelocity);
}