#include "Level/DynamicTiles/SwingingTile.h"
#include "Spells/Spell.h"
#include "Level/LevelMainCharacter.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Swinging, SwingingTile)

const float SwingingTile::DAMAGE_RADIUS = 25.f;

SwingingTile::~SwingingTile() {
	delete m_texture;
}

bool SwingingTile::init(const LevelTileProperties& properties) {
	if (!contains(properties, std::string("size"))) return false;
	m_size = std::stoi(properties.at(std::string("size")));
	if (m_size < 2 || m_size > 100) return false;
	m_length = TILE_SIZE_F * m_size;

	if (!contains(properties, std::string("speed"))) return false;
	m_speed = std::stoi(properties.at(std::string("speed")));
	if (m_speed < 10 || m_speed > 1000) return false;

	m_isInactive = contains(properties, std::string("inactive"));

	m_currentRotation = 0.f;
	if (contains(properties, std::string("angle"))) {
		m_currentRotation = static_cast<float>(std::stoi(properties.at(std::string("angle"))));
		m_currentRotation = std::fmod(m_currentRotation, 360.f);
	}
	
	if (!contains(properties, std::string("mode"))) return false;
	std::string mode = properties.at(std::string("mode"));
	if (mode.compare("round") == 0) {
		m_mode = SwingingTileMode::Round;
	}
	else {
		m_mode = SwingingTileMode::Pendulum;
	}

	if (contains(properties, std::string("direction"))) {
		std::string mode = properties.at(std::string("direction"));
		m_isClockwise = mode.compare("ccw") != 0;
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
	texImg.copy(fullImg, 0, 0, sf::IntRect(0, TILE_SIZE * 2 * skinNr, TILE_SIZE * 3, TILE_SIZE * 2));

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

	// debug circle
	m_debugCircle.setFillColor(COLOR_TRANSPARENT);
	m_debugCircle.setOutlineColor(COLOR_BAD);
	m_debugCircle.setOutlineThickness(2.f);
	m_debugCircle.setRadius(DAMAGE_RADIUS);
	m_debugCircle.setOrigin(sf::Vector2f(DAMAGE_RADIUS, DAMAGE_RADIUS));
}

void SwingingTile::update(const sf::Time& frametime) {
	if (!m_isInactive) {
		switch (m_mode) {
		case SwingingTileMode::Round:
			animateRound(frametime);
			break;
		case SwingingTileMode::Pendulum:
		default:
			animatePendulum(frametime);
		}
	}

	m_currentRotation = modAngle(m_currentRotation);
	m_animatedSprite.setRotation(m_currentRotation + 180.f);
	m_debugCircle.setPosition(getHeadPosition());
	LevelDynamicTile::update(frametime);
}

void SwingingTile::animatePendulum(const sf::Time& frametime) {
	float rad = degToRad(m_currentRotation);
	float acc = m_speed / m_length * std::sin(rad);
	m_pendulumVelocity += acc * frametime.asSeconds();
	rad += m_pendulumVelocity * frametime.asSeconds();
	m_currentRotation = radToDeg(rad);
}

void SwingingTile::animateRound(const sf::Time& frametime) {
	float diff = frametime.asSeconds() * m_speed;
	m_currentRotation += (m_isClockwise ? 1 : -1) * diff;
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

void SwingingTile::renderAfterForeground(sf::RenderTarget& target) {
	LevelDynamicTile::renderAfterForeground(target);
	if (m_isDebugRendering) {
		target.draw(m_debugCircle);
	}
}

void SwingingTile::onHit(LevelMovableGameObject* mob) {
	if (mob->getConfiguredType() != GameObjectType::_LevelMainCharacter)  return;

	// check AABB circle collision
	auto const& bb = *mob->getBoundingBox();
	auto const& pt = m_debugCircle.getPosition();
	sf::Vector2f pc = pt;
	if (pc.x > bb.left + bb.width) pc.x = bb.left + bb.width;
	if (pc.x < bb.left) pc.x = bb.left;
	if (pc.y > bb.top + bb.height) pc.y = bb.top + bb.height;
	if (pc.y < bb.top) pc.y = bb.top;

	if (dist(pc, pt) < DAMAGE_RADIUS) {
		mob->setDead();
	}
}

sf::Vector2f SwingingTile::getHeadPosition() const {
	float rad = degToRad(m_currentRotation);
	sf::Vector2f rotation(m_length * std::sin(rad), m_length * -std::cos(rad));
	float offset = m_length + TILE_SIZE_F;
	return getPosition() + sf::Vector2f(offset, offset) + rotation;
}

std::string SwingingTile::getSpritePath() const {
	return "res/texture/level_dynamic_tiles/spritesheet_tiles_swinging.png";
}
