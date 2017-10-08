#include "SpellCreators/AntiGravitySpellCreator.h"
#include "Screens/LevelScreen.h"

AntiGravitySpellCreator::AntiGravitySpellCreator(const SpellData& spellData, LevelMovableGameObject* owner) : SpellCreator(spellData, owner) {
}

void AntiGravitySpellCreator::execExecuteSpell(const sf::Vector2f& target) {
	if (removeOldSpell()) {
		return;
	}
	SpellData spellData = m_spellData;
	AntiGravitySpell* newSpell = new AntiGravitySpell();
	newSpell->load(spellData, m_owner, target);
	m_screen->addObject(newSpell);
	if (dynamic_cast<LevelMainCharacter*>(m_owner)) {
		m_screen->addSpellBuffToInterface(spellData.iconTextureRect, spellData.duration, newSpell, AttributeData());
	}
}

bool AntiGravitySpellCreator::removeOldSpell() {
	// check if there is already such a spell active for this owner and if yes, remove it
	std::vector<GameObject*>* spells = m_screen->getObjects(GameObjectType::_Spell);
	if (spells != nullptr) {
		for (auto& spell_ : *spells) {
			Spell* spell = dynamic_cast<Spell*>(spell_);
			if (spell == nullptr) continue;

			if (spell->getSpellID() == SpellID::AntiGravity && spell->getOwner() == m_owner) {
				spell->setDisposed();
				if (dynamic_cast<LevelMainCharacter*>(m_owner)) {
					m_screen->removeTypedBuffs(SpellID::AntiGravity);
				}

				return true;
			}
		}
	}
	return false;
}