#include "SpellCreators/LeapOfFaithSpellCreator.h"
#include "Screens/LevelScreen.h"

LeapOfFaithSpellCreator::LeapOfFaithSpellCreator(const SpellData& spellData, LevelMovableGameObject *owner) : SpellCreator(spellData, owner) {
}

void LeapOfFaithSpellCreator::execExecuteSpell(const sf::Vector2f& target) {
	removeOldSpell();
	SpellData spellData = m_spellData;
	float gravityScale = 1.f / (1.f + 0.2f * m_spellData.strength);
	LeapOfFaithSpell* newSpell = new LeapOfFaithSpell(gravityScale);
	newSpell->load(spellData, m_owner, target);
	m_screen->addObject(newSpell);

	if (dynamic_cast<LevelMainCharacter*>(m_owner)) {
		m_screen->removeTypedBuffs(SpellID::LeapOfFaith);
		m_screen->addSpellBuffToInterface(spellData.iconTextureRect, spellData.duration, newSpell, AttributeData());
	}
}

void LeapOfFaithSpellCreator::addDurationModifier(int level) {
	m_spellData.activeDuration += static_cast<float>(level) * m_spellData.durationModifierAddition;
	m_spellData.duration = m_spellData.activeDuration;
	m_spellData.cooldown = m_spellData.activeDuration;
}

void LeapOfFaithSpellCreator::addStrengthModifier(int level) {
	m_spellData.strength += level;
}

void LeapOfFaithSpellCreator::removeOldSpell() {
	// check if there is already such a spell active for this owner and if yes, remove it
	std::vector<GameObject*>* spells = m_screen->getObjects(GameObjectType::_Spell);
	if (spells != nullptr) {
		for (auto& spell_ : *spells) {
			Spell* spell = dynamic_cast<Spell*>(spell_);
			if (spell == nullptr) continue;

			if (spell->getSpellID() == SpellID::LeapOfFaith && spell->getOwner() == m_owner) {
				spell->setDisposed();
			}
		}
	}
}

int LeapOfFaithSpellCreator::getStrengthModifierValue() const {
	return m_spellData.strength;
}

std::string LeapOfFaithSpellCreator::getStrengthModifierName() const {
	return "HolyPower";
}