#include "Level/MOBBehavior/MovingBehaviors/ElysiaBossMovingBehavior.h"
#include "Level/Enemies/ElysiaBoss.h"
#include "Screens/LevelScreen.h"
#include "ObjectFactory.h"

const sf::Time ElysiaBossMovingBehavior::PROJECTILE_TIME = sf::seconds(5.f);
const sf::Time ElysiaBossMovingBehavior::NOSEDIVE_TIME = sf::seconds(1.5f);
const sf::Time ElysiaBossMovingBehavior::THUNDER_TIME = sf::seconds(2.f);

static const std::string SCREECH_SOUND = "res/sound/mob/elysia_screech.ogg";
static const std::string THUNDER_SOUND = "res/sound/mob/thunder.ogg";

ElysiaBossMovingBehavior::ElysiaBossMovingBehavior(ElysiaBoss* enemy) :
	MovingBehavior(enemy),
	EnemyMovingBehavior(enemy),
	FlyingBehavior(enemy) {
	m_boss = enemy;
	m_timeUntilTransition = PROJECTILE_TIME;

	// calculate targets
	float noseTargetHeight = 700.f;
	float thunderTop = 300.f;
	float thunderBottom = 1050.f;
	float levelWidth = m_enemy->getLevel()->getWorldRect().width;
	float bbWidth = enemy->getBoundingBox()->width;
	float bbHeight = enemy->getBoundingBox()->height;
	m_noseTargetLeft = sf::Vector2f(200.f, noseTargetHeight);
	m_noseTargetRight = sf::Vector2f(levelWidth - 100.f - bbWidth, noseTargetHeight);
	m_thunderTop = sf::Vector2f((levelWidth + TILE_SIZE_F) * 0.5f, thunderTop);
	m_thunderBottom = sf::Vector2f((levelWidth + TILE_SIZE_F) * 0.5f, thunderBottom);
	m_bossBoxOffset = sf::Vector2f(bbWidth * 0.5f, bbHeight * 0.5f);

	// load resources
	g_resourceManager->loadSoundbuffer(SCREECH_SOUND, ResourceType::Unique, this);
	g_resourceManager->loadSoundbuffer(THUNDER_SOUND, ResourceType::Unique, this);
	initTiles();
}

ElysiaBossMovingBehavior::~ElysiaBossMovingBehavior() {
	g_resourceManager->deleteUniqueResources(this);
}

void ElysiaBossMovingBehavior::execHandleMovementInput() {
	switch (m_boss->getBossState()) {
	case Projectile:
		handleProjectileMovement();
		return;
	case PreThunderstorm:
		handlePreThunder();
		return;
	case ThunderstormUp:
		handleThunderUp();
		return;
	case PreNosedive:
		handleNosediveUp();
		return;
	case Nosedive:
		handleNosediveDown();
		return;
	case Thunderstorm:
		handleThunderstorm();
		return;
	default:
		return;
	}
}

void ElysiaBossMovingBehavior::handleProjectileMovement() {
	gotoTarget(m_mainChar->getCenter() - sf::Vector2f(0.f, 200.f), 10.f);

	updateTime(m_timeUntilTransition, m_frameTime);
	if (m_timeUntilTransition == sf::Time::Zero) {
		if (rand() % 100 > m_thunderProbability) {
			// nosedive
			m_boss->setBossState(PreNosedive);
			m_isNoseRight = rand() % 2 == 0;
			m_flyingTarget = m_isNoseRight ? m_noseTargetRight : m_noseTargetLeft;
			m_maxVelocityX = 500;
			m_maxVelocityYUp = 500;
			m_thunderProbability += 20;
		}
		else {
			// thunderstorm
			m_boss->setBossState(PreThunderstorm);
			m_flyingTarget = m_thunderBottom;
			m_maxVelocityX = 300;
			m_maxVelocityYUp = 300;
			m_maxVelocityYDown = 300;
			m_thunderProbability = 20;
		}
	}
}

