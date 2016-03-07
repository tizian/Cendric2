#include "Map/NPC.h"
#include "Map/MapMainCharacter.h"
#include "Screens/MapScreen.h"

inline bool inRange(const sf::Vector2f& center, const sf::Vector2f& mainCharCenter, const float range) {
	return dist(center, mainCharCenter) <= range;
}

void NPC::load(MapMainCharacter* mainChar, const NPCData& data) {
	m_mainChar = mainChar;
	m_NPCdata = data;
	setBoundingBox(data.boundingBox);
	setSpriteOffset(sf::Vector2f(-data.boundingBox.left, -data.boundingBox.top));

	Animation* walkingAnimationDown = new Animation(sf::seconds(0.15f));
	walkingAnimationDown->setSpriteSheet(g_resourceManager->getTexture(data.spritesheetpath));
	walkingAnimationDown->addFrame(sf::IntRect(0, 0, 50, 50));
	walkingAnimationDown->addFrame(sf::IntRect(50, 0, 50, 50));
	walkingAnimationDown->addFrame(sf::IntRect(100, 0, 50, 50));
	walkingAnimationDown->addFrame(sf::IntRect(50, 0, 50, 50));

	addAnimation(GameObjectState::Walking_down, walkingAnimationDown);

	Animation* walkingAnimationLeft = new Animation(sf::seconds(0.15f));
	walkingAnimationLeft->setSpriteSheet(g_resourceManager->getTexture(data.spritesheetpath));
	walkingAnimationLeft->addFrame(sf::IntRect(0, 50, 50, 50));
	walkingAnimationLeft->addFrame(sf::IntRect(50, 50, 50, 50));
	walkingAnimationLeft->addFrame(sf::IntRect(100, 50, 50, 50));
	walkingAnimationLeft->addFrame(sf::IntRect(50, 50, 50, 50));

	addAnimation(GameObjectState::Walking_left, walkingAnimationLeft);

	Animation* walkingAnimationRight = new Animation(sf::seconds(0.15f));
	walkingAnimationRight->setSpriteSheet(g_resourceManager->getTexture(data.spritesheetpath));
	walkingAnimationRight->addFrame(sf::IntRect(0, 100, 50, 50));
	walkingAnimationRight->addFrame(sf::IntRect(50, 100, 50, 50));
	walkingAnimationRight->addFrame(sf::IntRect(100, 100, 50, 50));
	walkingAnimationRight->addFrame(sf::IntRect(50, 100, 50, 50));

	addAnimation(GameObjectState::Walking_right, walkingAnimationRight);

	Animation* walkingAnimationUp = new Animation(sf::seconds(0.15f));
	walkingAnimationUp->setSpriteSheet(g_resourceManager->getTexture(data.spritesheetpath));
	walkingAnimationUp->addFrame(sf::IntRect(0, 150, 50, 50));
	walkingAnimationUp->addFrame(sf::IntRect(50, 150, 50, 50));
	walkingAnimationUp->addFrame(sf::IntRect(100, 150, 50, 50));
	walkingAnimationUp->addFrame(sf::IntRect(50, 150, 50, 50));

	addAnimation(GameObjectState::Walking_up, walkingAnimationUp);

	Animation* idleAnimationDown = new Animation();
	idleAnimationDown->setSpriteSheet(g_resourceManager->getTexture(data.spritesheetpath));
	idleAnimationDown->addFrame(sf::IntRect(50, 0, 50, 50));

	addAnimation(GameObjectState::Idle_down, idleAnimationDown);

	Animation* idleAnimationLeft = new Animation();
	idleAnimationLeft->setSpriteSheet(g_resourceManager->getTexture(data.spritesheetpath));
	idleAnimationLeft->addFrame(sf::IntRect(50, 50, 50, 50));

	addAnimation(GameObjectState::Idle_left, idleAnimationLeft);

	Animation* idleAnimationRight = new Animation();
	idleAnimationRight->setSpriteSheet(g_resourceManager->getTexture(data.spritesheetpath));
	idleAnimationRight->addFrame(sf::IntRect(50, 100, 50, 50));

	addAnimation(GameObjectState::Idle_right, idleAnimationRight);

	Animation* idleAnimationUp = new Animation();
	idleAnimationUp->setSpriteSheet(g_resourceManager->getTexture(data.spritesheetpath));
	idleAnimationUp->addFrame(sf::IntRect(50, 150, 50, 50));

	addAnimation(GameObjectState::Idle_up, idleAnimationUp);

	// initial values
	m_state = GameObjectState::Idle_right;
	setCurrentAnimation(getAnimation(m_state), false);
	playCurrentAnimation(true);

	setPosition(data.position);
	setTooltipText(g_textProvider->getText(data.id, "npc"));
	setDebugBoundingBox(sf::Color::Magenta);

	if (!data.routineID.empty()) {
		setTalkingActive(false);
		setTalkingEnabled(true);
		m_routine.load(data.routineID, this);
	}
}

