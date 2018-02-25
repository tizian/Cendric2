#include "Level/Enemies/DragonWhelpEnemy.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveFlyingBehavior.h"
#include "Level/MOBBehavior/MovingBehaviors/AllyFlyingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Dragonwhelp, DragonWhelpEnemy)

void DragonWhelpEnemy::insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = rand() % 3 + 1;
	if (rand() % 2 > 0)
		loot.insert({ "mi_teeth", 1 });
	if (rand() % 2 > 0)
		loot.insert({ "mi_dragonskull", 1 });
}

void DragonWhelpEnemy::insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = rand() % 3 + 1;
}

DragonWhelpEnemy::DragonWhelpEnemy(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen) {
}

void DragonWhelpEnemy::loadAttributes() {
	if (m_skinNr == 2 || m_skinNr == 3) {
		// purple whelp
		m_attributes.setHealth(100);
	}
	else {
		// default whelp
		m_attributes.setHealth(50);
	}
	m_attributes.critical = 10;
	m_attributes.resistancePhysical = 150;
	m_attributes.resistanceFire = 5000;
	m_attributes.calculateAttributes();
}

void DragonWhelpEnemy::loadSpells() {
	SpellData fireBallSpell = SpellData::getSpellData(SpellID::FireBall);
	fireBallSpell.damage = 20;
	fireBallSpell.damagePerSecond = 10;
	fireBallSpell.duration = sf::seconds(2);
	fireBallSpell.cooldown = sf::seconds(4.f);
	fireBallSpell.speed = 200.f;
	fireBallSpell.castingTime = sf::milliseconds(6 * 50);
	fireBallSpell.fightingTime = sf::milliseconds(4 * 50);
	fireBallSpell.count = 2;

	if (m_skinNr == 2 || m_skinNr == 3) {
		// purple whelp has purple fire
		fireBallSpell.damage = 40;
		fireBallSpell.skinNr = 2;
		fireBallSpell.damageType = DamageType::Shadow;
	}

	m_spellManager->addSpell(fireBallSpell);
	m_spellManager->setCurrentSpell(0);
}

void DragonWhelpEnemy::update(const sf::Time& frameTime) {
	if (m_hatchingTime == sf::Time::Zero) {
		if (m_isEgg) {
			if (fastIntersect(m_boundingBox, *m_mainChar->getBoundingBox())) {
				setActive();
			}
			for (auto go : *m_screen->getObjects(_Enemy)) {
				auto e = dynamic_cast<Enemy*>(go);
				if (e->isAlly() && fastIntersect(m_boundingBox, *e->getBoundingBox())) {
					setActive();
				}
			}
		}

		Enemy::update(frameTime);
		return;
	}

	updateTime(m_hatchingTime, frameTime);
	Enemy::update(frameTime);
	if (m_hatchingTime == sf::Time::Zero) {
		m_movingBehavior->setMaxVelocityYUp(100.f);
		m_movingBehavior->setMaxVelocityYDown(100.f);
		m_movingBehavior->setMaxVelocityX(100.f);
		unlockAnimation();
		setState(GameObjectState::Idle);
	}
}

MovingBehavior* DragonWhelpEnemy::createMovingBehavior(bool asAlly) {
	FlyingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyFlyingBehavior(this);
	}
	else {
		behavior = new AggressiveFlyingBehavior(this);
	}
	behavior->setApproachingDistance(100.f);

	float velocity = (m_skinNr == 0 || m_skinNr == 2) ? 0.f : 100.f;
	behavior->setMaxVelocityYDown(velocity);
	behavior->setMaxVelocityYUp(velocity);
	behavior->setMaxVelocityX(velocity);
	return behavior;
}

AttackingBehavior* DragonWhelpEnemy::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyBehavior(this);
	}
	else {
		behavior = new AggressiveBehavior(this);
	}
	// an ally never spawns in an egg and also, only 0 and 2 are egged skins.
	m_isEgg = !(asAlly || (m_skinNr == 1 || m_skinNr == 3));
	behavior->setAggroRange(300.f);
	behavior->setAttackInput(std::bind(&DragonWhelpEnemy::handleAttackInput, this));
	return behavior;
}

void DragonWhelpEnemy::onHit(Spell* spell) {
	Enemy::onHit(spell);
	setActive();
}

void DragonWhelpEnemy::setDead() {
	if (m_isDead) return;
	m_isEgg = false;
	m_movingBehavior->setMaxVelocityYUp(100.f);
	m_movingBehavior->setMaxVelocityYDown(200.f);
	m_movingBehavior->setMaxVelocityX(100.f);
	unlockAnimation();
	Enemy::setDead();
}

