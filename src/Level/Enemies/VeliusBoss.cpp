#include "Level/Enemies/VeliusBoss.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/VeliusBossMovingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/ScriptedBehavior/ScriptedBehavior.h"
#include "Level/DynamicTiles/MirrorTile.h"
#include "Screens/LevelScreen.h"
#include "Registrar.h"
#include "GlobalResource.h"

REGISTER_ENEMY(EnemyID::Boss_Velius, VeliusBoss)

VeliusBoss::VeliusBoss(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen),
	Boss(level, screen) {
}

VeliusBoss::~VeliusBoss() {
	delete m_ray;
}

void VeliusBoss::update(const sf::Time& frameTime) {
	Boss::update(frameTime);
	if (m_ray) m_ray->update(frameTime);
}

void VeliusBoss::render(sf::RenderTarget& target) {
	//Boss::render(target);
	if (m_ray) m_ray->render(target);
}

void VeliusBoss::loadAttributes() {
	m_attributes.setHealth(500);
	m_attributes.resistanceIce = -20;
	m_attributes.resistanceLight = -20;
	m_attributes.resistanceFire = 50;
	m_attributes.resistanceShadow = 10000;
	m_attributes.critical = 100;
	m_attributes.calculateAttributes();
}

void VeliusBoss::loadSpells() {
	// this is the stunning projectile
	SpellData projectile = SpellData::getSpellData(SpellID::Projectile);
	projectile.skinNr = 2;
	projectile.damage = 25;
	projectile.damagePerSecond = 6;
	projectile.isStunning = true;
	projectile.duration = sf::seconds(1.f);
	projectile.cooldown = sf::seconds(5.f);
	projectile.isBlocking = true;
	projectile.fightingTime = sf::seconds(1.f);
	projectile.fightAnimation = GameObjectState::Fighting;
	projectile.castingTime = sf::seconds(1.f);
	projectile.castingAnimation = GameObjectState::Casting;
	projectile.speed = 300;

	m_spellManager->addSpell(projectile);

	m_spellManager->setCurrentSpell(0); // stun
	m_spellManager->setGlobalCooldown(sf::seconds(3.f));
}

void VeliusBoss::handleAttackInput() {
	// todo
	if (m_ray) return;
	
	m_ray = new MirrorRay(dynamic_cast<LevelScreen*>(m_screen));
	m_ray->initRay(getPosition(), sf::Vector2f(1.f, 0.f));
}

void VeliusBoss::loadAnimation(int skinNr) {
	int width = 120;
	int height = 150;

	setBoundingBox(sf::FloatRect(0.f, 0.f, 25.f, 115.f));
	setSpriteOffset(sf::Vector2f(-50.f, -35.f));
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* walkingAnimation = new Animation(sf::seconds(0.1f));
	walkingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 8; i++) {
		walkingAnimation->addFrame(sf::IntRect(i * width, 0, width, height));
	}

	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(8 * width, 0, width, height));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* jumpingAnimation = new Animation();
	jumpingAnimation->setSpriteSheet(tex);
	jumpingAnimation->addFrame(sf::IntRect(9 * width, 0, width, height));

	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(tex);
	deadAnimation->addFrame(sf::IntRect(8 * width, 0, width, height));
	deadAnimation->setLooped(false);

	addAnimation(GameObjectState::Dead, deadAnimation);

	Animation* fightingAnimation = new Animation(sf::seconds(0.1f));
	fightingAnimation->setSpriteSheet(tex);
	fightingAnimation->addFrame(sf::IntRect(10 * width, 0, width, height));
	fightingAnimation->setLooped(false);

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	Animation* castingAnimation = new Animation(sf::seconds(0.1f));
	castingAnimation->setSpriteSheet(tex);
	castingAnimation->addFrame(sf::IntRect(10 * width, 0, width, height));
	castingAnimation->setLooped(false);

	addAnimation(GameObjectState::Casting, castingAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

MovingBehavior* VeliusBoss::createMovingBehavior(bool asAlly) {
	WalkingBehavior* behavior;
	behavior = new VeliusBossMovingBehavior(this);
	behavior->setDistanceToAbyss(100.f);
	behavior->setApproachingDistance(600.f);
	behavior->setMaxVelocityYDown(800.f);
	behavior->setMaxVelocityYUp(600.f);
	behavior->setMaxVelocityX(200.f);
	behavior->setDropAlways(true);
	behavior->calculateJumpHeight();
	return behavior;
}

AttackingBehavior* VeliusBoss::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	behavior = new AggressiveBehavior(this);
	behavior->setAggroRange(10000.f);
	behavior->setAttackInput(std::bind(&VeliusBoss::handleAttackInput, this));
	return behavior;
}

float VeliusBoss::getConfiguredDistanceToHPBar() const {
	return 30.f;
}

std::string VeliusBoss::getSpritePath() const {
	return "res/texture/bosses/spritesheet_boss_velius.png";
}