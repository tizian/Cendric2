#include "SpellCreators/AureolaSpellCreator.h"
#include "Screens/LevelScreen.h"

AureolaSpellCreator::AureolaSpellCreator(const SpellData& spellData, LevelMovableGameObject *owner) : SpellCreator(spellData, owner) {
}

std::string AureolaSpellCreator::getStrengthModifierName() const {
	return "StunLevel";
}

void AureolaSpellCreator::execExecuteSpell(const sf::Vector2f& target) {
	SpellData spellData = m_spellData;
	updateDamageAndHeal(spellData);
	for (int i = 0; i < m_spellData.count; i++) {
		AureolaSpell* newSpell = new AureolaSpell();
		spellData.divergenceAngle = i * m_spellData.divergenceAngle;
		newSpell->load(spellData, m_owner, target);
		m_screen->addObject(newSpell);
	}
}

void AureolaSpellCreator::addCountModifier(int level) {
	m_spellData.count += m_spellData.countModifierAddition * level;
	m_spellData.divergenceAngle = 2 * M_PI / m_spellData.count;
}

void AureolaSpellCreator::addDurationModifier(int level) {
	m_spellData.duration += static_cast<float>(level) * m_spellData.durationModifierAddition;
}
