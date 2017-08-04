#include "Level/DynamicTiles/SwingingTile.h"
#include "Spells/Spell.h"
#include "Level/LevelMainCharacter.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Swinging, SwingingTile)

SwingingTile::~SwingingTile() {
	delete m_texture;
}

bool SwingingTile::init(const LevelTileProperties& properties) {
	if (!contains(properties, std::string("size"))) return false;
	m_size = std::stoi(properties.at(std::string("size")));
	if (m_size < 2 || m_size > 100) return false;

	if (!contains(properties, std::string("speed"))) return false;
	m_speed = std::stoi(properties.at(std::string("speed")));
	if (m_speed < 0 || m_speed > 1000) return false;

	if (!contains(properties, std::string("mode"))) return false;
	std::string mode = properties.at(std::string("mode"));
	if (mode.compare("round")) {
		m_mode = SwingingTileMode::Round;
	}
	else {
		m_mode = SwingingTileMode::Ease;
	}

	setBoundingBox(sf::FloatRect(0.f, 0.f, 2 * TILE_SIZE_F * (m_size + 1), 2 * TILE_SIZE_F * (m_size + 1)));
	setSpriteOffset(sf::Vector2f(TILE_SIZE_F * (m_size - 0.5f), TILE_SIZE_F * (m_size + 0.5f)));
	setPositionOffset(sf::Vector2f(-TILE_SIZE_F * (m_size + 0.5f), -TILE_SIZE_F * (m_size + 0.5f)));

	return true;
}

void SwingingTile::loadAnimation(int skinNr) {
	// a swinging tile creates its own texture out of several images.

	sf::Image img;
	img.create(TILE_SIZE * 3, (m_size + 1) * TILE_SIZE, COLOR_TRANSPARENT);

	sf::Image fullImg;
	fullImg.loadFromFile(getSpritePath());

	sf::Image texImg;
	texImg.create(TILE_SIZE * 3, 2 * TILE_SIZE, COLOR_TRANSPARENT);
	texImg.copy(fullImg, 0, TILE_SIZE * 2 * skinNr, sf::IntRect(0, 0, TILE_SIZE * 3, TILE_SIZE * 2));

	int length = m_size;

	// that's the part below, the blade
	img.copy(texImg, 0, TILE_SIZE * length, sf::IntRect(0, TILE_SIZE, TILE_SIZE * 3, TILE_SIZE));
	length--;

	// that's the thing that holds on to the blade
	img.copy(texImg, TILE_SIZE, TILE_SIZE * length, sf::IntRect(TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
	length--;

	// here comes the thingys in between, usually a chain.
	while (length > 0) {
		img.copy(texImg, TILE_SIZE, TILE_SIZE * length, sf::IntRect(TILE_SIZE * 2, 0, TILE_SIZE, TILE_SIZE));
		length--;
	}

	// that's the uppermost thingy that holds to the ceiling.
	img.copy(texImg, TILE_SIZE, TILE_SIZE * length, sf::IntRect(0, 0, TILE_SIZE, TILE_SIZE));

	// now cconvert to texture format
	delete m_texture;
	m_texture = new sf::Texture();
	m_texture->loadFromImage(img);

	// and load the sprite
	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(m_texture);
	idleAnimation->addFrame(sf::IntRect(0, 0, img.getSize().x, img.getSize().y));

	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(false);
}

void SwingingTile::update(const sf::Time& frametime) {
	m_currentRotation += (frametime.asSeconds() * 100);
	if (m_currentRotation > 360.f) {
		m_currentRotation -= 360.f;
	}
	m_animatedSprite.setRotation(m_currentRotation);
	LevelDynamicTile::update(frametime);
}

void SwingingTile::switchTile() {
	m_isInactive = !m_isInactive;
}

bool SwingingTile::isSwitchable() const {
	return true;
}

void SwingingTile::setInitialState(bool on) {
	m_isInactive = !on;
}

void SwingingTile::render(sf::RenderTarget& target) {
	LevelDynamicTile::render(target);
}

void SwingingTile::onHit(LevelMovableGameObject* mob) {
	if (mob->getConfiguredType() != GameObjectType::_LevelMainCharacter)  return;

}

void SwingingTile::setPosition(const sf::Vector2f& position) {
	LevelDynamicTile::setPosition(position);
}

std::string SwingingTile::getSpritePath() const {
	return "res/assets/level_dynamic_tiles/spritesheet_tiles_swinging.png";
}