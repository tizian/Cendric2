#include "Level/DynamicTiles/ChestLevelTile.h"
#include "Level/LevelMainCharacter.h"
#include "GameObjectComponents/InteractComponent.h"
#include "GameObjectComponents/LightComponent.h"
#include "Screens/LevelScreen.h"
#include "Callbacks/WorldCallback.h"

using namespace std;
using namespace luabridge;

ChestLevelTile::~ChestLevelTile() { 
	delete m_worldCallback; 
}

void ChestLevelTile::init() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, TILE_SIZE_F));
	setSpriteOffset(sf::Vector2f(-25.f, -50.f));

	m_interactComponent = new InteractComponent(g_textProvider->getText("Chest"), this, m_mainChar);
	m_interactComponent->setInteractRange(PICKUP_RANGE);
	m_interactComponent->setInteractText("ToOpen");
	m_interactComponent->setOnInteract(std::bind(&ChestLevelTile::onRightClick, this));
	addComponent(m_interactComponent);
}

void ChestLevelTile::loadAnimation(int skinNr) {
	m_isCollidable = false;
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* closedAnimation = new Animation(sf::seconds(10.f));
	closedAnimation->setSpriteSheet(tex);
	closedAnimation->addFrame(sf::IntRect(0, skinNr * 2 * TILE_SIZE, 2 * TILE_SIZE, 2 * TILE_SIZE));

	addAnimation(GameObjectState::Locked, closedAnimation);

	Animation* openAnimation = new Animation(sf::seconds(10.f));
	openAnimation->setSpriteSheet(tex);
	openAnimation->addFrame(sf::IntRect(2 * TILE_SIZE, skinNr * 2 * TILE_SIZE, 2 * TILE_SIZE, 2 * TILE_SIZE));

	addAnimation(GameObjectState::Unlocked, openAnimation);

	// initial values
	m_state = GameObjectState::Locked;
	setCurrentAnimation(getAnimation(m_state), false);
	playCurrentAnimation(false);
}

bool ChestLevelTile::loadLua() {
	if (m_data.luapath.empty()) {
		return true;
	}
	m_L = luaL_newstate();
	luaL_openlibs(m_L);
	delete m_worldCallback;
	m_worldCallback = new WorldCallback(dynamic_cast<WorldScreen*>(m_screen));
	m_worldCallback->bindFunctions(m_L);

	if (luaL_dofile(m_L, getResourcePath(m_data.luapath).c_str()) != 0) {
		g_logger->logError("ChestLevelTile", "Cannot read lua script: " + getResourcePath(m_data.luapath));
		m_L = nullptr;
		delete m_worldCallback;
		m_worldCallback = nullptr;
		return false;
	}

	return true;
}

void ChestLevelTile::setLoot(const std::map<std::string, int>& items, int gold) {
	delete m_lootWindow;
	m_lootWindow = nullptr;
	if (items.empty() && gold <= 0) return;
	m_lootWindow = new LootWindow();
	m_lootWindow->setLoot(items, gold);
}

void ChestLevelTile::onHit(Spell* spell) {
	switch (spell->getSpellID()) {
	case SpellID::Unlock:
		if (m_state == GameObjectState::Locked) {
			if (spell->getStrength() >= m_data.chestStrength) {
				unlock(true);
			}
			else {
				if (m_data.chestStrength > 4) {
					m_screen->setNegativeTooltip("IsLockedKey");
				}
				else {
					m_screen->setNegativeTooltip("NotEnoughStrength");
				}
			}
			spell->setDisposed();
		}
		break;
	case SpellID::Telekinesis:
		if (m_state == GameObjectState::Unlocked) {
			loot();
			spell->setDisposed();
		}
		else if (m_data.chestStrength == 0 && m_state == GameObjectState::Locked) {
			setState(GameObjectState::Unlocked);
			spell->setDisposed();
		}
		break;
	default:
		break;
	}
}

void ChestLevelTile::renderAfterForeground(sf::RenderTarget& renderTarget) {
	GameObject::renderAfterForeground(renderTarget);
	if (m_showLootWindow && m_lootWindow != nullptr) {
		m_lootWindow->render(renderTarget);
		m_showLootWindow = false;
	}
}

void ChestLevelTile::update(const sf::Time& frameTime) {
	LevelDynamicTile::update(frameTime);
	if (m_showLootWindow && m_lootWindow != nullptr) {
		sf::Vector2f pos(getBoundingBox()->left + getBoundingBox()->width, getBoundingBox()->top - m_lootWindow->getSize().y + 10.f);
		m_lootWindow->setPosition(pos);
	}
	m_showLootWindow = m_interactComponent->isInteractable() && (m_showLootWindow || g_inputController->isKeyActive(Key::ToggleTooltips));
}

