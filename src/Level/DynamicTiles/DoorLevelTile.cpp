#include "Level/DynamicTiles/DoorLevelTile.h"
#include "GameObjectComponents/InteractComponent.h"
#include "Level/LevelMainCharacter.h"
#include "World/Trigger.h"
#include "Spells/Spell.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Door, DoorLevelTile)

const float DoorLevelTile::OPEN_RANGE = 100.f;

bool DoorLevelTile::init(const LevelTileProperties& properties) {
	m_isInitiallyCollidable = contains(properties, std::string("collidable"));

	if (contains(properties, std::string("key"))) {
		m_keyItemID = properties.at("key");
	}

	if (!contains(properties, std::string("width"))) return false;
	int m_tileWidth = std::stoi(properties.at(std::string("width")));
	if (m_tileWidth < 1 || m_tileWidth > 2) return false;

	if (!contains(properties, std::string("strength"))) return false;
	int m_strength = std::stoi(properties.at(std::string("strength")));
	if (m_strength < 0 || m_strength > 4) return false;

	if (m_strength == 0 && !m_keyItemID.empty()) return false;

	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F * m_tileWidth, 3 * TILE_SIZE_F));

	m_interactComponent = new InteractComponent(g_textProvider->getText("Door"), this, m_mainChar);
	m_interactComponent->setInteractRange(OPEN_RANGE);
	m_interactComponent->setInteractText("ToOpen");
	m_interactComponent->setOnInteract(std::bind(&DoorLevelTile::onRightClick, this));
	addComponent(m_interactComponent);

	return true;
}

void DoorLevelTile::loadAnimation(int skinNr) {
	m_isCollidable = true;
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* closedAnimation = new Animation();
	closedAnimation->setSpriteSheet(tex);
	closedAnimation->addFrame(sf::IntRect(0, skinNr * 3 * TILE_SIZE, TILE_SIZE * m_tileWidth, 3 * TILE_SIZE));

	addAnimation(GameObjectState::Closed, closedAnimation);

	Animation* openAnimation = new Animation();
	openAnimation->setSpriteSheet(tex);
	openAnimation->addFrame(sf::IntRect(TILE_SIZE * m_tileWidth, skinNr * 3 * TILE_SIZE, TILE_SIZE * m_tileWidth, 3 * TILE_SIZE));

	addAnimation(GameObjectState::Open, openAnimation);
}

void DoorLevelTile::update(const sf::Time& frameTime) {
	if (!m_isInitialized) {
		close();
		playCurrentAnimation(false);
		m_isInitialized = true;
	}

	LevelDynamicTile::update(frameTime);
}

void DoorLevelTile::onHit(Spell* spell) {
	if (m_isOpen || spell->getSpellID() != SpellID::Unlock) {
		return;
	}
	
	if (spell->getStrength() >= m_strength) {
		open();
		g_resourceManager->playSound(GlobalResource::SOUND_MISC_UNLOCK);
	}
	else {
		if (m_strength > 4) {
			m_screen->setNegativeTooltip("IsLockedKey");
		}
		else {
			m_screen->setNegativeTooltip("NotEnoughStrength");
		}
	}
	spell->setDisposed();
}

void DoorLevelTile::onRightClick() {
	if (m_isOpen) return;

	// check if the door is in range
	bool inRange = dist(m_mainChar->getCenter(), getCenter()) <= OPEN_RANGE;

	if (!inRange) {
		m_screen->setNegativeTooltip("OutOfRange");
	}
	else if (m_strength == 0) {
		open();
	}
	else if (!m_keyItemID.empty() && m_screen->getCharacterCore()->hasItem(m_keyItemID, 1)) {
		open();
		g_resourceManager->playSound(GlobalResource::SOUND_MISC_UNLOCK);

		std::string tooltipText = g_textProvider->getText("Used");
		tooltipText.append(g_textProvider->getText(m_keyItemID, "item"));
		m_screen->setTooltipTextRaw(tooltipText, COLOR_GOOD, true);

		g_inputController->lockAction();
	}
	else {
		if (!m_keyItemID.empty() && m_strength < 5) {
			m_screen->setNegativeTooltip("IsLockedKeyPicklock");
		}
		else if (!m_keyItemID.empty()) {
			m_screen->setNegativeTooltip("IsLockedKey");
		}
		else if (m_strength < 5) {
			m_screen->setNegativeTooltip("IsLockedPicklock");
		}
	}
}

void DoorLevelTile::open() {
	m_isOpen = true;
	m_isCollidable = false;
	m_isStrictlyCollidable = false;
	setState(GameObjectState::Open);
	m_interactComponent->setInteractable(false);

	// unlock keyguarded triggers (which are level exits)
	for (auto& it : *m_screen->getObjects(GameObjectType::_Overlay)) {
		if (Trigger* trigger = dynamic_cast<Trigger*>(it)) {
			if (trigger->getData().isKeyGuarded && m_boundingBox.intersects(*trigger->getBoundingBox())) {
				trigger->getData().isTriggerable = true;
			}
		}
	}
}

void DoorLevelTile::close() {
	m_isOpen = false;
	m_isCollidable = m_isInitiallyCollidable;
	m_isStrictlyCollidable = m_isInitiallyCollidable;
	setState(GameObjectState::Closed);
	m_interactComponent->setInteractable(true);

	// lock keyguarded triggers (which are level exits)
	for (auto& it : *m_screen->getObjects(GameObjectType::_Overlay)) {
		if (Trigger* trigger = dynamic_cast<Trigger*>(it)) {
			if (trigger->getData().isKeyGuarded && m_boundingBox.intersects(*trigger->getBoundingBox())) {
				trigger->getData().isTriggerable = false;
			}
		}
	}
}

std::string DoorLevelTile::getSpritePath() const {
	return "res/assets/level_dynamic_tiles/spritesheet_tiles_door.png";
}
