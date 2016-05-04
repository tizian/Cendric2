#include "Level/LevelItem.h"
#include "Level/LevelMainCharacter.h"
#include "Screens/LevelScreen.h"
#include "GameObjectComponents/InteractComponent.h"
#include "GameObjectComponents/LightComponent.h"

LevelItem::LevelItem(LevelScreen* screen) : AnimatedGameObject() {
	m_screen = screen;
	m_mainChar = screen->getMainCharacter();
}

void LevelItem::load(const Item& item, const sf::Vector2f& position) {
	m_itemID = item.getID();
	m_itemType = item.getType();
	m_goldValue = item.getValue();

	if (!item.isLevelitem()) {
		g_logger->logError("LevelItem", "Tried to instantiate Levelitem that has no frames!");
		return;
	}

	Animation* idleAnimation = new Animation(item.getLevelitemBean().frame_time);
	setSpriteOffset(item.getLevelitemBean().sprite_offset);
	setBoundingBox(sf::FloatRect(0.f, 0.f, item.getLevelitemBean().bounding_box.x, item.getLevelitemBean().bounding_box.y));
	idleAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_levelitems));
	// add frames
	for (auto &frame : item.getFrames()) {
		idleAnimation->addFrame(frame.texture_location);
	}
	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(item.getFrames().size() > 1);

	if (item.isLevelitemLightedItem()) {
		const LevelitemLightBean& lightBean = item.getLevelitemLightBean();
		LightData lightData(lightBean.light_offset, lightBean.light_radius, lightBean.brightness);
		addComponent(new LightComponent(lightData, this));
	}

	InteractComponent* interactComponent = new InteractComponent(g_textProvider->getText(item.getID(), "item"), this, m_mainChar);
	interactComponent->setInteractRange(PICKUP_RANGE);
	interactComponent->setInteractText("ToPickup");
	interactComponent->setOnInteract(std::bind(&LevelItem::pickup, this));
	addComponent(interactComponent);
	setDebugBoundingBox(COLOR_GOOD);

	setPosition(position - getSpriteOffset());
}

void LevelItem::pickup() {
	// pickup, create the correct item or correct amount of gold in the players inventory.
	if (m_itemType == ItemType::Gold) {
		m_mainChar->addGold(m_goldValue);
	}
	else {
		m_mainChar->lootItem(m_itemID, 1);
	}
	m_screen->getCharacterCore()->setItemLooted(m_mainChar->getLevel()->getID(), m_spawnPosition);
	setDisposed();
}

void LevelItem::onRightClick() {
	if (m_mainChar->isDead()) return;
	// check if item is in range
	if (dist(m_mainChar->getCenter(), getCenter()) <= PICKUP_RANGE) {
		pickup();
	}
	else {
		m_screen->setTooltipText("OutOfRange", COLOR_BAD, true);
	}
	g_inputController->lockAction();
}

GameObjectType LevelItem::getConfiguredType() const {
	return GameObjectType::_LevelItem;
}

void LevelItem::setSpawnPosition(int spawnPosition) {
	m_spawnPosition = spawnPosition;
}

