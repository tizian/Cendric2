#include "Map/NPC.h"
#include "Map/MapMainCharacter.h"
#include "Screens/MapScreen.h"

inline bool inRange(const sf::Vector2f& center, const sf::Vector2f& mainCharCenter, const float range) {
	sf::Vector2f distVector = center - mainCharCenter;
	float dist = std::sqrt(distVector.x * distVector.x + distVector.y * distVector.y);
	return dist <= range;
}

void NPC::load(MapMainCharacter* mainChar, const NPCBean& bean) {
	m_mainChar = mainChar;
	m_bean = bean;

	Animation idleAnimation(sf::seconds(10.f));
	setBoundingBox(bean.boundingBox);
	setSpriteOffset(sf::Vector2f(-bean.boundingBox.left, -bean.boundingBox.top));
	idleAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_npcs));
	idleAnimation.addFrame(bean.texturePosition);

	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(false);

	setPosition(bean.position);
	setTooltipText(g_textProvider->getText(bean.id));
	setDebugBoundingBox(sf::Color::Magenta);
}

void NPC::onMouseOver() {
	m_tooltipTime = sf::seconds(1);
}

void NPC::onRightClick() {
	// check if npc is in range
	sf::Vector2f dist = m_mainChar->getCenter() - getCenter();
	if (sqrt(dist.x * dist.x + dist.y * dist.y) <= 100.f) {
		MapScreen* mapScreen = dynamic_cast<MapScreen*>(m_screen);
		mapScreen->setDialogue(m_bean);
	}
	else {
		m_screen->setTooltipText(g_textProvider->getText("OutOfRange"), sf::Color::Red, true);
	}
}

void NPC::onInteractKey() {
	onRightClick();
}

void NPC::renderAfterForeground(sf::RenderTarget &renderTarget) {
	AnimatedGameObject::renderAfterForeground(renderTarget);
	bool showTooltip = g_inputController->isKeyActive(Key::ToggleTooltips);
	if (showTooltip || m_tooltipTime > sf::Time::Zero) {
		renderTarget.draw(m_tooltipText);
	}
}

void NPC::update(const sf::Time& frameTime) {
	AnimatedGameObject::update(frameTime);
	if (m_tooltipTime > sf::Time::Zero) {
		m_tooltipTime = m_tooltipTime - frameTime;
		if (m_tooltipTime < sf::Time::Zero) {
			m_tooltipTime = sf::Time::Zero;
		}
	}
	checkCollisionWithMainChar();
}

void NPC::setTalksActive(bool talksActive) {
	m_bean.talksActive = talksActive;
}

void NPC::setDialogueID(const std::string& id) {
	m_bean.dialogueID = id;
}

void NPC::checkCollisionWithMainChar() {
	if (!m_bean.dialogueID.empty() && m_bean.talksActive && inRange(getCenter(), m_mainChar->getCenter(), TALKING_RANGE)) {
		setTalksActive(false);
		MapScreen* mapScreen = dynamic_cast<MapScreen*>(m_screen);
		mapScreen->setDialogue(m_bean);
	}
}

GameObjectType NPC::getConfiguredType() const {
	return GameObjectType::_NPC;
}

void NPC::setTooltipText(const std::string& tooltip) {
	m_tooltipText = BitmapText(tooltip);
	m_tooltipText.setColor(sf::Color::White);
	m_tooltipText.setCharacterSize(8);
	m_tooltipText.setPosition(sf::Vector2f(getPosition().x, getPosition().y - 10.f));
}

