#include "Level/LevelItem.h"
#include "Level/LevelMainCharacter.h"

void LevelItem::load(LevelMainCharacter* mainChar, const Item& item, const sf::Vector2f& position) {
	m_mainChar = mainChar;
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

	setPosition(position - getSpriteOffset());
	setTooltipText(g_textProvider->getText(item.getID(), "item"));
	setDebugBoundingBox(sf::Color::Green);
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
	// check if item is in range
	sf::Vector2f dist = m_mainChar->getCenter() - getCenter();
	if (sqrt(dist.x * dist.x + dist.y * dist.y) <= PICKUP_RANGE) {
		pickup();
	}
	else {
		m_screen->setTooltipText("OutOfRange", sf::Color::Red, true);
	}
	g_inputController->lockAction();
}

void LevelItem::onInteractKey() {
	// it is made impossible for a game object to have onInteractKey and onRightClick called in the same game loop
	onRightClick();
}

void LevelItem::onMouseOver() {
	m_animatedSprite.setColor(sf::Color::Red);
	m_tooltipTime = sf::seconds(1);
}

void LevelItem::render(sf::RenderTarget &renderTarget) {
	AnimatedGameObject::render(renderTarget);
	m_animatedSprite.setColor(sf::Color::White);
}

void LevelItem::renderAfterForeground(sf::RenderTarget& renderTarget) {
	AnimatedGameObject::renderAfterForeground(renderTarget);
	bool showTooltip = g_inputController->isKeyActive(Key::ToggleTooltips);
	if (showTooltip || m_tooltipTime > sf::Time::Zero) {
		renderTarget.draw(m_tooltipText);
	}
}

void LevelItem::update(const sf::Time& frameTime) {
	AnimatedGameObject::update(frameTime);
	if (m_tooltipTime > sf::Time::Zero) {
		m_tooltipTime = m_tooltipTime - frameTime;
		if (m_tooltipTime < sf::Time::Zero) {
			m_tooltipTime = sf::Time::Zero;
		}
	}
}

GameObjectType LevelItem::getConfiguredType() const {
	return GameObjectType::_LevelItem;
}

void LevelItem::setTooltipText(const std::string& tooltip) {
	m_tooltipText = BitmapText(tooltip);
	m_tooltipText.setColor(sf::Color::White);
	m_tooltipText.setCharacterSize(8);
	m_tooltipText.setPosition(sf::Vector2f(getPosition().x, getPosition().y - 10.f));
}

void LevelItem::setSpawnPosition(int spawnPosition) {
	m_spawnPosition = spawnPosition;
}

