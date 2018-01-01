#include "Level/DynamicTiles/DamagingTile.h"
#include "GameObjectComponents/LightComponent.h"
#include "Spells/Spell.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Damaging, DamagingTile)

const std::string DamagingTile::SHATTER_SOUND = "res/sound/mob/yashaadd_death.ogg";
const float DamagingTile::DAMAGE_RADIUS = 20.f;

bool DamagingTile::init(const LevelTileProperties& properties) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, TILE_SIZE_F));
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

	// debug circle
	m_debugCircle.setFillColor(COLOR_TRANSPARENT);
	m_debugCircle.setOutlineColor(COLOR_BAD);
	m_debugCircle.setOutlineThickness(2.f);
	m_debugCircle.setRadius(DAMAGE_RADIUS);
	m_debugCircle.setOrigin(sf::Vector2f(DAMAGE_RADIUS, DAMAGE_RADIUS));
	float circleRadius = DAMAGE_RADIUS + (m_boundingBox.width * 0.5f - DAMAGE_RADIUS);
	m_circleOffset = sf::Vector2f(circleRadius, circleRadius);
}

void DamagingTile::renderAfterForeground(sf::RenderTarget& target) {
	LevelDynamicTile::renderAfterForeground(target);
	if (m_isDebugRendering) {
		target.draw(m_debugCircle);
	}
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

void DamagingTile::setPosition(const sf::Vector2f& position) {
	LevelDynamicTile::setPosition(position);
	m_debugCircle.setPosition(position + m_circleOffset);
}

void DamagingTile::onHit(LevelMovableGameObject* mob) {
	if (mob->getConfiguredType() != GameObjectType::_LevelMainCharacter)  return;
	if (!fastIntersectCircle(*mob->getBoundingBox(), m_debugCircle.getPosition(), DAMAGE_RADIUS)) {
		return;
	}

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
	return "res/texture/level_dynamic_tiles/spritesheet_tiles_damaging.png";
}