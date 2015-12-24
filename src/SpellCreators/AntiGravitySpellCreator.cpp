#include "SpellCreators/AntiGravitySpellCreator.h"
#include "Screens/LevelScreen.h"

AntiGravitySpellCreator::AntiGravitySpellCreator(const SpellData& spellData, LevelMovableGameObject* owner) : SpellCreator(spellData, owner) {
}

void AntiGravitySpellCreator::executeSpell(const sf::Vector2f& target) {
	SpellData spellData= m_spellData;
	AntiGravitySpell* newSpell = new AntiGravitySpell();
	newSpell->load(spellData, m_owner, target);
	m_screen->addObject(newSpell);
	if (dynamic_cast<LevelMainCharacter*>(m_owner))
		m_screen->addBuffToInterface(BuffType::Spell, spellData.iconTextureRect, spellData.duration);
}