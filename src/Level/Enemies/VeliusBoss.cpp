#include "Level/Enemies/VeliusBoss.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/VeliusBossMovingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/ScriptedBehavior/ScriptedBehavior.h"
#include "Level/DynamicTiles/MirrorTile.h"
#include "GameObjectComponents/LightComponent.h"
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
	updateBossState(frameTime);
	if (m_ray) m_ray->update(frameTime);
}

void VeliusBoss::render(sf::RenderTarget& target) {
	Boss::render(target);
	if (m_ray) m_ray->render(target);
}

void VeliusBoss::loadAttributes() {
	m_attributes.setHealth(2000);
	m_attributes.resistanceIce = -20;
	m_attributes.resistanceLight = -20;
	m_attributes.resistanceFire = 50;
	m_attributes.resistanceShadow = 10000;
	m_attributes.critical = 100;
	m_attributes.calculateAttributes();
}

void VeliusBoss::updateBossState(const sf::Time& frameTime) {

}

void VeliusBoss::setBossState(VeliusBossState state) {
	delete m_ray;
	m_ray = nullptr;
	switch (m_bossState)
	{
	case AttackIllusion:
		m_level->setBackgroundLayerColor(COLOR_MEDIUM_PURPLE);
		break;
	case ExtractDivine:
	{
		m_ray = new MirrorRay(dynamic_cast<LevelScreen*>(m_screen));
		m_ray->initRay(sf::Vector2f(775.f, 600.f), sf::Vector2f(0.f, -1.f), COLOR_DIVINE);
		m_level->setBackgroundLayerColor(COLOR_DIVINE);
		break;
	}
	case AttackDivine:
		break;
	case ExtractNecromancy:
	{
		m_ray = new MirrorRay(dynamic_cast<LevelScreen*>(m_screen));
		m_ray->initRay(sf::Vector2f(775.f, 600.f), sf::Vector2f(0.f, -1.f), COLOR_NECROMANCY);
		m_level->setBackgroundLayerColor(COLOR_NECROMANCY);
		break;
	}
	case AttackNecromancy:
		break;
	case ExtractTwilight: 
	{
		m_ray = new MirrorRay(dynamic_cast<LevelScreen*>(m_screen));
		m_ray->initRay(sf::Vector2f(775.f, 600.f), sf::Vector2f(0.f, -1.f), COLOR_TWILIGHT);
		m_level->setBackgroundLayerColor(COLOR_TWILIGHT);
		break;
	}
	case AttackTwilight:
		break;
	case ExtractElemental:
	{
		m_ray = new MirrorRay(dynamic_cast<LevelScreen*>(m_screen));
		m_ray->initRay(sf::Vector2f(775.f, 600.f), sf::Vector2f(0.f, -1.f), COLOR_ELEMENTAL);
		m_level->setBackgroundLayerColor(COLOR_ELEMENTAL);
		break;
	}
		break;
	default:
		break;
	}

	
}

void VeliusBoss::loadSpells() {
	// illusion spell
	SpellData projectile = SpellData::getSpellData(SpellID::FireBall);
	projectile.skinNr = 6;
	projectile.damage = 25;
	projectile.count = 3;
	projectile.duration = sf::seconds(1.f);
	projectile.cooldown = sf::seconds(5.f);
	projectile.isBlocking = true;
	projectile.fightingTime = sf::seconds(0.f);
	projectile.castingTime = sf::seconds(0.45f);
	projectile.castingAnimation = GameObjectState::Casting;
	projectile.speed = 300;

	m_spellManager->addSpell(projectile);

	m_spellManager->setGlobalCooldown(sf::seconds(3.f));
}

void VeliusBoss::handleAttackInput() {
	switch (m_bossState)
	{
	case AttackIllusion:
		m_spellManager->setCurrentSpell(0); // illusion attack
		break;
	case ExtractDivine:
	case ExtractNecromancy:
	case ExtractTwilight:
	case ExtractElemental:
		return;
	case AttackDivine:
		break;
	case AttackNecromancy:
		break;
	case AttackTwilight:
		break;
	default:
		break;
	}
	m_spellManager->executeCurrentSpell(m_mainChar); // illusion attack
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
	deadAnimation->addFrame(sf::IntRect(12 * width, 0, width, height));
	deadAnimation->setLooped(false);

	addAnimation(GameObjectState::Dead, deadAnimation);

	// the "throwning" spell
	Animation* castingAnimation = new Animation(sf::seconds(0.15f));
	castingAnimation->setSpriteSheet(tex);
	castingAnimation->addFrame(sf::IntRect(14 * width, 0, width, height));
	castingAnimation->addFrame(sf::IntRect(15 * width, 0, width, height));
	castingAnimation->addFrame(sf::IntRect(16 * width, 0, width, height));
	castingAnimation->setLooped(false);

	addAnimation(GameObjectState::Casting, castingAnimation);

	// the shackle
	Animation* casting2Animation = new Animation(sf::seconds(0.1f));
	casting2Animation->setSpriteSheet(tex);
	casting2Animation->addFrame(sf::IntRect(13 * width, 0, width, height));
	casting2Animation->setLooped(false);

	addAnimation(GameObjectState::Casting2, casting2Animation);

	// the crystal
	Animation* casting3Animation = new Animation(sf::seconds(0.1f));
	casting3Animation->setSpriteSheet(tex);
	casting3Animation->addFrame(sf::IntRect(10 * width, 0, width, height));
	casting3Animation->setLooped(false);

	addAnimation(GameObjectState::Casting3, casting3Animation);

	// the come at me, brah
	Animation* comeAnimation = new Animation(sf::seconds(0.1f));
	comeAnimation->setSpriteSheet(tex);
	comeAnimation->addFrame(sf::IntRect(11 * width, 0, width, height));
	comeAnimation->setLooped(false);

	addAnimation(GameObjectState::Broken, comeAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);

	// component: light
	addComponent(new LightComponent(LightData(
		sf::Vector2f(getBoundingBox()->width * 0.5f, 0.f),
		sf::Vector2f(100.f, 100.f), 0.6f), this));
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