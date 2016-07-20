#include "Map/NPC.h"
#include "Map/MapMainCharacter.h"
#include "Screens/MapScreen.h"
#include "GameObjectComponents/InteractComponent.h"

NPC::NPC(MapScreen* mapScreen) : MapMovableGameObject(mapScreen->getWorld()) {
	m_screen = mapScreen;
	m_mainChar = mapScreen->getMainCharacter();
}

void NPC::load(const NPCData& data) {
	g_resourceManager->loadTexture(data.spritesheetpath, ResourceType::Map);
	const sf::Texture* tex = g_resourceManager->getTexture(data.spritesheetpath);
	m_NPCdata = data;
	setBoundingBox(data.boundingBox);
	setSpriteOffset(sf::Vector2f(-data.boundingBox.left, -data.boundingBox.top));

	Animation* walkingAnimationDown = new Animation(sf::seconds(0.15f));
	walkingAnimationDown->setSpriteSheet(tex);
	walkingAnimationDown->addFrame(sf::IntRect(0, 0, 50, 50));
	walkingAnimationDown->addFrame(sf::IntRect(50, 0, 50, 50));
	walkingAnimationDown->addFrame(sf::IntRect(100, 0, 50, 50));
	walkingAnimationDown->addFrame(sf::IntRect(50, 0, 50, 50));

	addAnimation(GameObjectState::Walking_down, walkingAnimationDown);

	Animation* walkingAnimationLeft = new Animation(sf::seconds(0.15f));
	walkingAnimationLeft->setSpriteSheet(tex);
	walkingAnimationLeft->addFrame(sf::IntRect(0, 50, 50, 50));
	walkingAnimationLeft->addFrame(sf::IntRect(50, 50, 50, 50));
	walkingAnimationLeft->addFrame(sf::IntRect(100, 50, 50, 50));
	walkingAnimationLeft->addFrame(sf::IntRect(50, 50, 50, 50));

	addAnimation(GameObjectState::Walking_left, walkingAnimationLeft);

	Animation* walkingAnimationRight = new Animation(sf::seconds(0.15f));
	walkingAnimationRight->setSpriteSheet(tex);
	walkingAnimationRight->addFrame(sf::IntRect(0, 100, 50, 50));
	walkingAnimationRight->addFrame(sf::IntRect(50, 100, 50, 50));
	walkingAnimationRight->addFrame(sf::IntRect(100, 100, 50, 50));
	walkingAnimationRight->addFrame(sf::IntRect(50, 100, 50, 50));

	addAnimation(GameObjectState::Walking_right, walkingAnimationRight);

	Animation* walkingAnimationUp = new Animation(sf::seconds(0.15f));
	walkingAnimationUp->setSpriteSheet(tex);
	walkingAnimationUp->addFrame(sf::IntRect(0, 150, 50, 50));
	walkingAnimationUp->addFrame(sf::IntRect(50, 150, 50, 50));
	walkingAnimationUp->addFrame(sf::IntRect(100, 150, 50, 50));
	walkingAnimationUp->addFrame(sf::IntRect(50, 150, 50, 50));

	addAnimation(GameObjectState::Walking_up, walkingAnimationUp);

	Animation* idleAnimationDown = new Animation();
	idleAnimationDown->setSpriteSheet(tex);
	idleAnimationDown->addFrame(sf::IntRect(50, 0, 50, 50));

	addAnimation(GameObjectState::Idle_down, idleAnimationDown);

	Animation* idleAnimationLeft = new Animation();
	idleAnimationLeft->setSpriteSheet(tex);
	idleAnimationLeft->addFrame(sf::IntRect(50, 50, 50, 50));

	addAnimation(GameObjectState::Idle_left, idleAnimationLeft);

	Animation* idleAnimationRight = new Animation();
	idleAnimationRight->setSpriteSheet(tex);
	idleAnimationRight->addFrame(sf::IntRect(50, 100, 50, 50));

	addAnimation(GameObjectState::Idle_right, idleAnimationRight);

	Animation* idleAnimationUp = new Animation();
	idleAnimationUp->setSpriteSheet(tex);
	idleAnimationUp->addFrame(sf::IntRect(50, 150, 50, 50));

	addAnimation(GameObjectState::Idle_up, idleAnimationUp);

	// initial values
	m_state = GameObjectState::Idle_right;
	setCurrentAnimation(getAnimation(m_state), false);
	playCurrentAnimation(true);

	setPosition(data.position);

	InteractComponent* interactComponent = new InteractComponent(g_textProvider->getText(data.id, "npc"), this, m_mainChar);
	interactComponent->setInteractRange(TALKING_RANGE);
	interactComponent->setInteractText("ToTalk");
	interactComponent->setOnInteract(std::bind(&NPC::trySetDialogue, this));
	addComponent(interactComponent);

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

void NPC::onLeftClick() {
	onRightClick();
}

void NPC::onRightClick() {
	// check if npc is in range
	if (dist(m_mainChar->getCenter(), getCenter()) <= TALKING_RANGE) {
		trySetDialogue();
	}
	else {
		m_screen->setTooltipText("OutOfRange", COLOR_BAD, true);
	}
}

void NPC::update(const sf::Time& frameTime) {
	if (m_routineReloadNeeded) {
		reloadRoutine();
		m_routineReloadNeeded = false;
	}

	m_routine.update(frameTime);
	updateAnimation(frameTime);
	MovableGameObject::update(frameTime);
	checkCollisionWithMainChar();
}

void NPC::notifyReloadNeeded() {
	if (m_routineReloadEnabled) {
		m_routineReloadNeeded = true;
	}
}

void NPC::setTalkingActive(bool active) {
	m_NPCdata.talkingActive = active;
}

void NPC::setTalkingEnabled(bool enabled) {
	m_NPCdata.talkingEnabled = enabled;
}

void NPC::setReloadEnabled(bool enabled) {
	m_routineReloadEnabled = enabled;
}

void NPC::setDialogueID(const std::string& id) {
	m_NPCdata.dialogueID = id;
}

void NPC::checkCollisionWithMainChar() {
	if (m_NPCdata.talkingActive && dist(getCenter(), m_mainChar->getCenter()) <= TALKING_RANGE) {
		trySetDialogue();
	}
}

void NPC::trySetDialogue() {
	if (m_NPCdata.dialogueID.empty() || !m_NPCdata.talkingEnabled) {
		m_screen->setTooltipText("NothingToSay", COLOR_BAD, true);
		return;
	}
	MapScreen* mapScreen = dynamic_cast<MapScreen*>(m_screen);
	turnToMainchar();
	mapScreen->setDialogue(this);
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

GameObjectType NPC::getConfiguredType() const {
	return GameObjectType::_MapMovableGameObject;
}

const NPCData& NPC::getNPCData() const {
	return m_NPCdata;
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
