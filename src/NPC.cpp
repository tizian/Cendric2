#include "NPC.h"
#include "MapMainCharacter.h"
#include "Screens/MapScreen.h"

void NPC::load(MapMainCharacter* mainChar, const NPCBean& bean) {
	m_mainChar = mainChar;
	m_bean = bean;

	Animation idleAnimation;
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	setBoundingBox(bean.boundingBox);
	idleAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_npcs));
	idleAnimation.addFrame(bean.texturePosition);

	addAnimation(GameObjectState::Idle, idleAnimation);
	setFrameTime(sf::seconds(10.f));

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
	if (sqrt(dist.x * dist.x + dist.y * dist.y) <= TALKING_RANGE) {
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
	GameObject::renderAfterForeground(renderTarget);
	bool showTooltip = g_inputController->isKeyActive(Key::ToggleTooltips);
	if (showTooltip || m_tooltipTime > sf::Time::Zero) {
		renderTarget.draw(m_tooltipText);
	}
}

void NPC::update(const sf::Time& frameTime) {
	GameObject::update(frameTime);
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
	if (!m_bean.dialogueID.empty() && m_bean.talksActive && getBoundingBox()->intersects(*(m_mainChar->getBoundingBox()))) {
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