void ElysiaBossMovingBehavior::handleThunderstorm() {
	updateTime(m_timeUntilTransition, m_frameTime);
	m_movingDirectionY = 0;
	m_movingDirectionX = 0;

	updateTime(m_timeUntilNextThunder, m_frameTime);

	if (m_timeUntilNextThunder == sf::Time::Zero) {
		m_timeUntilNextThunder = m_nextThunderTime;
		float offset = (m_damagingTiles[0].first->getPosition().x == 0.f) ? TILE_SIZE_F : 0.f;
		for (auto& it : m_damagingTiles) {
			it.first->setPosition(sf::Vector2f(it.second + offset, it.first->getPosition().y + TILE_SIZE_F));
		}
	}

	if (m_timeUntilTransition == sf::Time::Zero) {
		m_boss->setBossState(Projectile);
		m_timeUntilTransition = PROJECTILE_TIME;
		m_maxVelocityX = 200;
		m_maxVelocityYUp = 200;

		for (auto& it : m_cloudTiles) {
			it.first->setDisposed();
			it.first = nullptr;
		}

		for (auto& it : m_damagingTiles) {
			it.first->setDisposed();
			it.first = nullptr;
		}
	}
}

void ElysiaBossMovingBehavior::handleNosediveDown() {
	m_enemy->getAnimatedSprite().setFlippedX(false);
	m_movingDirectionY = 0;
	m_movingDirectionX = 0;

	m_nosediveTime += m_frameTime;
	float dt = m_nosediveTime / NOSEDIVE_TIME;
	sf::Vector2f newPos;
	newPos.x = lerp(dt, m_nosediveStartX, m_nosediveEndX);
	newPos.y = m_flyingTarget.y - m_nosediveTrajHeight * std::sin(dt * M_PI);

	sf::Vector2f vel = newPos - m_enemy->getCenter();
	m_enemy->setSpriteRotation(atan2(vel.y, vel.x));
	m_enemy->setPosition(newPos - m_bossBoxOffset);

	if (dt >= 1.f) {
		m_boss->setBossState(Projectile);
		m_boss->setCurrentAnimation(m_boss->getAnimation(GameObjectState::Flying));
		setCollisionsEnabled(true);
		m_timeUntilTransition = PROJECTILE_TIME;
		m_maxVelocityX = 200;
		m_maxVelocityYUp = 200;
		m_enemy->setSpriteRotation(0.f);

		WorldCollisionQueryRecord rec;
		rec.boundingBox = *m_enemy->getBoundingBox();
		if (m_enemy->getLevel()->collides(rec)) {
			m_enemy->setPosition(m_isNoseRight ? m_noseTargetLeft - m_bossBoxOffset : m_noseTargetRight - m_bossBoxOffset);
		}
	}
}

void ElysiaBossMovingBehavior::handleNosediveUp() {
	gotoTarget(m_flyingTarget, 10.f);

	if (dist(m_enemy->getCenter(), m_flyingTarget) < 50.f) {
		m_boss->setBossState(Nosedive);
		m_boss->setCurrentAnimation(m_boss->getAnimation(GameObjectState::Fighting));
		setCollisionsEnabled(false);

		m_nosediveTrajHeight = m_enemy->getCenter().y - m_mainChar->getCenter().y;
		m_nosediveTime = sf::Time::Zero;
		m_nosediveStartX = m_enemy->getCenter().x;
		m_nosediveEndX = m_nosediveStartX + (m_mainChar->getCenter().x - m_enemy->getCenter().x) * 2.f;

		g_resourceManager->playSound(SCREECH_SOUND);
	}
}

void ElysiaBossMovingBehavior::handleThunderUp() {
	gotoTarget(m_flyingTarget, 10.f);

	if (dist(m_enemy->getCenter(), m_flyingTarget) < 20.f) {
		m_boss->setBossState(Thunderstorm);
		m_timeUntilTransition = THUNDER_TIME;
		m_timeUntilNextThunder = m_nextThunderTime;
		m_enemy->setVelocity(sf::Vector2f());

		g_resourceManager->playSound(THUNDER_SOUND);
		LevelTileProperties properties;

		// create thunder tiles
		for (auto& it : m_damagingTiles) {
			LevelDynamicTile* tile = ObjectFactory::Instance()->createLevelDynamicTile(LevelDynamicTileID::Damaging, dynamic_cast<LevelScreen*>(m_boss->getScreen()));
			tile->init(properties);
			tile->setPosition(sf::Vector2f(it.second, m_thunderStartY) + tile->getPositionOffset());
			tile->loadResources();
			tile->loadAnimation(0);
			tile->setPosition(sf::Vector2f(it.second, m_thunderStartY) + tile->getPositionOffset());
			tile->setDebugBoundingBox(COLOR_NEUTRAL);
			m_boss->getScreen()->addObject(tile);
			it.first = tile;
		}
	}
}

