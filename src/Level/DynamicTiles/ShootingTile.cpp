#include "Level/DynamicTiles/ShootingTile.h"
#include "Spells/Spell.h"
#include "Registrar.h"
#include "Screens/WorldScreen.h"
#include "SpellCreators/ProjectileSpellCreator.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Shooting, ShootingTile)

ShootingTile::ShootingTile(LevelScreen* levelScreen) :
	LevelDynamicTile(levelScreen) {
}

void ShootingTile::init() {
	setSpriteOffset(sf::Vector2f(-25.f, 0.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, TILE_SIZE_F));
}

void ShootingTile::loadSpells(int skinNr) {
	switch (skinNr) {
	case 1:
	default:
	{
		m_spellData = SpellData::getSpellData(SpellID::Projectile);
		m_activeTime = sf::seconds(1.f);
		m_cooldown = sf::seconds(5.f);
		break;
	}
	}
}

void ShootingTile::loadAnimation(int skinNr) {
	m_isCollidable = true;
	loadSpells(skinNr);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_shooting));
	idleAnimation->addFrame(sf::IntRect(0, (skinNr - 1) * TILE_SIZE, TILE_SIZE * 2, TILE_SIZE));


	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* activeAnimation = new Animation();
	activeAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_shooting));
	for (int i = 1; i < 4; ++i) {
		activeAnimation->addFrame(sf::IntRect(TILE_SIZE * 2 * i, (skinNr - 1) * TILE_SIZE, TILE_SIZE * 2, TILE_SIZE));
	}

	addAnimation(GameObjectState::Active, activeAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

void ShootingTile::update(const sf::Time& frameTime) {
	LevelDynamicTile::update(frameTime);

}

void ShootingTile::onHit(Spell* spell) {
	// nop, yet
}


