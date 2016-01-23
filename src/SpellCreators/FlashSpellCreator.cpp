#include "SpellCreators/FlashSpellCreator.h"
#include "Screens/LevelScreen.h"

FlashSpellCreator::FlashSpellCreator(const SpellData& spellData, LevelMovableGameObject* owner) : SpellCreator(spellData, owner) {
}

void FlashSpellCreator::executeSpell(const sf::Vector2f& target) {
	SpellData spellData = m_spellData;
	updateDamage(spellData);
	
	// check where port of owner is possible
	sf::FloatRect ownerBB = *(m_owner->getBoundingBox());
	float newRange = 0.f;
	while (!m_level->collides(ownerBB, true) && newRange <= spellData.range + 1) {
		newRange++;
		ownerBB.left = m_owner->isFacingRight() ? ownerBB.left + 1 : ownerBB.left - 1;
	}
	// check if we are inside a dynamic tile and move backwards if so. 
	// This is also used because we are one pixel inside the wall.
	while (m_level->collides(ownerBB)) {
		newRange--;
		ownerBB.left = m_owner->isFacingRight() ? ownerBB.left - 1 : ownerBB.left + 1;
	}
	// apply
	spellData.range = newRange;
	m_owner->setPosition(sf::Vector2f(ownerBB.left, ownerBB.top));
	FlashSpell* newSpell = new FlashSpell();
	newSpell->load(spellData, m_owner, target);
	m_screen->addObject(newSpell);
}