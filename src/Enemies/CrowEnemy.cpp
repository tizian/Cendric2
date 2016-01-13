#include "Enemies/CrowEnemy.h"
#include "Level/LevelMainCharacter.h"

void CrowEnemy::insertDefaultLoot(std::map<std::string, int>& loot, int& gold) {
	if (gold != 0 || !loot.empty()) return;

	gold = 2;
}

CrowEnemy::CrowEnemy(Level* level, LevelMainCharacter* mainChar) : 
	FlyingEnemy(level, mainChar, EnemyID::Crow),
	Enemy(level, mainChar, EnemyID::Crow),
	LevelMovableGameObject(level) {
	load();
	loadAttributes();
	loadSpells();
}

void CrowEnemy::loadAttributes() {
	m_attributes.setHealth(60);
	m_attributes.resistancePhysical = 10;
	m_attributes.calculateAttributes();
}

void CrowEnemy::loadSpells() {
	SpellData chopSpell = SpellData::getSpellData(SpellID::Chop);
	chopSpell.activeDuration = sf::milliseconds(500);
	chopSpell.cooldown = sf::milliseconds(1000);
	chopSpell.damage = 2;
	chopSpell.damagePerSecond = 10;
	chopSpell.duration = sf::seconds(3.f);
	chopSpell.boundingBox = sf::FloatRect(10, 0, 30, 30);

	m_spellManager->addSpell(chopSpell);
	m_spellManager->setCurrentSpell(0);
}

sf::Vector2f CrowEnemy::getConfiguredSpellOffset() const {
	return sf::Vector2f(-10.f, 0.f);
}

void CrowEnemy::handleAttackInput() {
	if (m_enemyState != EnemyState::Chasing) return;
	if (distToMainChar() < 50.f) {
		m_spellManager->executeCurrentSpell(m_mainChar->getCenter());
		m_chasingTime = sf::Time::Zero;
		m_waitingTime = sf::seconds(static_cast<float>(rand() % 8 + 3));;
	}
}

float CrowEnemy::getAggroRange() const {
	return 400.f;
}

float CrowEnemy::getApproachingDistance() const {
	return 10.f;
}

sf::Time CrowEnemy::getConfiguredWaitingTime() const {
	return sf::seconds(static_cast<float>(rand() % 4 + 1));
}

sf::Time CrowEnemy::getConfiguredChasingTime() const {
	return sf::seconds(static_cast<float>(rand() % 4 + 2));
}

void CrowEnemy::load() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 60.f, 54.f));
	setSpriteOffset(sf::Vector2f(-5.f, -5.f));

	Animation flyingAnimation;
	flyingAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_crow));
	flyingAnimation.addFrame(sf::IntRect(0, 0, 70, 64));
	flyingAnimation.addFrame(sf::IntRect(70, 0, 70, 64));
	flyingAnimation.addFrame(sf::IntRect(140, 0, 70, 64));
	flyingAnimation.addFrame(sf::IntRect(210, 0, 70, 64));

	addAnimation(GameObjectState::Flying, flyingAnimation);

	addAnimation(GameObjectState::Idle, flyingAnimation);

	Animation fightingAnimation;
	fightingAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_crow));
	fightingAnimation.addFrame(sf::IntRect(0, 64, 70, 64));
	fightingAnimation.addFrame(sf::IntRect(70, 64, 70, 64));
	fightingAnimation.addFrame(sf::IntRect(140, 64, 70, 64));

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	Animation deadAnimation;
	deadAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_crow));
	deadAnimation.addFrame(sf::IntRect(0, 128, 70, 64));

	addAnimation(GameObjectState::Dead, deadAnimation);

	// initial values
	m_state = GameObjectState::Idle;
	m_isFacingRight = true;
	setCurrentAnimation(getAnimation(m_state), !m_isFacingRight);
	playCurrentAnimation(true);
}

sf::Time CrowEnemy::getConfiguredFightAnimationTime() const {
	return sf::milliseconds(3 * 100);
}

float CrowEnemy::getMaxVelocityYUp() const {
	return 100.f;
}

float CrowEnemy::getMaxVelocityYDown() const {
	return 100.f;
}

float CrowEnemy::getMaxVelocityX() const {
	return 100.f;
}
