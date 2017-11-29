#include "Spells/ShackleSpell.h"
#include "GlobalResource.h"
#include "Level/LevelMainCharacter.h"
#include "GameObjectComponents/LightComponent.h"
#include "GameObjectComponents/ParticleComponent.h"
#include "World/CustomParticleUpdaters.h"

ShackleSpell::ShackleSpell() : Spell() {
}

void ShackleSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	Spell::load(bean, mob, target);
	loadComponents();
}

void ShackleSpell::update(const sf::Time& frameTime) {
	Spell::update(frameTime);
}

bool ShackleSpell::getConfiguredRotateSprite() const {
	return false;
}

void ShackleSpell::loadComponents() {
	// light
	LightData lightData(sf::Vector2f(m_boundingBox.height * 0.5f, m_boundingBox.height * 0.5f), 1000.f, 1.f);
	auto lc = new LightComponent(lightData, this);
	addComponent(lc);
}
