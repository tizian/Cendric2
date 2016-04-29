#include "Level/DynamicTiles/ShootingTile.h"
#include "Spells/Spell.h"
#include "Registrar.h"
#include "Screens/WorldScreen.h"
#include "SpellCreators/ProjectileSpellCreator.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Shooting, ShootingTile)

ShootingTile::ShootingTile(LevelScreen* levelScreen) :
	LevelDynamicTile(levelScreen) {
}

void ShootingTile::init() {
	setSpriteOffset(sf::Vector2f(-25.f, 0.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, TILE_SIZE_F));
}

void ShootingTile::loadSpells() {
	switch (m_skinNr) {
	case 1:
	default:
	{
		m_spellData = SpellData::getSpellData(SpellID::Projectile);
		m_spellData.duration = sf::seconds(3.f);
		m_spellData.damagePerSecond = 5;
		m_spellData.divergenceAngle = 0.f;
		m_activeTime = sf::seconds(1.f);
		m_spellOffsetTime = sf::seconds(0.5f);
		m_cooldown = sf::seconds(5.f);
		break;
	}
	}
	m_spellData.isAlly = false;
}

void ShootingTile::loadAnimation(int skinNr) {
	m_skinNr = skinNr;
	m_isCollidable = false;
	loadSpells();

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_shooting));
	idleAnimation->addFrame(sf::IntRect(0, (skinNr - 1) * TILE_SIZE, TILE_SIZE * 2, TILE_SIZE));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* activeAnimation = new Animation(sf::milliseconds(60));
	activeAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_shooting));
	for (int i = 0; i < 4; ++i) {
		activeAnimation->addFrame(sf::IntRect(TILE_SIZE * 2 * i, (skinNr - 1) * TILE_SIZE, TILE_SIZE * 2, TILE_SIZE));
	}

	addAnimation(GameObjectState::Active, activeAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

void ShootingTile::update(const sf::Time& frameTime) {
	LevelDynamicTile::update(frameTime);
	if (m_remainingActiveTime > sf::Time::Zero) {
		updateTime(m_remainingActiveTime, frameTime);
		if (m_remainingActiveTime == sf::Time::Zero) {
			setState(GameObjectState::Idle);
		}
	}

	if (m_remainingSpellOffsetTime > sf::Time::Zero) {
		updateTime(m_remainingSpellOffsetTime, frameTime);
		if (m_remainingSpellOffsetTime == sf::Time::Zero) {
			executeSpells();
		}
	}

	updateTime(m_remainingCooldown, frameTime);
	if (m_remainingCooldown == sf::Time::Zero) {
		m_remainingCooldown = m_cooldown;
		m_remainingActiveTime = m_activeTime;
		m_remainingSpellOffsetTime = m_spellOffsetTime;
		setState(GameObjectState::Active);
	}
}

void ShootingTile::executeSpells() {
	switch (m_skinNr) {
	case 1:
	default:
	{
		ProjectileSpell* spell1 = new ProjectileSpell();
		ProjectileSpell* spell2 = new ProjectileSpell();
		spell1->load(m_spellData, this, sf::Vector2f(getPosition().x, getPosition().y + getBoundingBox()->height / 2.f));
		spell2->load(m_spellData, this, sf::Vector2f(getPosition().x + getBoundingBox()->width, getPosition().y + getBoundingBox()->height / 2.f));
		m_screen->addObject(spell1);
		m_screen->addObject(spell2);
		break;
	}
	}
}

void ShootingTile::onHit(Spell* spell) {
	// nop, yet
}


