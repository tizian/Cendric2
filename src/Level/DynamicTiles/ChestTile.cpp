#include "Level/DynamicTiles/ChestTile.h"
#include "Level/LevelMainCharacter.h"
#include "GameObjectComponents/InteractComponent.h"
#include "GameObjectComponents/LightComponent.h"
#include "Screens/LevelScreen.h"

using namespace std;

void ChestTile::init() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, TILE_SIZE_F));
	setSpriteOffset(sf::Vector2f(-25.f, -50.f));

	m_interactComponent = new InteractComponent(g_textProvider->getText("Chest"), this, m_mainChar);
	m_interactComponent->setInteractRange(PICKUP_RANGE);
	m_interactComponent->setInteractText("ToOpen");
	m_interactComponent->setOnInteract(std::bind(&ChestTile::onRightClick, this));
	addComponent(m_interactComponent);
}

void ChestTile::loadAnimation(int skinNr) {
	m_isCollidable = false;
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* closedAnimation = new Animation(sf::seconds(10.f));
	closedAnimation->setSpriteSheet(tex);
	closedAnimation->addFrame(sf::IntRect(0, (skinNr - 1) * 2 * TILE_SIZE, 2 * TILE_SIZE, 2 * TILE_SIZE));

	addAnimation(GameObjectState::Locked, closedAnimation);

	Animation* openAnimation = new Animation(sf::seconds(10.f));
	openAnimation->setSpriteSheet(tex);
	openAnimation->addFrame(sf::IntRect(2 * TILE_SIZE, (skinNr - 1) * 2 * TILE_SIZE, 2 * TILE_SIZE, 2 * TILE_SIZE));

	addAnimation(GameObjectState::Unlocked, openAnimation);

	// initial values
	m_state = GameObjectState::Locked;
	setCurrentAnimation(getAnimation(m_state), false);
	playCurrentAnimation(false);
}

void ChestTile::setLoot(const std::map<std::string, int>& items, int gold) {
	m_lootableItems = items;
	m_lootableGold = gold;
	delete m_lootWindow;
	m_lootWindow = nullptr;
	if (items.empty() && gold <= 0) return;
	m_lootWindow = new LootWindow();
	m_lootWindow->setLoot(items, gold);
}

void ChestTile::onHit(Spell* spell) {
	switch (spell->getSpellID()) {
	case SpellID::Unlock:
		if (m_state == GameObjectState::Locked) {
			if (spell->getStrength() >= m_strength) {
				unlock();
			}
			spell->setDisposed();
		}
		break;
	case SpellID::Telekinesis:
		if (m_state == GameObjectState::Unlocked) {
			loot();
			spell->setDisposed();
		}
		else if (m_strength == 0 && m_state == GameObjectState::Locked) {
			setState(GameObjectState::Unlocked);
			spell->setDisposed();
		}
		break;
	default:
		break;
	}
}

void ChestTile::renderAfterForeground(sf::RenderTarget& renderTarget) {
	GameObject::renderAfterForeground(renderTarget);
	if (m_showLootWindow && m_lootWindow != nullptr) {
		m_lootWindow->render(renderTarget);
		m_showLootWindow = false;
	}
}

void ChestTile::update(const sf::Time& frameTime) {
	LevelDynamicTile::update(frameTime);
	if (m_showLootWindow && m_lootWindow != nullptr) {
		sf::Vector2f pos(getBoundingBox()->left + getBoundingBox()->width, getBoundingBox()->top - m_lootWindow->getSize().y + 10.f);
		m_lootWindow->setPosition(pos);
	}
	m_showLootWindow = m_interactComponent->isInteractable() && (m_showLootWindow || g_inputController->isKeyActive(Key::ToggleTooltips));
}

