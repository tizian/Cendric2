#include "SpellCreators/ShackleSpellCreator.h"
#include "Screens/LevelScreen.h"

ShackleSpellCreator::ShackleSpellCreator(const SpellData &spellData, LevelMovableGameObject *owner) : SpellCreator(spellData, owner) {
}

int ShackleSpellCreator::getStrengthModifierValue() const {
	return m_strength;
}

std::string ShackleSpellCreator::getStrengthModifierName() const {
	return "ShacklePower";
}

void ShackleSpellCreator::executeSpell(const sf::Vector2f &target) {
	SpellData spellData = m_spellData;
	updateDamage(spellData);
	int div = 0;
	int sign = 1;
	for (int i = 0; i < m_spellData.count; i++) {
		ShackleSpell* newSpell = new ShackleSpell(m_spellData.duration, m_strength);
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

void ShackleSpellCreator::addStrengthModifier(int level) {
	m_strength += level;
}