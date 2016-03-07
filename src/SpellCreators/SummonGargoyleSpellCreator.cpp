#include "SpellCreators/SummonGargoyleSpellCreator.h"
#include "Screens/LevelScreen.h"
#include "Level/Enemies/GargoyleEnemy.h"

SummonGargoyleSpellCreator::SummonGargoyleSpellCreator(const SpellData &spellData, LevelMovableGameObject *owner) : SpellCreator(spellData, owner) {
}

std::string SummonGargoyleSpellCreator::getStrengthModifierName() const {
	return "GargoyleStrength";
}

void SummonGargoyleSpellCreator::executeSpell(const sf::Vector2f &target) {
	// dispose old summoned gargoyles. Doesn't check for owner though, but cendric is the only one that can have allied enemies.
	auto enemies = m_screen->getObjects(GameObjectType::_Enemy);
	for (auto& go : *enemies) {
		if (GargoyleEnemy* enemy = dynamic_cast<GargoyleEnemy*>(go)) {
			if (enemy->isSummoned()) {
				enemy->setDisposed();
			}
		}
	}

	GargoyleEnemy* gargoyle = new GargoyleEnemy(m_level, m_screen);
	gargoyle->setSummoned(m_spellData.strength, m_spellData.damage, m_spellData.count, m_spellData.duration);
	gargoyle->setPosition(m_owner->getPosition());
	m_screen->addObject(gargoyle);
}

void SummonGargoyleSpellCreator::addDurationModifier(int level) {
	m_spellData.duration += static_cast<float>(level) * m_spellData.durationModifierAddition;
}
