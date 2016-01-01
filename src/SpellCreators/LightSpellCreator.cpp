#include "SpellCreators/LightSpellCreator.h"
#include "Screens/LevelScreen.h"

LightSpellCreator::LightSpellCreator(const SpellData &spellData, LevelMovableGameObject *owner) : SpellCreator(spellData, owner) {
}

void LightSpellCreator::executeSpell(const sf::Vector2f &target) {
	removeOldSpell();
	SpellData spellData = m_spellData;
	LightSpell* newSpell = new LightSpell();
	newSpell->load(spellData, m_owner, target);
	m_screen->addObject(newSpell);

	if (dynamic_cast<LevelMainCharacter*>(m_owner)) {
		m_screen->removeTypedBuffs(SpellID::Light);
		m_screen->addBuffToInterface(BuffType::Spell, spellData.iconTextureRect, spellData.duration, SpellID::Light);
	}
		
}

void LightSpellCreator::addDurationModifier(int level) {
	m_spellData.activeDuration += static_cast<float>(level) * m_spellData.durationModifierAddition;
	m_spellData.duration = m_spellData.activeDuration;
	m_spellData.cooldown = m_spellData.activeDuration;
}

void LightSpellCreator::removeOldSpell() {
	// check if there is already such a spell active for this owner and if yes, remove it
	std::vector<GameObject*>* spells = m_screen->getObjects(GameObjectType::_Spell);
	if (spells != nullptr) {
		for (auto& spell_ : *spells) {
			Spell* spell = dynamic_cast<Spell*>(spell_);
			if (spell == nullptr) continue;

			if (spell->getSpellID() == SpellID::Light && spell->getOwner() == m_owner) {
				spell->setDisposed();
			}
		}
	}
}