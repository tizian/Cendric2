#include "Level/DynamicTiles/ModifierTile.h"
#include "Level/LevelMainCharacter.h"
#include "Spells/Spell.h"
#include "Screens/LevelScreen.h"
#include "Registrar.h"
#include "GameObjectComponents/LightComponent.h"
#include "GameObjectComponents/ParticleComponent.h"
#include "GlobalResource.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Modifier, ModifierTile)

ModifierTile::ModifierTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {
}

bool ModifierTile::init(const LevelTileProperties& properties) {
	LevelScreen* lScreen = dynamic_cast<LevelScreen*>(m_screen);
	auto& modifiersUnlocked = lScreen->getCharacterCore()->getData().modifiersUnlocked;
	auto& worldID = lScreen->getWorld()->getID();
	if (contains(modifiersUnlocked, worldID) && contains(modifiersUnlocked.at(worldID), m_objectID)) {
		// modifier is already unlocked
		setDisposed();
		return true;
	}

	if (!contains(properties, std::string("type"))) return false;
	m_modifier.type = SpellModifier::resolveType(properties.at(std::string("type")));
	if (m_modifier.type == SpellModifierType::VOID) return false;

	auto const& modifiersLearned = lScreen->getCharacterCore()->getData().modfiersLearned;

	m_modifier.level = std::min(3, contains(modifiersLearned, m_modifier.type) ? 
		modifiersLearned.at(m_modifier.type) + 1 : 1);

	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, TILE_SIZE_F));
	loadComponents();

	return true;
}

void ModifierTile::loadAnimation(int skinNr) {
	m_isCollidable = false;

	sf::IntRect rect = sf::IntRect((m_modifier.level - 1) * 50, 50, 50, 50);
	m_animatedSprite.setColor(SpellModifier::getSpellModifierColor(m_modifier.type));

	const sf::Texture* tex = g_resourceManager->getTexture(GlobalResource::TEX_GEMS);
	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(rect);
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

void ModifierTile::update(const sf::Time& frameTime) {
	LevelDynamicTile::update(frameTime);

	if (m_state == GameObjectState::Active) {
		updateTime(m_particleTime, frameTime);
		if (m_particleTime == sf::Time::Zero) {
			setDisposed();
		}
	}
}

void ModifierTile::render(sf::RenderTarget& target) {
	if (m_isFirstRenderIteration) {
		GameObject::render(target);
		target.draw(m_animatedSprite);
		m_isFirstRenderIteration = false;
	}
	else {
		m_isFirstRenderIteration = true;
	}
}

void ModifierTile::addModifier() {
	m_state = GameObjectState::Active;
	setCurrentAnimation(getAnimation(m_state), false);

	LevelScreen* screen = dynamic_cast<LevelScreen*>(getScreen());
	screen->notifyModifierLearned(m_modifier.type, m_objectID);
	m_pc->setEmitRate(0.f);
}

void ModifierTile::onHit(LevelMovableGameObject* mob) {
	if (m_state == GameObjectState::Active) return;
	LevelMainCharacter* character = dynamic_cast<LevelMainCharacter*>(mob);
	if (character) {
		addModifier();
	}
}

void ModifierTile::onHit(Spell* spell) {
	// nop
}

void ModifierTile::loadComponents() {
	// light
	addComponent(new LightComponent(LightData(
		sf::Vector2f(TILE_SIZE_F * 0.5f, TILE_SIZE_F * 0.5f),
		sf::Vector2f(200.f, 200.f), 0.8f), this));

	// particles
	ParticleComponentData data;
	data.particleCount = 300;
	data.emitRate = 100.f * m_modifier.level / 3.0f;
	data.isAdditiveBlendMode = true;
	data.texturePath = GlobalResource::TEX_PARTICLE_STAR;
	
	auto posGen = new particles::PointSpawner();
	posGen->center = sf::Vector2f(getPosition().x + getBoundingBox()->width / 2.f, getPosition().y + getBoundingBox()->height / 2.f);
	data.spawner = posGen;

	auto sizeGen = new particles::SizeGenerator();
	sizeGen->minStartSize = 20.f;
	sizeGen->maxStartSize = 30.f;
	sizeGen->minEndSize = 0.f;
	sizeGen->maxEndSize = 2.f;
	data.sizeGen = sizeGen;

	auto colGen = new particles::ColorGenerator();
	colGen->minStartCol = SpellModifier::getSpellModifierColor(m_modifier.type);
	colGen->maxStartCol = sf::Color(255, 255, 255, 255);
	colGen->minEndCol = SpellModifier::getSpellModifierColor(m_modifier.type);
	colGen->maxEndCol = SpellModifier::getSpellModifierColor(m_modifier.type);
	colGen->maxEndCol.a = 0;
	data.colorGen = colGen;

	auto velGen = new particles::AngledVelocityGenerator();
	velGen->minAngle = 0.f;
	velGen->maxAngle = 360.f;
	velGen->minStartSpeed = 30.f * m_modifier.level / 3;
	velGen->maxStartSpeed = 60.f * m_modifier.level / 3;
	data.velGen = velGen;

	auto timeGen = new particles::TimeGenerator();
	timeGen->minTime = 2.f;
	timeGen->maxTime = 3.f;
	data.timeGen = timeGen;

	m_pc = new ParticleComponent(data, this);
	m_pc->setOffset(sf::Vector2f(m_boundingBox.width * 0.5f, m_boundingBox.height * 0.5f));
	addComponent(m_pc);
}