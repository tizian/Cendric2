#include "Level/DynamicTiles/DamagingTile.h"
#include "GameObjectComponents/LightComponent.h"
#include "Spells/Spell.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Damaging, DamagingTile)

const std::string  DamagingTile::SHATTER_SOUND = "res/sound/mob/yashaadd_death.ogg";

bool DamagingTile::init(const LevelTileProperties& properties) {
	setPositionOffset(sf::Vector2f(5.f, 5.f));
	setSpriteOffset(sf::Vector2f(-5.f, -5.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F - 10.f, TILE_SIZE_F - 10.f));
	addComponent(new LightComponent(LightData(sf::Vector2f(TILE_SIZE_F * 0.5f, TILE_SIZE_F * 0.5f), TILE_SIZE_F, 0.5f), this));

	m_isDivine = contains(properties, std::string("divine"));

	g_resourceManager->loadSoundbuffer(SHATTER_SOUND, ResourceType::Level);
	return true;
}

void DamagingTile::loadAnimation(int skinNr) {
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* idleAnimation = new Animation(sf::seconds(0.05f));
	idleAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 4; ++i) {
		idleAnimation->addFrame(sf::IntRect(i * TILE_SIZE, skinNr * TILE_SIZE, TILE_SIZE, TILE_SIZE));
	}

	addAnimation(GameObjectState::Idle, idleAnimation);

	if (m_isDivine) {
		Animation* shatterAnimation = new Animation();
		shatterAnimation->setSpriteSheet(tex);
		for (int i = 0; i < 4; ++i) {
			shatterAnimation->addFrame(sf::IntRect(4 * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE));
		}
		shatterAnimation->setLooped(false);

		addAnimation(GameObjectState::Broken, shatterAnimation);
	}

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

void DamagingTile::update(const sf::Time& frameTime) {
	LevelDynamicTile::update(frameTime);
	if (!(m_isDivine && m_isBroken)) 
		return;
	updateTime(m_despawnTime, frameTime);
	if (m_despawnTime == sf::Time::Zero) {
		setDisposed();
	}
}

void DamagingTile::onHit(LevelMovableGameObject* mob) {
	if (!m_isDivine) {
		mob->setDead();
		return;
	}

	if (m_isBroken) 
		return;

	auto gos = *m_screen->getObjects(GameObjectType::_Spell);
	for (auto go : gos) {
		Spell* spell = dynamic_cast<Spell*>(go);
		if (spell && spell->getSpellID() == SpellID::DivineShield &&
			spell->getBoundingBox()->contains(mob->getCenter())) {
			m_isBroken = true;
			setState(GameObjectState::Broken);
			m_despawnTime = m_animatedSprite.getAnimation()->getAnimationTime();
			g_resourceManager->playSound(SHATTER_SOUND);
			return;
		}
	}

	mob->setDead();
}

std::string DamagingTile::getSpritePath() const {
	return "res/assets/level_dynamic_tiles/spritesheet_tiles_damaging.png";
}