#include "Level/Enemies/RoyalguardBoss.h"
#include "Level/Level.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehavior.h"
#include "Screens/LevelScreen.h"

const sf::Vector2f RoyalguardBoss::WEAPON_ORIGIN = sf::Vector2f(56.f, 15.f);
const sf::Time RoyalguardBoss::HEALING_TIME = sf::seconds(5.f);

RoyalguardBoss::RoyalguardBoss(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen) {

	m_isAlwaysUpdate = true;
	m_isBoss = true;
	m_stateTime = sf::seconds(1.f);
}

void RoyalguardBoss::loadBoss() {
	loadWeapon();
	loadParticles();
	loadHealingParticles();
}

void RoyalguardBoss::notifyOtherDeath(const sf::Vector2f& newPos, RoyalguardBoss* other) {
	if (m_isDead) return;
	m_other = other;
	setPosition(newPos);

	for (auto it : m_components) {
		if (auto pc = dynamic_cast<ParticleComponent*>(it)) {
			pc->setVisible(false);
			pc->reset();
		}
	}

	m_weaponOffset = sf::Vector2f(34.f, -5.f);
	m_isWeaponVisible = true;
	m_stateTime = HEALING_TIME;
	m_bossState = RoyalguardBossState::Healing;
	m_weaponRotateType = WeaponRotateType::Fixed;
	getMovingBehavior()->setMaxVelocityX(200.f);
	m_healingPc->setVisible(true);
	m_weapon.setRotation(270.f); // up
}

void RoyalguardBoss::revive() {
	if (!m_isDead) return;

	m_isDead = false;
	m_attributes.currentHealthPoints = m_attributes.maxHealthPoints;
}

void RoyalguardBoss::setDead() {
	if (m_isDead) return;
	Enemy::setDead();
	clearReward();

	m_isWeaponVisible = false;
	for (auto pc : m_pcs) {
		pc->setVisible(false);
	}

	WorldCollisionQueryRecord rec;
	rec.boundingBox = *getBoundingBox();

	for (auto go : *m_screen->getObjects(GameObjectType::_Enemy)) {
		if (auto other = dynamic_cast<RoyalguardBoss*>(go)) {
			if (other == this) continue;
			rec.boundingBox.left += 20.f;
			other->notifyOtherDeath(getPosition() + (m_level->collides(rec) ? sf::Vector2f() : sf::Vector2f(20.f, 0.f)), this);
			rec.boundingBox.left -= 20.f;
			break;
		}
	}
}

void RoyalguardBoss::update(const sf::Time& frameTime) {
	Enemy::update(frameTime);
	updateBossState(frameTime);
	if (m_isWeaponVisible) {
		updateWeapon(frameTime);
	}
	for (auto pc : m_pcs) {
		pc->flipOffsetX(!isFacingRight());
	}
}

void RoyalguardBoss::render(sf::RenderTarget& target) {
	Enemy::render(target);
	if (m_isWeaponVisible) {
		target.draw(m_weapon);
	}
}

void RoyalguardBoss::loadWeapon() {
	g_resourceManager->loadTexture(getWeaponTexturePath(), ResourceType::Level);
	auto tex = g_resourceManager->getTexture(getWeaponTexturePath());
	if (!tex) return;
	
	m_weapon.setTexture(*tex);
	m_weapon.setOrigin(WEAPON_ORIGIN);
}

void RoyalguardBoss::loadHealingParticles() {
	// particles
	ParticleComponentData data;
	data.particleCount = 20;
	data.texturePath = GlobalResource::TEX_PARTICLE_BLOB;
	data.particleTexture = &dynamic_cast<LevelScreen*>(m_screen)->getParticleFGRenderTexture();
	data.emitRate = 5.f;
	data.isAdditiveBlendMode = true;

	auto spawner = new particles::BoxSpawner();
	spawner->size = sf::Vector2f(getBoundingBox()->width * 0.2f, 0.f);
	data.spawner = spawner;

	auto sizeGen = new particles::SizeGenerator();
	sizeGen->minStartSize = 10.f;
	sizeGen->maxStartSize = 30.f;
	sizeGen->minEndSize = 5.f;
	sizeGen->maxEndSize = 10.f;
	data.sizeGen = sizeGen;

	auto colGen = new particles::ColorGenerator();
	colGen->minStartCol = sf::Color(255, 255, 204, 100);
	colGen->maxStartCol = sf::Color(255, 255, 255, 150);
	colGen->minEndCol = sf::Color(255, 255, 0, 0);
	colGen->maxEndCol = sf::Color(255, 255, 255, 0);
	data.colorGen = colGen;

	auto timeGen = new particles::TimeGenerator();
	timeGen->minTime = 2.0f;
	timeGen->maxTime = 4.0f;
	data.timeGen = timeGen;

	auto velgen = new particles::AngledVelocityGenerator();
	velgen->maxAngle = 190.f;
	velgen->minAngle = 170.f;
	velgen->minStartSpeed = 20.f;
	velgen->maxStartSpeed = 40.f;
	data.velGen = velgen;

	m_healingPc = new ParticleComponent(data, this);
	m_healingPc->setOffset(sf::Vector2f(getBoundingBox()->width * 0.5f, -40.f));
	m_healingPc->setVisible(false);
	addComponent(m_healingPc);
	m_pcs.push_back(m_healingPc);
}

void RoyalguardBoss::updateWeapon(const sf::Time& frameTime) {
	sf::Vector2f offset = isFacingRight() ? m_weaponOffset : sf::Vector2f(m_boundingBox.width - m_weaponOffset.x, m_weaponOffset.y);
	m_weapon.setPosition(getPosition() + offset);
	switch (m_weaponRotateType)
	{
	case WeaponRotateType::ToMainChar: 
	{
		sf::Vector2f toMainChar = m_mainChar->getCenter() - m_weapon.getPosition();
		m_weapon.setRotation(radToDeg(std::atan2(toMainChar.y, toMainChar.x)));
		break;
	}
	case WeaponRotateType::Turn:
	{
		float rot = isFacingRight() ? 1000.f : -1000.f;
		m_weapon.setRotation(m_weapon.getRotation() + frameTime.asSeconds() * rot);
		break;
	}
		
	case WeaponRotateType::Fixed:
	default:
		break;
	}
}

void RoyalguardBoss::clearBossState() {
	m_stateTime = sf::Time::Zero;
}

sf::Time RoyalguardBoss::getConfiguredWaitingTime() const {
	return sf::Time::Zero;
}

float RoyalguardBoss::getConfiguredDistanceToHPBar() const {
	return 30.f;
}
