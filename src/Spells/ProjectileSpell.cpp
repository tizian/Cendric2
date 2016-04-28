#include "Spells/ProjectileSpell.h"
#include "Screens/LevelScreen.h"

ProjectileSpell::ProjectileSpell() : Spell() {
}

void ProjectileSpell::load(const SpellData& data, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	init(data);
	Spell::load(data, mob, target);
}

void ProjectileSpell::load(const SpellData& data, LevelDynamicTile* tile, const sf::Vector2f& target) {
	init(data);
	
	// very similar to Spell::load with a mob, but its using a LevelDynamicTile instead
	m_data = data;
	setBoundingBox(data.boundingBox);
	setDebugBoundingBox(COLOR_BAD);

	m_level = tile->getLevel();
	m_mob = nullptr;
	m_screen = tile->getScreen();
	m_enemies = m_screen->getObjects(GameObjectType::_Enemy);

	m_mainChar = dynamic_cast<LevelScreen*>(m_screen)->getMainCharacter();

	sf::Vector2f absolutePosition = tile->getCenter();
	setPosition(absolutePosition);

	sf::Vector2f trueDir = target - absolutePosition;
	// normalize dir
	float len = sqrt(trueDir.x * trueDir.x + trueDir.y * trueDir.y);
	trueDir.x = (len == 0) ? 0 : trueDir.x / len;
	trueDir.y = (len == 0) ? 0 : trueDir.y / len;

	sf::Vector2f direction = rotateVector(trueDir, data.divergenceAngle);

	if (getConfiguredRotateSprite()) {
		setSpriteRotation(atan2(direction.y, direction.x));
	}

	setVelocity(m_data.speed * direction);
}

void ProjectileSpell::init(const SpellData& data) {
	setSpriteOffset(sf::Vector2f(-19.f, 1.f));

	Animation* spellAnimation = new Animation();
	spellAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_spell_projectile));
	spellAnimation->addFrame(sf::IntRect(0, data.skinNr * 9, 48, 9));

	addAnimation(GameObjectState::Idle, spellAnimation);

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(false);
}

