#include "Level/DynamicTiles/MirrorTile.h"
#include "Spells/Spell.h"
#include "Level/LevelMainCharacter.h"
#include "GameObjectComponents/InteractComponent.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Lever, LeverTile)

const float MirrorTile::ACTIVATE_RANGE = 80.f;
const float MirrorTile::TICK_ANGLE = 15.f;

MirrorTile::MirrorTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {
	m_interactComponent = new InteractComponent(g_textProvider->getText("Lever"), this, m_mainChar);
	m_interactComponent->setInteractRange(ACTIVATE_RANGE);
	m_interactComponent->setInteractText("ToSwitch");
	m_interactComponent->setOnInteract(std::bind(&MirrorTile::switchLever, this));
	addComponent(m_interactComponent);
}

bool MirrorTile::init(const LevelTileProperties& properties) {
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, TILE_SIZE_F));

	m_currentRotation = 0.f;
	if (contains(properties, std::string("angle"))) {
		m_currentRotation = static_cast<float>(std::stoi(properties.at(std::string("angle"))));
		m_currentRotation = std::fmod(m_currentRotation, 360.f);
	}

	return true;
}

void MirrorTile::loadAnimation(int skinNr) {
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* defaultAnimation = new Animation(sf::seconds(10.0f));
	defaultAnimation->setSpriteSheet(tex);
	defaultAnimation->addFrame(sf::IntRect(0, skinNr * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Active, defaultAnimation);

	// initial values
	setState(GameObjectState::Active);
	playCurrentAnimation(false);

	// set origin of animation to center
	m_animatedSprite.setOrigin(sf::Vector2f(TILE_SIZE_F * 0.5f, TILE_SIZE_F * 0.5f));
}

void MirrorTile::onHit(Spell* spell) {
	switch (spell->getSpellID()) {
	case SpellID::Telekinesis:
		switchLever();
		spell->setDisposed();
		break;
	default:
		break;
	}
}

void MirrorTile::onRightClick() {
	if (m_mainChar->isDead()) return;
	// check if lever is in range
	if (dist(m_mainChar->getCenter(), getCenter()) <= ACTIVATE_RANGE) {
		switchLever();
	}
	else {
		m_screen->setNegativeTooltip("OutOfRange");
	}
	g_inputController->lockAction();
}

void MirrorTile::switchLever() {
	setRotation(m_currentRotation + TICK_ANGLE);
}

void MirrorTile::setRotation(float rotation) {
	m_currentRotation = std::fmod(rotation, 360.f);
	m_animatedSprite.setRotation(m_currentRotation);
}

std::string MirrorTile::getSpritePath() const {
	return "res/texture/level_dynamic_tiles/spritesheet_tiles_mirror.png";
}