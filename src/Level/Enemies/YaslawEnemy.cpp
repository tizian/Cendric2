#include "Level/Enemies/YaslawEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AllyWalkingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "GameObjectComponents/InteractComponent.h"
#include "Level/MOBBehavior/ScriptedBehavior/ScriptedBehavior.h"
#include "Spells/SpellManager.h"
#include "Screens/Screen.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Yaslaw, YaslawEnemy)

YaslawEnemy::YaslawEnemy(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen) {
}

void YaslawEnemy::loadAttributes() {
	m_attributes.setHealth(100);
	m_attributes.resistanceFire = 30;
	m_attributes.resistanceShadow = 30;
	m_attributes.resistanceLight = 30;
	m_attributes.resistanceShadow = 30;
	m_attributes.critical = 10;
	m_attributes.calculateAttributes();

	m_isInvincible = true;
}

void YaslawEnemy::loadSpells() {
	SpellData leechSpell = SpellData::getSpellData(SpellID::Leech);
	leechSpell.damage = 20;
	leechSpell.duration = sf::seconds(2.f);
	leechSpell.count = 2;
	leechSpell.cooldown = sf::milliseconds(3000);
	leechSpell.fightingTime = sf::milliseconds(400);
	leechSpell.castingTime = sf::milliseconds(500);
	leechSpell.isBlocking = true;

	m_spellManager->addSpell(leechSpell);

	m_spellManager->setCurrentSpell(0); // leech
}

void YaslawEnemy::handleAttackInput() {
	if (m_enemyAttackingBehavior->distToTarget() < 500.f) {
		m_spellManager->executeCurrentSpell(getCurrentTarget());
	}
}

void YaslawEnemy::loadAnimation(int skinNr) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 30.f, 90.f));
	setSpriteOffset(sf::Vector2f(-45.f, -30.f));
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* walkingAnimation = new Animation();
	walkingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 8; ++i) {
		walkingAnimation->addFrame(sf::IntRect(i * 120, skinNr * 120, 120, 120));
	}

	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(8 * 120, skinNr * 120, 120, 120));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* jumpingAnimation = new Animation();
	jumpingAnimation->setSpriteSheet(tex);
	jumpingAnimation->addFrame(sf::IntRect(9 * 120, skinNr * 120, 120, 120));

	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	Animation* deadAnimation = new Animation(sf::milliseconds(70));
	deadAnimation->setSpriteSheet(tex);
	deadAnimation->addFrame(sf::IntRect(10 * 120, skinNr * 120, 120, 120));
	deadAnimation->setLooped(false);
	addAnimation(GameObjectState::Dead, deadAnimation);

	Animation* fightingStartAnimation = new Animation();
	fightingStartAnimation->setSpriteSheet(tex);
	fightingStartAnimation->addFrame(sf::IntRect(11 * 120, skinNr * 120, 120, 120));
	fightingStartAnimation->addFrame(sf::IntRect(12 * 120, skinNr * 120, 120, 120));
	fightingStartAnimation->addFrame(sf::IntRect(13 * 120, skinNr * 120, 120, 120));
	fightingStartAnimation->addFrame(sf::IntRect(14 * 120, skinNr * 120, 120, 120));
	fightingStartAnimation->setLooped(false);

	addAnimation(GameObjectState::Casting, fightingStartAnimation);

	Animation* fightingAnimation = new Animation();
	fightingAnimation->setSpriteSheet(tex);
	fightingAnimation->addFrame(sf::IntRect(14 * 120, skinNr * 120, 120, 120));
	fightingStartAnimation->setLooped(false);

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	// light
	LightData data(sf::Vector2f(40.f, -10.f), sf::Vector2f(70.f, 100.f), 0.8f);
	m_lightComponent = new LightComponent(data, this);
	addComponent(m_lightComponent);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);

	// initializing a yaslaw will delete all other yaslaw so that only one yaslaw exists!!
	std::string oldLuaPath;
	for (auto it : *m_screen->getObjects(_Enemy)) {
		Enemy* e = dynamic_cast<Enemy*>(it);
		if (e->getEnemyID() == EnemyID::Yaslaw && e != this) {
			if (e->getScriptedBehavior()) {
				oldLuaPath = e->getScriptedBehavior()->getLuaPath();
			}

			e->setDisposed();
		}
	}

	if (!oldLuaPath.empty())
		setScriptedBehavior(oldLuaPath);
}

void YaslawEnemy::update(const sf::Time& frameTime) {
	Enemy::update(frameTime);
	m_lightComponent->flipOffsetX(!m_enemyMovingBehavior->isFacingRight());
}

void YaslawEnemy::setAlly(const sf::Time& ttl) {
	Enemy::setAlly(sf::seconds(46.f));
}

void YaslawEnemy::setDead() {
	Enemy::setDead();
	m_lightComponent->setVisible(false);
	m_interactComponent->setInteractable(false);
}

MovingBehavior* YaslawEnemy::createMovingBehavior(bool asAlly) {
	AllyWalkingBehavior* behavior = new AllyWalkingBehavior(this);
	behavior->setDistanceToAbyss(20.f);
	behavior->setApproachingDistance(50.f);
	behavior->setMaxVelocityYDown(800.f);
	behavior->setMaxVelocityYUp(600.f);
	behavior->setMaxVelocityX(150.f);
	behavior->calculateJumpHeight();
	behavior->setReplaceDistance(10000.f);
	return behavior;
}

AttackingBehavior* YaslawEnemy::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior = new AllyBehavior(this);
	behavior->setAggroRange(800.f);
	behavior->setAttackInput(std::bind(&YaslawEnemy::handleAttackInput, this));
	return behavior;
}

std::string YaslawEnemy::getSpritePath() const {
	return "res/texture/enemies/spritesheet_enemy_yaslaw.png";
}