void ElysiaBossMovingBehavior::handlePreThunder() {
	gotoTarget(m_flyingTarget, 10.f);

	if (dist(m_enemy->getCenter(), m_flyingTarget) < 50.f) {
		m_boss->setPosition(m_flyingTarget - m_bossBoxOffset);
		m_boss->setBossState(ThunderstormUp);
		m_flyingTarget = m_thunderTop;
		m_maxVelocityX = 0;
		m_maxVelocityYUp = 100;
		m_maxVelocityYDown = 100;

		LevelTileProperties properties;

		// create cloud tiles
		for (auto& it : m_cloudTiles) {
			LevelDynamicTile* tile = ObjectFactory::Instance()->createLevelDynamicTile(LevelDynamicTileID::Disappearing, dynamic_cast<LevelScreen*>(m_boss->getScreen()));
			tile->init(properties);
			tile->setPosition(it.second + tile->getPositionOffset());
			tile->loadResources();
			tile->loadAnimation(4);
			tile->setPosition(it.second + tile->getPositionOffset());
			tile->setDebugBoundingBox(COLOR_NEUTRAL);
			m_boss->getScreen()->addObject(tile);
			it.first = tile;
		}
	}
}

void ElysiaBossMovingBehavior::update(const sf::Time& frameTime) {
	m_frameTime = frameTime;
	FlyingBehavior::update(frameTime);
}

void ElysiaBossMovingBehavior::updateAnimation(const sf::Time& frameTime) {
	GameObjectState newState = GameObjectState::Idle;
	if (m_boss->isDead()) {
		newState = GameObjectState::Dead;
	}
	else if (m_boss->getBossState() == ThunderstormUp || m_boss->getBossState() == Thunderstorm) {
		newState = GameObjectState::Casting3;
	}
	else if (m_boss->getBossState() == Nosedive) {
		newState = GameObjectState::Fighting;
	}
	else if (m_fightAnimationTime > sf::Time::Zero) {
		newState = m_fightAnimationState;
	}
	else if (std::abs(m_enemy->getVelocity().x) > 20.f || std::abs(m_enemy->getVelocity().y) > 20.f) {
		newState = GameObjectState::Flying;
	}

	// only update animation if we need to
	if (m_enemy->getState() != newState || (m_nextIsFacingRight != m_isFacingRight)) {
		m_isFacingRight = m_nextIsFacingRight;
		m_enemy->setState(newState);
		m_enemy->setCurrentAnimation(m_enemy->getAnimation(m_enemy->getState()), !m_isFacingRight);
	}
}

void ElysiaBossMovingBehavior::initTiles() {
	// init cloud tile positions
	float posY = std::floor(m_thunderBottom.y / TILE_SIZE_F) * TILE_SIZE_F;
	float posX = std::floor(m_thunderBottom.x / TILE_SIZE_F) * TILE_SIZE_F;
	float diffX = 2 * TILE_SIZE_F;
	int sign = 1;

	for (int i = 0; i <= 5; ++i) {
		m_cloudTiles.push_back(std::make_pair(nullptr, sf::Vector2f(posX - diffX, posY)));
		m_cloudTiles.push_back(std::make_pair(nullptr, sf::Vector2f(posX + diffX, posY)));

		posY -= 3 * TILE_SIZE_F;
		diffX += sign * 2 * TILE_SIZE_F;
		sign = -sign;
	}

	posY += 3 * TILE_SIZE_F;
	diffX = 6 * TILE_SIZE_F;
	m_cloudTiles.push_back(std::make_pair(nullptr, sf::Vector2f(posX - diffX, posY)));
	m_cloudTiles.push_back(std::make_pair(nullptr, sf::Vector2f(posX + diffX, posY)));

	// init damaging tile positions
	int number = static_cast<int>(m_enemy->getLevel()->getWorldRect().width / TILE_SIZE_F * 0.5f);
	posX = 0.f;
	for (int i = 0; i < number; ++i) {
		m_damagingTiles.push_back(std::make_pair(nullptr, posX));
		posX += 2 * TILE_SIZE_F;
	}

	m_thunderStartY = posY + 3 * TILE_SIZE_F;
	m_nextThunderTime = THUNDER_TIME / ((m_enemy->getLevel()->getWorldRect().height - m_thunderStartY - 200.f) / TILE_SIZE_F);
}

