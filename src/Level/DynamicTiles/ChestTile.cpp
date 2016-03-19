#include "Level/DynamicTiles/ChestTile.h"
#include "Level/LevelMainCharacter.h"
#include "Spells/UnlockSpell.h"

using namespace std;

// this tile is special and is not registered
ChestTile::ChestTile(LevelMainCharacter* mainChar, Level* level) : LevelDynamicTile(level) {
	m_mainChar = mainChar;
}

void ChestTile::init() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, TILE_SIZE_F));
	setSpriteOffset(sf::Vector2f(-25.f, -50.f));
}

void ChestTile::loadAnimation(int skinNr) {
	m_isCollidable = false;

	Animation* closedAnimation = new Animation(sf::seconds(10.f));
	closedAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_chest));
	closedAnimation->addFrame(sf::IntRect(0, (skinNr - 1) * 2 * TILE_SIZE, 2 * TILE_SIZE, 2 * TILE_SIZE));

	addAnimation(GameObjectState::Locked, closedAnimation);

	Animation* openAnimation = new Animation(sf::seconds(10.f));
	openAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_chest));
	openAnimation->addFrame(sf::IntRect(2 * TILE_SIZE, (skinNr - 1) * 2 * TILE_SIZE, 2 * TILE_SIZE, 2 * TILE_SIZE));

	addAnimation(GameObjectState::Unlocked, openAnimation);

	// initial values
	m_state = GameObjectState::Locked;
	setCurrentAnimation(getAnimation(m_state), false);
	playCurrentAnimation(false);
}

void ChestTile::setLoot(const std::map<string, int>& items, int gold) {
	m_lootableItems = items;
	m_lootableGold = gold;
	delete m_lootWindow;
	m_lootWindow = nullptr;
	if (items.empty() && gold <= 0) return;
	m_lootWindow = new LootWindow();
	m_lootWindow->setLoot(items, gold);
}

void ChestTile::onHit(Spell* spell) {
	UnlockSpell* unlock;
	switch (spell->getSpellID()) {
	case SpellID::Unlock:
		if (m_state == GameObjectState::Locked) {
			unlock = dynamic_cast<UnlockSpell*>(spell);
			if (unlock != nullptr && unlock->getStrength() >= m_strength) {
				m_state = GameObjectState::Unlocked;
				setCurrentAnimation(getAnimation(m_state), false);
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

void ChestTile::renderAfterForeground(sf::RenderTarget &renderTarget) {
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
	m_showLootWindow = m_showLootWindow || g_inputController->isKeyActive(Key::ToggleTooltips);
}

void ChestTile::setObjectID(int id) {
	m_objectID = id;
}

void ChestTile::setStrength(int strength) {
	if (strength < 0 || strength > 4) return;
	m_strength = strength;
}

void ChestTile::onMouseOver() {
	if (m_state == GameObjectState::Unlocked) {
		setSpriteColor(COLOR_LOOTABLE, sf::milliseconds(100));
		m_showLootWindow = true;
	}
}

void ChestTile::loot() {
	// loot, create the correct items + gold in the players inventory.
	m_mainChar->lootItems(m_lootableItems);
	if (m_lootableGold > 0)
		m_mainChar->addGold(m_lootableGold);
	m_screen->getCharacterCore()->setChestLooted(m_mainChar->getLevel()->getID(), m_objectID);
	setDisposed();
}

void ChestTile::onRightClick() {
	if (m_state == GameObjectState::Unlocked) {
		// check if the chest is in range
		sf::Vector2f dist = m_mainChar->getCenter() - getCenter();
		if (sqrt(dist.x * dist.x + dist.y * dist.y) <= PICKUP_RANGE) {
			loot();
		}
		else {
			m_screen->setTooltipText("OutOfRange", COLOR_BAD, true);
		}
		g_inputController->lockAction();
	}
	else if (m_strength == 0 && m_state == GameObjectState::Locked) {
		// check if the chest is in range
		sf::Vector2f dist = m_mainChar->getCenter() - getCenter();
		if (sqrt(dist.x * dist.x + dist.y * dist.y) <= PICKUP_RANGE) {
			// unlock!
			setState(GameObjectState::Unlocked);
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