void ChestLevelTile::setChestData(const ChestTileData& data) {
	m_data = data;
	if (m_data.chestStrength < 0 || m_data.chestStrength > 5) {
		m_data.chestStrength = 0;
	}

	setLoot(data.loot.first, data.loot.second);

	if (data.isOpen) {
		unlock(false);
	}
	if (!data.tooltipText.empty()) {
		m_interactComponent->setTooltipText(g_textProvider->getText(data.tooltipText, "chest"));
	}
	if (data.lightData.radius.x > 0.f && data.lightData.radius.y > 0.f) {
		m_lightComponent = new LightComponent(data.lightData, this);
		addComponent(m_lightComponent);
	}

	loadLua();
}

void ChestLevelTile::onMouseOver() {
	LevelDynamicTile::onMouseOver();
	if (m_state == GameObjectState::Unlocked) {
		setSpriteColor(COLOR_INTERACTIVE, sf::milliseconds(100));
		m_showLootWindow = true;
	}
}

void ChestLevelTile::unlock(bool soundOn) {
	bool isObserved = dynamic_cast<LevelScreen*>(m_screen)->getWorldData()->isObserved;
	if (isObserved) {
		m_interactComponent->setInteractText("ToSteal");
		m_interactComponent->setInteractTextColor(COLOR_BAD);
	}
	else {
		m_interactComponent->setInteractText("ToPickup");
	}

	if (soundOn) g_resourceManager->playSound(GlobalResource::SOUND_MISC_UNLOCK);
	setState(GameObjectState::Unlocked);
}

void ChestLevelTile::loot() {
	bool isObserved = dynamic_cast<LevelScreen*>(m_screen)->getWorldData()->isObserved;
	if (isObserved && dynamic_cast<LevelScreen*>(m_screen)->notifyObservers()) {
		return;
	}

	// loot, create the correct items + gold in the players inventory.
	m_mainChar->lootItems(m_data.loot.first);
	m_mainChar->addGold(m_data.loot.second);

	// execute the script if it exists
	executeOnLoot();

	m_screen->getCharacterCore()->setChestLooted(m_mainChar->getLevel()->getID(), m_data.objectID);
	m_interactComponent->setInteractable(false);

	if (m_lightComponent != nullptr) {
		m_lightComponent->setVisible(false);
	}
	if (!m_data.isPermanent) {
		setDisposed();
	}
}

void ChestLevelTile::executeOnLoot() const {
	if (m_L == nullptr) return;
	LuaRef onLoot = getGlobal(m_L, "onLoot");
	if (!onLoot.isFunction()) {
		return;
	}

	try {
		onLoot(m_worldCallback);
	}
	catch (LuaException const& e) {
		g_logger->logError("ChestLevelTile", "LuaException: " + std::string(e.what()));
	}
}

void ChestLevelTile::onRightClick() {
	if (m_mainChar->isDead() || !m_interactComponent->isInteractable()) return;

	// check if the chest is in range
	bool inRange = dist(m_mainChar->getCenter(), getCenter()) <= PICKUP_RANGE;

	if (m_state == GameObjectState::Unlocked) {
		if (inRange) {
			loot();
		}
		else {
			m_screen->setNegativeTooltip("OutOfRange");
		}
		g_inputController->lockAction();
	}
	else if (m_data.chestStrength == 0 && m_state == GameObjectState::Locked) {
		if (inRange) {
			unlock(true);
		}
		else {
			m_screen->setNegativeTooltip("OutOfRange");
		}
		g_inputController->lockAction();
	}
	else if (!m_data.keyItemID.empty() && m_screen->getCharacterCore()->hasItem(m_data.keyItemID, 1)) {
		if (inRange) {
			unlock(true);
			std::string tooltipText = g_textProvider->getText("Used");
			tooltipText.append(g_textProvider->getText(m_data.keyItemID, "item"));
			m_screen->setTooltipTextRaw(tooltipText, COLOR_GOOD, true);
		}
		else {
			m_screen->setNegativeTooltip("OutOfRange");
		}
		g_inputController->lockAction();
	}
	else {
		if (!m_data.keyItemID.empty() && m_data.chestStrength < 5) {
			m_screen->setNegativeTooltip("IsLockedKeyPicklock");
		}
		else if (!m_data.keyItemID.empty()) {
			m_screen->setNegativeTooltip("IsLockedKey");
		}
		else if (m_data.chestStrength < 5) {
			m_screen->setNegativeTooltip("IsLockedPicklock");
		}
	}
}

std::string ChestLevelTile::getSpritePath() const {
	return "res/assets/level_dynamic_tiles/spritesheet_tiles_chest.png";
}