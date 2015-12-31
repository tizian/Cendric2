#include "SpellCreators/FlashSpellCreator.h"
#include "Screens/LevelScreen.h"

FlashSpellCreator::FlashSpellCreator(const SpellData& spellData, LevelMovableGameObject* owner) : SpellCreator(spellData, owner) {
}

void FlashSpellCreator::executeSpell(const sf::Vector2f& target) {
	SpellData spellData = m_spellData;
	updateDamage(spellData);
	
	// check if port of owner is possible
	sf::FloatRect ownerBB = *(m_owner->getBoundingBox());
	ownerBB.left = m_owner->getIsFacingRight() ? ownerBB.left + spellData.range : ownerBB.left - spellData.range;
	if (!m_level->collidesX(ownerBB) && !m_level->collidesY(ownerBB)) {
		m_owner->setPosition(sf::Vector2f(ownerBB.left, ownerBB.top));
		FlashSpell* newSpell = new FlashSpell();
		newSpell->load(spellData, m_owner, target);
		m_screen->addObject(newSpell);
	}
	else {
		g_logger->logInfo("FlashSpellCreator", "Flash port would stuck its owner. The port is not executed.");
	}
}

void FlashSpellCreator::addRangeModifier(int level) {
	m_spellData.range += m_spellData.rangeModifierAddition * level;
	m_spellData.boundingBox.width = m_spellData.range;
}