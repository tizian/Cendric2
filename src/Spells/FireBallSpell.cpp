#include "Spells/FireBallSpell.h"
#include "GameObjectComponents/LightComponent.h"

void FireBallSpell::load(const SpellData& data, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	setSpriteOffset(sf::Vector2f(-20.f, -20.f));

	Animation* spellAnimation = new Animation();
	spellAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_spell_fireball));
	for (int i = 0; i < 4; ++i) {
		spellAnimation->addFrame(sf::IntRect(i * 50, data.skinNr * 50, 50, 50));
	}

	addAnimation(GameObjectState::Idle, spellAnimation);

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);

	Spell::load(data, mob, target);
	LightData lightData(sf::Vector2f(), 80.f, 0.8f);
	addComponent(new LightComponent(lightData, this));
	g_resourceManager->playSound(m_sound, ResourceID::Sound_spell_fireball);
}

void FireBallSpell::setDisposed() {
	Spell::setDisposed();
	m_sound.stop();
}

