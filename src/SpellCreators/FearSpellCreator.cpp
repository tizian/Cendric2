#include "SpellCreators/FearSpellCreator.h"
#include "Screens/LevelScreen.h"

FearSpellCreator::FearSpellCreator(const SpellData &spellData, LevelMovableGameObject *owner) : SpellCreator(spellData, owner) {
}

int FearSpellCreator::getStrengthModifierValue() const {
	return m_strength;
}

std::string FearSpellCreator::getStrengthModifierName() const {
	return "FearLevel";
}

void FearSpellCreator::executeSpell(const sf::Vector2f &target) {
	SpellData spellData= m_spellData;
	updateDamage(spellData);
	int div = 0;
	int sign = 1;
	for (int i = 0; i < m_spellData.count; i++) {
		FearSpell* newSpell = new FearSpell(m_spellData.duration, m_strength);
		spellData.divergenceAngle = div * sign * m_spellData.divergenceAngle;
		spellData.duration = m_activeDuration;
		newSpell->load(spellData, m_owner, target);
		m_screen->addObject(newSpell);
		sign = -sign;
		if (sign == -1) {
			div += 1;
		}
	}

	m_owner->setFightAnimationTime();
}

void FearSpellCreator::addStrengthModifier(int level) {
	m_strength += level;
}