void ChestTile::setChestData(const ChestData& data) {
	m_objectID = data.objectID;
	if (data.chestStrength >= 0 && data.chestStrength <= 5) {
		m_strength = data.chestStrength;
	}
	m_keyItemID = data.keyItemID;
	setLoot(data.loot.first, data.loot.second);
	m_isPermanent = data.isPermanent;
	if (data.isOpen) {
		unlock();
	}
	if (!data.tooltipText.empty()) {
		m_interactComponent->setTooltipText(g_textProvider->getText(data.tooltipText, "chest"));
	}
	if (data.lightData.radius.x > 0.f && data.lightData.radius.y > 0.f) {
		m_lightComponent = new LightComponent(data.lightData, this);
		addComponent(m_lightComponent);
	}
	m_conditionProgress = data.conditionProgress;
}

void ChestTile::onMouseOver() {
	LevelDynamicTile::onMouseOver();
	if (m_state == GameObjectState::Unlocked) {
		setSpriteColor(COLOR_INTERACTIVE, sf::milliseconds(100));
		m_showLootWindow = true;
	}
}

void ChestTile::unlock() {
	bool isObserved = dynamic_cast<LevelScreen*>(m_screen)->getWorldData()->isObserved;
	if (isObserved) {
		m_interactComponent->setInteractText("ToSteal");
		m_interactComponent->setInteractTextColor(COLOR_BAD);
	}
	else {
		m_interactComponent->setInteractText("ToPickup");
	}

	setState(GameObjectState::Unlocked);
}

void ChestTile::loot() {
	bool isObserved = dynamic_cast<LevelScreen*>(m_screen)->getWorldData()->isObserved;
	if (isObserved && dynamic_cast<LevelScreen*>(m_screen)->notifyObservers()) {
		return;
	}

	// loot, create the correct items + gold in the players inventory.
	m_mainChar->lootItems(m_lootableItems);
	if (m_lootableGold > 0) {
		m_mainChar->addGold(m_lootableGold);
	}
	if (!m_conditionProgress.first.empty() && !m_conditionProgress.second.empty()) {
		dynamic_cast<LevelScreen*>(m_screen)->notifyConditionAdded(m_conditionProgress.first, m_conditionProgress.second);
	}
	
	m_screen->getCharacterCore()->setChestLooted(m_mainChar->getLevel()->getID(), m_objectID);
	m_interactComponent->setInteractable(false);

	if (m_lightComponent != nullptr) {
		m_lightComponent->setVisible(false);
	}
	if (!m_isPermanent) {
		setDisposed();
	}
}

void ChestTile::onRightClick() {
	if (m_mainChar->isDead() || !m_interactComponent->isInteractable()) return;
	
	// check if the chest is in range
	bool inRange = dist(m_mainChar->getCenter(), getCenter()) <= PICKUP_RANGE;

	if (m_state == GameObjectState::Unlocked) {
		if (inRange) {
			loot();
		}
		else {
			m_screen->setTooltipText("OutOfRange", COLOR_BAD, true);
		}
		g_inputController->lockAction();
	}
	else if (m_strength == 0 && m_state == GameObjectState::Locked) {
		if (inRange) {
			unlock();
		}
		else {
			m_screen->setTooltipText("OutOfRange", COLOR_BAD, true);
		}
		g_inputController->lockAction();
	}
	else if (!m_keyItemID.empty() && m_screen->getCharacterCore()->hasItem(m_keyItemID, 1)) {
		if (inRange) {
			unlock();
			std::string tooltipText = g_textProvider->getText("Used");
			tooltipText.append(g_textProvider->getText(m_keyItemID, "item"));
			m_screen->setTooltipTextRaw(tooltipText, COLOR_GOOD, true);
		}
		else {
			m_screen->setTooltipText("OutOfRange", COLOR_BAD, true);
		}
		g_inputController->lockAction();
	}
	else {
		m_screen->setTooltipText("IsLocked", COLOR_BAD, true);
	}
}

std::string ChestTile::getSpritePath() const {
	return "res/assets/level_dynamic_tiles/spritesheet_tiles_chest.png";
}