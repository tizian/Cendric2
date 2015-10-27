#include "SpellCreators/LightSpellCreator.h"
#include "Screens/LevelScreen.h"

LightSpellCreator::LightSpellCreator(const SpellBean &spellBean, LevelMovableGameObject *owner) : SpellCreator(spellBean, owner) {
}

void LightSpellCreator::executeSpell(const sf::Vector2f &target) {
	SpellBean spellBean = m_spellBean;
	LightSpell* newSpell = new LightSpell();
	newSpell->load(spellBean, m_owner, target);
	m_screen->addObject(newSpell);
}