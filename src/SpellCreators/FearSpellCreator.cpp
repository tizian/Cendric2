#include "SpellCreators/FearSpellCreator.h"
#include "Screens/LevelScreen.h"

FearSpellCreator::FearSpellCreator(const SpellData &spellData, LevelMovableGameObject *owner) : SpellCreator(spellData, owner) {
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
		FearSpell* newSpell = new FearSpell();
		spellData.divergenceAngle = div * sign * m_spellData.divergenceAngle;
		newSpell->load(spellData, m_owner, target);
		m_screen->addObject(newSpell);
		sign = -sign;
		if (sign == -1) {
			div += 1;
		}
	}

	m_owner->setFightAnimationTime();
}

void FearSpellCreator::addDurationModifier(int level) {
	m_spellData.duration += static_cast<float>(level) * m_spellData.durationModifierAddition;
}