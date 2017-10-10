#include "Level/DynamicTiles/SpellTile.h"
#include "Level/LevelMainCharacter.h"
#include "Spells/Spell.h"
#include "Screens/LevelScreen.h"
#include "Registrar.h"
#include "GameObjectComponents/LightComponent.h"
#include "GameObjectComponents/ParticleComponent.h"
#include "GlobalResource.h"
#include "Spells/LeapOfFaithSpell.h"
#include "Spells/FlashSpell.h"
#include "Spells/GhostFormSpell.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Spell, SpellTile)

const std::string SpellTile::THUNDER_SOUND_PATH = "res/sound/mob/thunder.ogg";

SpellTile::SpellTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {
}

bool SpellTile::init(const LevelTileProperties& properties) {
	LevelScreen* lScreen = dynamic_cast<LevelScreen*>(m_screen);

	if (!contains(properties, std::string("spellid"))) return false;
	m_spellID = static_cast<SpellID>(std::stoi(properties.at(std::string("spellid"))));
	if (m_spellID <= SpellID::VOID || m_spellID >= SpellID::MAX) return false;

	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, TILE_SIZE_F));
	loadComponents();
	return true;
}

void SpellTile::loadAnimation(int skinNr) {
	m_isCollidable = false;

	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());
	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(0, skinNr * TILE_SIZE, TILE_SIZE, TILE_SIZE));
	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* activatedAnimation = new Animation(sf::seconds(10.f));
	activatedAnimation->setSpriteSheet(tex);
	activatedAnimation->addFrame(sf::IntRect()); // idle
	addAnimation(GameObjectState::Active, activatedAnimation);

	// initial values
	m_state = GameObjectState::Idle;
	setCurrentAnimation(getAnimation(m_state), false);
	playCurrentAnimation(true);
}

void SpellTile::update(const sf::Time& frameTime) {
	LevelDynamicTile::update(frameTime);

	if (m_state == GameObjectState::Active) {
		updateTime(m_particleTime, frameTime);
		if (m_particleTime == sf::Time::Zero) {
			setDisposed();
		}
	}
}

void SpellTile::render(sf::RenderTarget& target) {
	if (m_isFirstRenderIteration) {
		GameObject::render(target);
		target.draw(m_animatedSprite);
		m_isFirstRenderIteration = false;
	}
	else {
		m_isFirstRenderIteration = true;
	}
}

void SpellTile::touch() {
	m_state = GameObjectState::Active;
	setCurrentAnimation(getAnimation(m_state), false);
	dynamic_cast<LevelScreen*>(m_screen)->notifySpellLearned(m_spellID);
	addSpell();

	m_pc->setEmitRate(0.f);
}

void SpellTile::addSpell() {
	switch (m_spellID)
	{
	case SpellID::LeapOfFaith: {
		loadLeapOfFaithSpell();
		break;
	}
	case SpellID::Flash: {
		loadFlashSpell();
		break;
	}
	case SpellID::GhostForm: {
		loadGhostFormSpell();
		break;
	}
	default:
		break;
	}
}

void SpellTile::loadLeapOfFaithSpell() {
	SpellData data = SpellData::getSpellData(SpellID::LeapOfFaith);
	data.duration = sf::seconds(999);
	data.activeDuration = data.duration;
	data.strength = 3;
	g_resourceManager->loadTexture(data.spritesheetPath, ResourceType::Level);

	float gravityScale = 1.f / (1.f + 0.2f * data.strength);
	LeapOfFaithSpell* newSpell = new LeapOfFaithSpell(gravityScale);
	newSpell->load(data, m_mainChar, sf::Vector2f());
	m_screen->addObject(newSpell);

	dynamic_cast<LevelScreen*>(m_screen)->addSpellBuffToInterface(data.iconTextureRect, data.duration, newSpell, AttributeData());
}

