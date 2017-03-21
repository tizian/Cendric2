#include "Level/DynamicTiles/DoorLevelTile.h"
#include "GameObjectComponents/InteractComponent.h"
#include "Level/LevelMainCharacter.h"
#include "Trigger.h"
#include "Spells/Spell.h"

const float DoorLevelTile::OPEN_RANGE = 100.f;

void DoorLevelTile::init() {
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F * m_doorData.tileWidth, 3 * TILE_SIZE_F));

	m_interactComponent = new InteractComponent(g_textProvider->getText("Door"), this, m_mainChar);
	m_interactComponent->setInteractRange(OPEN_RANGE);
	m_interactComponent->setInteractText("ToOpen");
	m_interactComponent->setOnInteract(std::bind(&DoorLevelTile::onRightClick, this));
	addComponent(m_interactComponent);
}

void DoorLevelTile::loadAnimation(int skinNr) {
	m_isCollidable = true;
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* closedAnimation = new Animation();
	closedAnimation->setSpriteSheet(tex);
	closedAnimation->addFrame(sf::IntRect(0, skinNr * 3 * TILE_SIZE, TILE_SIZE * m_doorData.tileWidth, 3 * TILE_SIZE));

	addAnimation(GameObjectState::Closed, closedAnimation);

	Animation* openAnimation = new Animation();
	openAnimation->setSpriteSheet(tex);
	openAnimation->addFrame(sf::IntRect(TILE_SIZE * m_doorData.tileWidth, skinNr * 3 * TILE_SIZE, TILE_SIZE * m_doorData.tileWidth, 3 * TILE_SIZE));

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
	
	if (spell->getStrength() >= m_doorData.strength) {
		open();
		g_resourceManager->playSound(GlobalResource::SOUND_MISC_UNLOCK);
	}
	else {
		if (m_doorData.strength > 4) {
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
	else if (m_doorData.strength == 0) {
		open();
	}
	else if (!m_doorData.keyItemID.empty() && m_screen->getCharacterCore()->hasItem(m_doorData.keyItemID, 1)) {
		open();
		g_resourceManager->playSound(GlobalResource::SOUND_MISC_UNLOCK);

		std::string tooltipText = g_textProvider->getText("Used");
		tooltipText.append(g_textProvider->getText(m_doorData.keyItemID, "item"));
		m_screen->setTooltipTextRaw(tooltipText, COLOR_GOOD, true);

		g_inputController->lockAction();
	}
	else {
		if (!m_doorData.keyItemID.empty() && m_doorData.strength < 5) {
			m_screen->setNegativeTooltip("IsLockedKeyPicklock");
		}
		else if (!m_doorData.keyItemID.empty()) {
			m_screen->setNegativeTooltip("IsLockedKey");
		}
		else if (m_doorData.strength < 5) {
			m_screen->setNegativeTooltip("Picklock");
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
	m_isCollidable = m_doorData.isCollidable;
	m_isStrictlyCollidable = m_doorData.isCollidable;
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

void DoorLevelTile::setDoorData(const DoorData& data) {
	m_doorData = data;
}

std::string DoorLevelTile::getSpritePath() const {
	return "res/assets/level_dynamic_tiles/spritesheet_tiles_door.png";
}