void NPC::reloadRoutine() {
	if (!m_routine.getID().empty()) {
		setTalkingActive(false);
		setTalkingEnabled(true);
		m_routine.load(m_routine.getID(), this, false);
	}
}

void NPC::onMouseOver() {
	m_tooltipTime = sf::seconds(1);
}

void NPC::onLeftClick() {
	onRightClick();
}

void NPC::onRightClick() {
	if (!m_NPCdata.talkingEnabled) {
		m_screen->setTooltipText("NothingToSay", sf::Color::Red, true);
		return;
	}
	// check if npc is in range
	sf::Vector2f dist = m_mainChar->getCenter() - getCenter();
	if (sqrt(dist.x * dist.x + dist.y * dist.y) <= 100.f) {
		MapScreen* mapScreen = dynamic_cast<MapScreen*>(m_screen);
		turnToMainchar();
		mapScreen->setDialogue(this);
	}
	else {
		m_screen->setTooltipText("OutOfRange", sf::Color::Red, true);
	}
}

void NPC::renderAfterForeground(sf::RenderTarget &renderTarget) {
	MovableGameObject::renderAfterForeground(renderTarget);
	bool showTooltip = g_inputController->isKeyActive(Key::ToggleTooltips);
	if (showTooltip || m_tooltipTime > sf::Time::Zero) {
		renderTarget.draw(m_tooltipText);
	}
}

void NPC::update(const sf::Time& frameTime) {
	GameObject::updateTime(m_tooltipTime, frameTime);
	m_routine.update(frameTime);
	updateAnimation(frameTime);
	MovableGameObject::update(frameTime);
	checkCollisionWithMainChar();
}

void NPC::setTalkingActive(bool active) {
	m_NPCdata.talkingActive = active;
}

void NPC::setTalkingEnabled(bool enabled) {
	m_NPCdata.talkingEnabled = enabled;
}

void NPC::setDialogueID(const std::string& id) {
	m_NPCdata.dialogueID = id;
}

void NPC::checkCollisionWithMainChar() {
	if (m_NPCdata.talkingActive && !m_NPCdata.dialogueID.empty() && inRange(getCenter(), m_mainChar->getCenter(), TALKING_RANGE)) {
		turnToMainchar();
		MapScreen* mapScreen = dynamic_cast<MapScreen*>(m_screen);
		
		mapScreen->setDialogue(this);
	}
}

void NPC::turnToMainchar() {
	sf::Vector2f distance = m_mainChar->getCenter() - getCenter();
	if (std::abs(distance.x) > std::abs(distance.y)) {
		setState((distance.x > 0.f) ? GameObjectState::Idle_right : GameObjectState::Idle_left);
	}
	else {
		setState((distance.y > 0.f) ? GameObjectState::Idle_down : GameObjectState::Idle_up);
	}
}

void NPC::setPosition(const sf::Vector2f& pos) {
	MapMovableGameObject::setPosition(pos);
	m_tooltipText.setPosition(sf::Vector2f(pos.x, pos.y - 10.f));
}

GameObjectType NPC::getConfiguredType() const {
	return GameObjectType::_MapMovableGameObject;
}

const NPCData& NPC::getNPCData() const {
	return m_NPCdata;
}

void NPC::setTooltipText(const std::string& tooltip) {
	m_tooltipText = BitmapText(tooltip);
	m_tooltipText.setTextStyle(TextStyle::Shadowed);
	m_tooltipText.setColor(sf::Color::White);
	m_tooltipText.setCharacterSize(8);
}

float NPC::getConfiguredMaxVelocityYUp() const {
	return 200.0f;
}

float NPC::getConfiguredMaxVelocityYDown() const {
	return 200.0f;
}

float NPC::getConfiguredMaxVelocityX() const {
	return 200.0f;
}
