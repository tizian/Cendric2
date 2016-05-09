#include "Level/DynamicTiles/ShootingTile.h"
#include "Spells/Spell.h"
#include "Registrar.h"
#include "Screens/WorldScreen.h"
#include "Spells/ProjectileSpell.h"
#include "Spells/FireBallSpell.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Shooting, ShootingTile)

ShootingTile::ShootingTile(LevelScreen* levelScreen) :
	LevelDynamicTile(levelScreen) {
}

void ShootingTile::init() {
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
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
		m_recoveringTime = sf::seconds(20.f);
		m_cooldown = sf::seconds(5.f);
		break;
	}
	case 2:
	case 3:
	{
		m_spellData = SpellData::getSpellData(SpellID::FireBall);
		m_spellData.duration = sf::seconds(3.f);
		m_spellData.damagePerSecond = 10;
		m_spellData.damage = 20;
		m_spellData.divergenceAngle = 0.f;
		m_cooldown = sf::seconds(4.f);
		m_activeTime = sf::seconds(0.6f);
		m_spellOffsetTime = sf::seconds(0.1f);
		m_recoveringTime = sf::seconds(15.f);
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
	idleAnimation->addFrame(sf::IntRect(0, (skinNr - 1) * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* activeAnimation = new Animation(sf::milliseconds(60));
	activeAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_shooting));

	switch (skinNr) {
	case 1:
	default:
		// the turning wheel
		for (int i = 0; i < 4; ++i) {
			activeAnimation->addFrame(sf::IntRect(TILE_SIZE * i, (skinNr - 1) * TILE_SIZE, TILE_SIZE, TILE_SIZE));
		}
		break;
	case 2:
	case 3:
		// fire flowers
		activeAnimation->addFrame(sf::IntRect(TILE_SIZE * 1, (skinNr - 1) * TILE_SIZE, TILE_SIZE, TILE_SIZE));
		activeAnimation->addFrame(sf::IntRect(TILE_SIZE * 0, (skinNr - 1) * TILE_SIZE, TILE_SIZE, TILE_SIZE));
		activeAnimation->addFrame(sf::IntRect(TILE_SIZE * 2, (skinNr - 1) * TILE_SIZE, TILE_SIZE, TILE_SIZE));
		activeAnimation->addFrame(sf::IntRect(TILE_SIZE * 3, (skinNr - 1) * TILE_SIZE, TILE_SIZE, TILE_SIZE));
		activeAnimation->setLooped(false);
		break;
	}

	addAnimation(GameObjectState::Active, activeAnimation);

	Animation* brokenAnimation = new Animation();
	brokenAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_shooting));
	brokenAnimation->addFrame(sf::IntRect(TILE_SIZE * 4, (skinNr - 1) * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Broken, brokenAnimation);

	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_shooting));
	deadAnimation->addFrame(sf::IntRect(TILE_SIZE * 5, (skinNr - 1) * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Dead, deadAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

void ShootingTile::update(const sf::Time& frameTime) {
	LevelDynamicTile::update(frameTime);
	if (m_remainingRecoveringTime > sf::Time::Zero) {
		updateTime(m_remainingRecoveringTime, frameTime);
		if (m_remainingRecoveringTime == sf::Time::Zero) {
			setState(GameObjectState::Idle);
			m_isBroken = false;
		}
		if (m_state == GameObjectState::Dead)
			return;
	}
	if (m_remainingActiveTime > sf::Time::Zero) {
		updateTime(m_remainingActiveTime, frameTime);
		if (m_remainingActiveTime == sf::Time::Zero) {
			setState(m_isBroken ? GameObjectState::Broken : GameObjectState::Idle);
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
	case 2:
	{
		FireBallSpell* spell = new FireBallSpell();
		spell->load(m_spellData, this, sf::Vector2f(getPosition().x + getBoundingBox()->width / 2.f, getPosition().y));
		m_screen->addObject(spell);
		break;
	}
	case 3:
	{
		FireBallSpell* spell = new FireBallSpell();
		spell->load(m_spellData, this, sf::Vector2f(getPosition().x + getBoundingBox()->width / 2.f, getPosition().y + getBoundingBox()->height));
		m_screen->addObject(spell);
		break;
	}
	}
}

void ShootingTile::onHit(Spell* spell) {
	if (spell->getSpellID() != SpellID::Chop) return;
	
	if (m_isBroken) {
		setState(GameObjectState::Dead);
	}
	else {
		setState(GameObjectState::Broken);
		m_isBroken = true;
	}
	spell->setDisposed();
	m_remainingRecoveringTime = m_recoveringTime;
}

