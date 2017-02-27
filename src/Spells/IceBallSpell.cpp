#include "Spells/IceBallSpell.h"
#include "GameObjectComponents/LightComponent.h"

void IceBallSpell::load(const SpellData& data, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	setSpriteOffset(sf::Vector2f(-10.f, -10.f));
	setBoundingBox(sf::FloatRect(0, 0, 10, 10));
	int size = 30;

	Animation* spellAnimation = new Animation();
	spellAnimation->setSpriteSheet(g_resourceManager->getTexture(data.spritesheetPath));
	spellAnimation->addFrame(sf::IntRect(0, data.skinNr * size, size, size));
	spellAnimation->addFrame(sf::IntRect(size, data.skinNr * size, size, size));

	addAnimation(GameObjectState::Idle, spellAnimation);

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);

	Spell::load(data, mob, target);

	LightData lightData(sf::Vector2f(m_boundingBox.width / 2.f, m_boundingBox.height / 2.f), 80.f, 0.4f);
	addComponent(new LightComponent(lightData, this));
}

