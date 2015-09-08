#include "DynamicTiles/ChestTile.h"
#include "LevelMainCharacter.h"
#include "Spell.h"

using namespace std;

ChestTile::ChestTile(LevelMainCharacter* mainChar, Level* level) : DynamicTile(level)
{
	m_mainChar = mainChar;
}

void ChestTile::init()
{
	setBoundingBox(sf::FloatRect(0.f, 0.f, static_cast<float>(m_tileSize.x), static_cast<float>(m_tileSize.y)));
	setSpriteOffset(sf::Vector2f(-25.f, -50.f));
}

void ChestTile::load(int skinNr)
{
	m_isCollidable = false;

	Animation closedAnimation;
	closedAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_chest));
	closedAnimation.addFrame(sf::IntRect(0, 0, 2 * m_tileSize.x, 2 * m_tileSize.y));

	addAnimation(GameObjectState::Locked, closedAnimation);
	
	Animation openAnimation;
	openAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_chest));
	openAnimation.addFrame(sf::IntRect(2 * m_tileSize.x, 0, 2 * m_tileSize.x, 2 * m_tileSize.y));

	addAnimation(GameObjectState::Unlocked, openAnimation);

	setFrameTime(sf::seconds(10.f));

	// initial values
	m_state = GameObjectState::Locked;
	setCurrentAnimation(getAnimation(m_state), false);
	playCurrentAnimation(false);
}

void ChestTile::setLoot(const std::map<string, int>& items, int gold)
{
	m_lootableItems = items;
	m_lootableGold = gold;
	delete m_lootWindow;
	m_lootWindow = nullptr;
	if (items.empty() && gold <= 0) return;
	m_lootWindow = new LootWindow();
	m_lootWindow->setLoot(items, gold);
}

void ChestTile::onHit(Spell* spell)
{
	UnlockSpell* unlock;
	switch (spell->getSpellID())
	{
	case SpellID::Unlock:
		if (m_state == GameObjectState::Locked)
		{
			unlock = dynamic_cast<UnlockSpell*>(spell);
			if (unlock != nullptr && unlock->getStrength() >= m_strength)
			{
				m_state = GameObjectState::Unlocked;
				setCurrentAnimation(getAnimation(m_state), false);
			}
			spell->setDisposed();
		}
		break;
	default:
		break;
	}
}

void ChestTile::renderAfterForeground(sf::RenderTarget &renderTarget)
{
	GameObject::renderAfterForeground(renderTarget);
	if (m_showLootWindow && m_lootWindow != nullptr)
	{
		m_lootWindow->render(renderTarget);
		m_showLootWindow = false;
	}
}

void ChestTile::update(const sf::Time& frameTime)
{
	DynamicTile::update(frameTime);
	if (m_showLootWindow && m_lootWindow != nullptr)
	{
		sf::Vector2f pos(getBoundingBox()->left + getBoundingBox()->width, getBoundingBox()->top - m_lootWindow->getSize().y + 10.f);
		m_lootWindow->setPosition(pos);
	}
}

void ChestTile::setObjectID(int id)
{
	m_objectID = id;
}

void ChestTile::setStrength(int strength)
{
	if (strength < 0 || strength > 4) return;
	m_strength = strength;
}

void ChestTile::onMouseOver()
{
	if (m_state == GameObjectState::Unlocked)
	{
		setSpriteColor(sf::Color::Red, sf::milliseconds(100));
		m_showLootWindow = true;
	}
}

void ChestTile::onRightClick()
{
	if (m_state == GameObjectState::Unlocked)
	{
		// check if the chest is in range
		sf::Vector2f dist = m_mainChar->getCenter() - getCenter();
		if (sqrt(dist.x * dist.x + dist.y * dist.y) <= PICKUP_RANGE)
		{
			// loot, create the correct items + gold in the players inventory.
			m_mainChar->lootItems(m_lootableItems);
			m_mainChar->addGold(m_lootableGold);
			m_screen->getCharacterCore()->setChestLooted(m_mainChar->getLevel()->getID(), m_objectID);
			setDisposed();
		}
		else
		{
			m_screen->setTooltipText(g_textProvider->getText("OutOfRange"), sf::Color::Red, true);
		}
		g_inputController->lockAction();
	}
	else if (m_strength == 0 && m_state == GameObjectState::Locked)
	{
		// check if the chest is in range
		sf::Vector2f dist = m_mainChar->getCenter() - getCenter();
		if (sqrt(dist.x * dist.x + dist.y * dist.y) <= PICKUP_RANGE)
		{
			// unlock!
			m_state = GameObjectState::Unlocked;
			setCurrentAnimation(getAnimation(m_state), false);
		}
		else
		{
			m_screen->setTooltipText(g_textProvider->getText("OutOfRange"), sf::Color::Red, true);
		}
		g_inputController->lockAction();
	}
}