#include "SpellCreators/IcyAmbushSpellCreator.h"
#include "Screens/LevelScreen.h"

IcyAmbushSpellCreator::IcyAmbushSpellCreator(const SpellData& spellData, LevelMovableGameObject* owner) : SpellCreator(spellData, owner) {
}

void IcyAmbushSpellCreator::executeSpell(const sf::Vector2f& target) {
	SpellData spellData = m_spellData;
	updateDamage(spellData);

	IcyAmbushSpell* newSpell = new IcyAmbushSpell();
	newSpell->load(spellData, m_owner, target);
	m_screen->addObject(newSpell);

	m_owner->setFightAnimationTime();
}

void IcyAmbushSpellCreator::addDurationModifier(int level) {
	m_spellData.duration += static_cast<float>(level)* m_spellData.durationModifierAddition;
}