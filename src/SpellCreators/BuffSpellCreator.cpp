#include "SpellCreators/BuffSpellCreator.h"
#include "Screens/LevelScreen.h"

BuffSpellCreator::BuffSpellCreator(const SpellData& spellData, LevelMovableGameObject* owner) : SpellCreator(spellData, owner) {
}

void BuffSpellCreator::execExecuteSpell(const sf::Vector2f& target) {
	SpellData spellData = m_spellData;
	updateDamageAndHeal(spellData);
	AttributeData damage = ZERO_ATTRIBUTES;
	damage.damageFire = spellData.damage;
	damage.damageIce = spellData.damage;
	damage.damageLight = spellData.damage;
	damage.damagePhysical = spellData.damage;
	damage.damageShadow = spellData.damage;
	BuffSpell* newSpell = new BuffSpell(damage);
	newSpell->load(spellData, m_owner, target);
	m_screen->addObject(newSpell);
	
	m_owner->addHeal(newSpell->getHeal(), false);
}
