#include "SpellCreators/FlashSpellCreator.h"
#include "Screens/LevelScreen.h"

FlashSpellCreator::FlashSpellCreator(const SpellData& spellData, LevelMovableGameObject* owner) : SpellCreator(spellData, owner) {
}

void FlashSpellCreator::executeSpell(const sf::Vector2f& target) {
	SpellData spellData = m_spellData;
	updateDamage(spellData);
	
	// check where port of owner is possible
	WorldCollisionQueryRecord rec;
	rec.boundingBox = *(m_owner->getBoundingBox());
	rec.ignoreDynamicTiles = true;
	float newRange = 0.f;
	while (!m_level->collides(rec) && newRange <= spellData.range + 1) {
		newRange++;
		rec.boundingBox.left = m_owner->isFacingRight() ? 
			rec.boundingBox.left + 1 : 
			rec.boundingBox.left - 1;
	}
	// check if we are inside a dynamic tile and move backwards if so. 
	// This is also used because we are one pixel inside the wall.
	while (m_level->collides(rec)) {
		newRange--;
		rec.boundingBox.left = m_owner->isFacingRight() ? 
			rec.boundingBox.left - 1 : 
			rec.boundingBox.left + 1;
	}
	// apply
	spellData.range = newRange;
	m_owner->setPosition(sf::Vector2f(rec.boundingBox.left, rec.boundingBox.top));
	FlashSpell* newSpell = new FlashSpell();
	newSpell->load(spellData, m_owner, target);
	m_screen->addObject(newSpell);
}