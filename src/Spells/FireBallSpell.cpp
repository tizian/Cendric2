#include "Spells/FireBallSpell.h"
#include "GameObjectComponents/LightComponent.h"

void FireBallSpell::init(const SpellData& data) {
	setSpriteOffset(sf::Vector2f(-20.f, -20.f));
	const sf::Texture* tex = g_resourceManager->getTexture(data.spritesheetPath);

	Animation* spellAnimation = new Animation();
	spellAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 4; ++i) {
		spellAnimation->addFrame(sf::IntRect(i * 50, data.skinNr * 50, 50, 50));
	}

	addAnimation(GameObjectState::Idle, spellAnimation);

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);
}

void FireBallSpell::load(const SpellData& data, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	init(data);
	Spell::load(data, mob, target);

	LightData lightData(sf::Vector2f(m_boundingBox.width / 2.f, m_boundingBox.height / 2.f), 80.f, 0.8f);
	addComponent(new LightComponent(lightData, this));
	g_resourceManager->playSound(data.soundPath);
}

void FireBallSpell::load(const SpellData& data, LevelDynamicTile* tile, const sf::Vector2f& target) {
	init(data);
	Spell::load(data, tile, target);

	LightData lightData(sf::Vector2f(m_boundingBox.width / 2.f, m_boundingBox.height / 2.f), 80.f, 0.8f);
	addComponent(new LightComponent(lightData, this));
}
