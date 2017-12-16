#include "Spells/ShackleSpell.h"
#include "Level/LevelMainCharacter.h"
#include "Level/Level.h"
#include "GameObjectComponents/LightComponent.h"
#include "World/CustomParticleUpdaters.h"
#include "Screens/LevelScreen.h"
#include "ObjectFactory.h"

ShackleSpell::ShackleSpell() : Spell() {
}

void ShackleSpell::load(const SpellData& data, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	Spell::load(data, mob, target);
	loadComponents();
	m_isAlwaysUpdate = true;
	m_mainChar->setStunned(m_data.duration);
	m_level = mob->getLevel();

	// set times
	m_timeUntilShackleStart = sf::seconds(1.f);
	m_timeShackleOut = sf::seconds(2.f);
	m_timeShackleIn = sf::seconds(2.f);
	m_timeShackleDone = sf::seconds(2.f);

	setSpriteOffset(sf::Vector2f(-20.f, -20.f));

	// load animation
	const sf::Texture* tex = g_resourceManager->getTexture(data.spritesheetPath);
	Animation* spellAnimation = new Animation();
	spellAnimation->setSpriteSheet(tex);
	spellAnimation->addFrame(sf::IntRect(0, 50, 50, 50));
	spellAnimation->setLooped(false);

	addAnimation(GameObjectState::Idle, spellAnimation);

	setState(GameObjectState::Idle);
}

void ShackleSpell::update(const sf::Time& frameTime) {
	Spell::update(frameTime);
	setPosition(m_mainChar->getPosition() + sf::Vector2f(m_mainChar->getBoundingBox()->width * 0.5f, -40.f));
	 
	if (m_timeUntilShackleStart > sf::Time::Zero) {
		updateTime(m_timeUntilShackleStart, frameTime);
		float brightness = (2.f - m_timeUntilShackleStart.asSeconds()) / 2.f;
		setSpriteColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(brightness * 255)), sf::seconds(1.f));
		m_lightComponent->setBrightness(brightness);

		if (m_timeUntilShackleStart == sf::Time::Zero) {
			// load sprites
			for (int i = 0; i < 4; i++) {
				ShackleSprite* sprite = new ShackleSprite();
				sprite->load(i * M_PI * 0.5f, m_mob->getCenter());
				m_screen->addObject(sprite);
				m_shackleSprites.push_back(sprite);
			}
		}
		return;
	}

	if (m_timeShackleOut > sf::Time::Zero) {
		updateTime(m_timeShackleOut, frameTime);
		return;
	}

	if (m_timeShackleIn > sf::Time::Zero) {
		updateTime(m_timeShackleIn, frameTime);
		m_currentDimming = std::min(1.f, 1.f - (m_timeShackleIn.asSeconds() / 2.f));
		m_level->setAmbientDimming(0.5f + m_currentDimming * 0.5f);
		m_level->setLightDimming(m_currentDimming);

		for (size_t i = 0; i < m_shackleSprites.size(); /* don't increment here, we remove on the fly */) {
			m_shackleSprites[i]->gotoMainchar(m_mainChar);
			if (m_shackleSprites[i]->isDisposed()) {
				m_shackleSprites.erase(m_shackleSprites.begin() + i);
			}
			else {
				i++;
			}
		}

		if (m_timeShackleIn == sf::Time::Zero) {
			// spawn blocks and set main char
			if (m_data.strength == 2) {
				m_mainChar->setPosition(sf::Vector2f(660.f, 510.f));
			}
			else {
				m_mainChar->setPosition(sf::Vector2f(1230.f, 560.f));
			}
			m_mainChar->setFacingRight(false);
			LevelTileProperties properties;

			for (int i = 0; i < 3; ++i) {
				auto pos = sf::Vector2f(1150.f, 500.f + i * TILE_SIZE_F);
				LevelDynamicTile* tile = ObjectFactory::Instance()->createLevelDynamicTile(LevelDynamicTileID::Destructible, dynamic_cast<LevelScreen*>(m_screen));
				tile->init(properties);
				tile->setPosition(pos);
				tile->loadResources();
				tile->loadAnimation(0);
				tile->setPosition(pos);
				tile->setDebugBoundingBox(COLOR_NEUTRAL);
				m_screen->addObject(tile);
			}
		}
		return;
	}

	updateTime(m_timeShackleDone, frameTime);
	m_currentDimming = std::min(1.f, m_timeShackleDone.asSeconds() / 2.f);
	m_level->setAmbientDimming(0.5f + m_currentDimming * 0.5f);
	m_level->setLightDimming(m_currentDimming);

	if (m_timeShackleDone == sf::Time::Zero) {
		setDisposed();
		for (auto shackle : m_shackleSprites) {
			shackle->setDisposed();
		}
	}
}

bool ShackleSpell::getConfiguredRotateSprite() const {
	return false;
}

void ShackleSpell::loadComponents() {
	// light
	LightData lightData(sf::Vector2f(m_boundingBox.height * 0.5f, m_boundingBox.height * 0.5f), 200.f, 0.f);
	m_lightComponent = new LightComponent(lightData, this);
	addComponent(m_lightComponent);
}

//////////// SHACKLE SPRITE /////////////////

const float ShackleSprite::SPEED = 500.f;

void ShackleSprite::gotoMainchar(GameObject* mainChar) {
	sf::Vector2f dir = mainChar->getCenter() - getPosition();
	float abs = norm(dir);
	dir = dir / abs;
	m_velocity = dir * SPEED * 2.f;
	setSpriteRotation(atan2(getVelocity().y, getVelocity().x) - M_PI * 0.5f);

	if (fastIntersect(*mainChar->getBoundingBox(), m_boundingBox)) {
		setDisposed();
	}
}

void ShackleSprite::load(float angle, const sf::Vector2f& pos) {
	m_isAlwaysUpdate = true;
	m_boundingBox = sf::FloatRect(0.f, 0.f, 50.f, 50.f);

	const sf::Texture* tex = g_resourceManager->getTexture("res/texture/spells/spritesheet_spell_shackle.png");
	Animation* spellAnimation = new Animation();
	spellAnimation->setSpriteSheet(tex);
	spellAnimation->addFrame(sf::IntRect(0, 0, 50, 50));
	spellAnimation->setLooped(false);

	addAnimation(GameObjectState::Idle, spellAnimation);

	setState(GameObjectState::Idle);

	setSpriteRotation(angle - M_PI * 0.5f);

	m_velocity.x = std::round(SPEED * std::cos(angle));
	m_velocity.y = std::round(SPEED * std::sin(angle));

	setPosition(pos);
}