void DragonWhelpEnemy::setActive() {
	if (m_isDead || !m_isEgg) {
		return;
	}
	m_isEgg = false;
	setState(GameObjectState::Hatching);
	setCurrentAnimation(getAnimation(GameObjectState::Hatching), false, true);
	m_hatchingTime = getAnimation(GameObjectState::Hatching)->getAnimationTime();
}

void DragonWhelpEnemy::handleAttackInput() {
	if (m_isEgg) return;
	if (m_enemyState != EnemyState::Chasing) return;
	if (getCurrentTarget() == nullptr) return;
	if (m_hatchingTime > sf::Time::Zero) return;
	m_spellManager->executeCurrentSpell(getCurrentTarget());
}

sf::Time DragonWhelpEnemy::getConfiguredWaitingTime() const {
	return sf::seconds(static_cast<float>(rand() % 2 + 1));
}

sf::Time DragonWhelpEnemy::getConfiguredChasingTime() const {
	return sf::seconds(static_cast<float>(rand() % 4 + 2));
}

void DragonWhelpEnemy::loadAnimation(int skinNr) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 40.f, 30.f));
	setSpriteOffset(sf::Vector2f(-15.f, -35.f));
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	int width = 70;
	int height = 90;
	skinNr = skinNr / 2; // because we always do have egged and hatched skins

	Animation* flyingAnimation = new Animation(sf::milliseconds(50));
	flyingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 10; ++i) {
		flyingAnimation->addFrame(sf::IntRect(i * width, (height * 4) * skinNr, width, height));
	}

	addAnimation(GameObjectState::Flying, flyingAnimation);

	Animation* idleAnimation = new Animation(sf::milliseconds(50));
	idleAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 10; ++i) {
		idleAnimation->addFrame(sf::IntRect(i * width, (height * 4) * skinNr, width, height));
	}

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* castingAnimation = new Animation(sf::milliseconds(50));
	castingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 6; ++i) {
		castingAnimation->addFrame(sf::IntRect(i * width, (height * 4) * skinNr + height, width, height));
	}

	addAnimation(GameObjectState::Casting, castingAnimation);

	Animation* fightingAnimation = new Animation(sf::milliseconds(50));
	fightingAnimation->setSpriteSheet(tex);
	for (int i = 6; i < 10; ++i) {
		fightingAnimation->addFrame(sf::IntRect(i * width, (height * 4) * skinNr + height, width, height));
	}

	addAnimation(GameObjectState::Fighting, fightingAnimation);


	Animation* deadAnimation = new Animation(sf::milliseconds(80));
	deadAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 6; ++i) {
		deadAnimation->addFrame(sf::IntRect(i * width, (height * 4) * skinNr + 2 * height, width, height));
	}
	deadAnimation->setLooped(false);

	addAnimation(GameObjectState::Dead, deadAnimation);

	Animation* inactiveAnimation = new Animation(sf::seconds(10.f));
	inactiveAnimation->setSpriteSheet(tex);
	inactiveAnimation->addFrame(sf::IntRect(6 * width, (height * 4) * skinNr + 2 * height, width, height));
	inactiveAnimation->setLooped(false);

	addAnimation(GameObjectState::Inactive, inactiveAnimation);

	Animation* hatchingAnimation = new Animation(sf::milliseconds(50));
	hatchingAnimation->setSpriteSheet(tex);
	for (int i = 6; i < 10; ++i) {
		hatchingAnimation->addFrame(sf::IntRect(i * width, (height * 4) * skinNr + 2 * height, width, height));
	}
	for (int i = 0; i < 10; ++i) {
		hatchingAnimation->addFrame(sf::IntRect(i * width, (height * 4) * skinNr + 3 * height, width, height));
	}
	hatchingAnimation->setLooped(false);

	addAnimation(GameObjectState::Hatching, hatchingAnimation);

	// initial values
	if (m_skinNr == 1 || m_skinNr == 3) {
		setState(GameObjectState::Idle);
		setCurrentAnimation(getAnimation(GameObjectState::Idle));
		
	} else {
		setState(GameObjectState::Inactive);
		setCurrentAnimation(getAnimation(GameObjectState::Inactive), false, true);
	}

	playCurrentAnimation(true);
}

std::string DragonWhelpEnemy::getSpritePath() const {
	return "res/texture/enemies/spritesheet_enemy_dragonwhelp.png";
}

std::string DragonWhelpEnemy::getDeathSoundPath() const {
	return "res/sound/mob/dragon_death.ogg";
}
