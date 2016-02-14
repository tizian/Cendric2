#include "SpellCreators/ShadowTrapSpellCreator.h"
#include "Screens/LevelScreen.h"

ShadowTrapSpellCreator::ShadowTrapSpellCreator(const SpellData& spellData, LevelMovableGameObject *owner) : SpellCreator(spellData, owner) {
}

std::string ShadowTrapSpellCreator::getStrengthModifierName() const {
	return "StunLevel";
}

void ShadowTrapSpellCreator::executeSpell(const sf::Vector2f &target) {
	SpellData spellData = m_spellData;
	updateDamageAndHeal(spellData);

	ShadowTrapSpell* newSpell = new ShadowTrapSpell();

	newSpell->load(spellData, m_owner, target);
	m_screen->addObject(newSpell);
}

void ShadowTrapSpellCreator::addDurationModifier(int level) {
	m_spellData.duration += static_cast<float>(level) * m_spellData.durationModifierAddition;
}
