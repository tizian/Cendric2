#include "SpellCreators/TelekinesisSpellCreator.h"
#include "Screens/LevelScreen.h"

TelekinesisSpellCreator::TelekinesisSpellCreator(const SpellData& spellData, LevelMovableGameObject* owner) : SpellCreator(spellData, owner) {
}

void TelekinesisSpellCreator::executeSpell(const sf::Vector2f& target) {
	SpellData spellData = m_spellData;
	updateDamage(spellData);

	TelekinesisSpell* newSpell = new TelekinesisSpell();
	newSpell->load(spellData, m_owner, target);
	newSpell->setItemVector(m_screen->getObjects(GameObjectType::_LevelItem));
	m_screen->addObject(newSpell);

	m_owner->setFightAnimationTime();
}

void TelekinesisSpellCreator::addRangeModifier(int level) {
	SpellCreator::addRangeModifier(level);
	m_spellData.activeDuration = sf::seconds(m_spellData.range / m_spellData.startVelocity);
}