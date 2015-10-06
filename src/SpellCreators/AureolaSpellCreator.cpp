#include "SpellCreators/AureolaSpellCreator.h"
#include "Screens/LevelScreen.h"

AureolaSpellCreator::AureolaSpellCreator(const SpellBean &spellBean, LevelMovableGameObject *owner) : SpellCreator(spellBean, owner) {
}

void AureolaSpellCreator::executeSpell(const sf::Vector2f &target) {
	SpellBean spellBean = m_spellBean;
	updateDamage(spellBean);
	for (int i = 0; i < m_spellBean.count; i++) {
		AureolaSpell* newSpell = new AureolaSpell();
		spellBean.divergenceAngle = i * m_spellBean.divergenceAngle;
		newSpell->load(spellBean, m_owner, target);
		m_screen->addObject(newSpell);
	}

	m_owner->setFightAnimationTime();
}

void AureolaSpellCreator::addCountModifier(int level) {
	m_spellBean.count += m_spellBean.countModifierAddition * level;
	m_spellBean.divergenceAngle = 2 * M_PI / m_spellBean.count;
}