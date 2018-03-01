#include "SpellCreators/ShadowTrapSpellCreator.h"
#include "Screens/LevelScreen.h"

ShadowTrapSpellCreator::ShadowTrapSpellCreator(const SpellData& spellData, LevelMovableGameObject *owner) : SpellCreator(spellData, owner) {
}

std::string ShadowTrapSpellCreator::getStrengthModifierName() const {
	return "StunLevel";
}

void ShadowTrapSpellCreator::execExecuteSpell(const sf::Vector2f& target) {
	SpellData spellData = m_spellData;
	updateDamageAndHeal(spellData);

	ShadowTrapSpell* newSpell = new ShadowTrapSpell();

	newSpell->load(spellData, m_owner, target);
	m_screen->addObject(newSpell);
}

void ShadowTrapSpellCreator::addDurationModifier(int level) {
	m_spellData.duration += static_cast<float>(level) * m_spellData.durationModifierAddition;
}

void ShadowTrapSpellCreator::addCountModifier(int level) {
	switch (level)
	{
	case 1:
		m_spellData.cooldown = sf::seconds(8.f);
		break;
	case 2:
		m_spellData.cooldown = sf::seconds(6.f);
		break;
	case 3:
		m_spellData.cooldown = sf::seconds(4.f);
		break;
	default:
		m_spellData.cooldown = sf::seconds(10.f);
		break;
	}
}