void SpellTile::loadGhostFormSpell() {
	SpellData data = SpellData::getSpellData(SpellID::GhostForm);;
	data.duration = sf::seconds(999);
	data.activeDuration = data.duration;
	data.strength = 3;
	AttributeData additionalDamage;
	additionalDamage.damagePhysical = data.strength;
	GhostFormSpell* newSpell = new GhostFormSpell(additionalDamage);
	newSpell->load(data, m_mainChar, sf::Vector2f());
	m_screen->addObject(newSpell);

	dynamic_cast<LevelScreen*>(m_screen)->addSpellBuffToInterface(data.iconTextureRect, data.duration, newSpell, additionalDamage);
}

void SpellTile::loadFlashSpell() {
	SpellData data = SpellData::getSpellData(SpellID::Flash);
	g_resourceManager->loadTexture(data.spritesheetPath, ResourceType::Level);
	g_resourceManager->loadSoundbuffer(THUNDER_SOUND_PATH, ResourceType::Level);
	g_resourceManager->playSound(THUNDER_SOUND_PATH, false);

	m_mainChar->setPosition(m_mainChar->getPosition() + sf::Vector2f(6 * TILE_SIZE_F, 0.f));
	FlashSpell* newSpell = new FlashSpell();
	newSpell->load(data, m_mainChar, sf::Vector2f());
	m_screen->addObject(newSpell);
}

void SpellTile::onHit(LevelMovableGameObject* mob) {
	if (m_state == GameObjectState::Active) return;
	LevelMainCharacter* character = dynamic_cast<LevelMainCharacter*>(mob);
	if (character) {
		touch();
	}
}

void SpellTile::loadComponents() {
	// light
	addComponent(new LightComponent(LightData(
		sf::Vector2f(TILE_SIZE_F * 0.5f, TILE_SIZE_F * 0.5f),
		sf::Vector2f(200.f, 200.f), 0.8f), this));

	// particles
	ParticleComponentData data;
	data.particleCount = 300;
	data.emitRate = 100.f;
	data.isAdditiveBlendMode = true;
	data.texturePath = GlobalResource::TEX_PARTICLE_STAR;

	auto posGen = new particles::PointSpawner();
	posGen->center = sf::Vector2f(getPosition().x + getBoundingBox()->width / 2.f, getPosition().y + getBoundingBox()->height / 2.f);
	data.spawner = posGen;

	auto sizeGen = new particles::SizeGenerator();
	sizeGen->minStartSize = 10.f;
	sizeGen->maxStartSize = 20.f;
	sizeGen->minEndSize = 0.f;
	sizeGen->maxEndSize = 2.f;
	data.sizeGen = sizeGen;

	sf::Color spellColor = getColorFromSpellType(SpellData::getSpellData(m_spellID).spellType);

	auto colGen = new particles::ColorGenerator();
	colGen->minStartCol = spellColor;
	colGen->maxStartCol = COLOR_WHITE;
	colGen->minEndCol = sf::Color(0, 0, 0, 0);
	colGen->maxEndCol = spellColor;
	colGen->maxEndCol.a = 0;
	data.colorGen = colGen;

	auto velGen = new particles::AngledVelocityGenerator();
	velGen->minAngle = 0.f;
	velGen->maxAngle = 360.f;
	velGen->minStartSpeed = 30.f;
	velGen->maxStartSpeed = 60.f;
	data.velGen = velGen;

	auto timeGen = new particles::TimeGenerator();
	timeGen->minTime = 2.f;
	timeGen->maxTime = 3.f;
	data.timeGen = timeGen;

	m_pc = new ParticleComponent(data, this);
	m_pc->setOffset(sf::Vector2f(m_boundingBox.width * 0.5f, m_boundingBox.height * 0.5f));
	addComponent(m_pc);
}

std::string SpellTile::getSpritePath() const {
	return "res/texture/level_dynamic_tiles/spritesheet_tiles_spell.png";
}
