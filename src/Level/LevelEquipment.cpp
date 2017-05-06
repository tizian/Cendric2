#include "Level/LevelEquipment.h"
#include "Level/LevelMainCharacter.h"
#include "GameObjectComponents/LightComponent.h"

const int LevelEquipment::EQ_SIZE = 120;

LevelEquipment::LevelEquipment(LevelMainCharacter* mainChar) : AnimatedGameObject() { 
	m_mainChar = mainChar; 
	m_screen = mainChar->getScreen();
}

void LevelEquipment::load(const ItemEquipmentBean* itemBean, const ItemEquipmentLightBean* light, ItemType type) {
	if (itemBean == nullptr || itemBean->status == BeanStatus::NotSet) return;
	m_itemType = type;
	const ItemEquipmentBean& eq = *itemBean;

	std::map<GameObjectState, std::vector<sf::IntRect>> texturePositions;

	int offset = 0;
	for (int i = 0; i < eq.frames_walk; ++i) {
		texturePositions[GameObjectState::Walking].push_back(sf::IntRect(offset * EQ_SIZE, 0, EQ_SIZE, EQ_SIZE));
		++offset;
	}
	for (int i = 0; i < eq.frames_idle; ++i) {
		texturePositions[GameObjectState::Idle].push_back(sf::IntRect(offset * EQ_SIZE, 0, EQ_SIZE, EQ_SIZE));
		++offset;
	}
	for (int i = 0; i < eq.frames_jump; ++i) {
		texturePositions[GameObjectState::Jumping].push_back(sf::IntRect(offset * EQ_SIZE, 0, EQ_SIZE, EQ_SIZE));
		++offset;
	}
	for (int i = 0; i < eq.frames_fight; ++i) {
		texturePositions[GameObjectState::Fighting].push_back(sf::IntRect(offset * EQ_SIZE, 0, EQ_SIZE, EQ_SIZE));
		++offset;
	}
	for (int i = 0; i < eq.frames_climb1; ++i) {
		texturePositions[GameObjectState::Climbing_1].push_back(sf::IntRect(offset * EQ_SIZE, 0, EQ_SIZE, EQ_SIZE));
		++offset;
	}
	for (int i = 0; i < eq.frames_climb2; ++i) {
		texturePositions[GameObjectState::Climbing_2].push_back(sf::IntRect(offset * EQ_SIZE, 0, EQ_SIZE, EQ_SIZE));
		++offset;
	}

	setBoundingBox(sf::FloatRect(0, 0, static_cast<float>(EQ_SIZE), static_cast<float>(EQ_SIZE)));

	if (light != nullptr && light->status == BeanStatus::Filled) {
		const ItemEquipmentLightBean& lightBean = *light;
		LightData lightData(LightData(lightBean.light_offset, lightBean.light_radius, lightBean.brightness));
		setLightComponent(lightData);
	}

	if (!eq.texture_path.empty()) {
		g_resourceManager->loadTexture(eq.texture_path, ResourceType::Level);
		for (auto& ani : texturePositions) {
			Animation* animation = new Animation();
			if (ani.first == GameObjectState::Fighting) {
				animation->setFrameTime(sf::milliseconds(70));
			}
			animation->setSpriteSheet(g_resourceManager->getTexture(eq.texture_path));
			for (auto& frame : ani.second) {
				animation->addFrame(frame);
			}
			addAnimation(ani.first, animation);
		}

		// initial values
		setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
		playCurrentAnimation(true);

		m_hasTexture = true;
	}

	sf::Vector2f newPosition;
	calculatePositionAccordingToMainChar(newPosition);
	setPosition(newPosition);
}

void LevelEquipment::calculatePositionAccordingToMainChar(sf::Vector2f& position) const {
	sf::Vector2f mainCharPosition(m_mainChar->getPosition().x + (m_mainChar->getBoundingBox()->width / 2), m_mainChar->getPosition().y);
	sf::Vector2f offset(-60.f, -30.f);
	if (!m_mainChar->isFacingRight())
		offset.x = -offset.x - getBoundingBox()->width;
	if (m_mainChar->isUpsideDown())
		offset.y = m_mainChar->getBoundingBox()->height - offset.y - getBoundingBox()->height;

	position.x = (mainCharPosition + offset).x;
	position.y = (mainCharPosition + offset).y;
}

void LevelEquipment::setPosition(const sf::Vector2f& position) {
	if (m_lightComponent != nullptr) {
		m_lightComponent->flipOffsetX(m_isFacingRight);
	}
	AnimatedGameObject::setPosition(position);
}

void LevelEquipment::update(const sf::Time& frameTime) {
	if (!m_hasTexture) {
		sf::Vector2f newPosition;
		calculatePositionAccordingToMainChar(newPosition);
		setPosition(newPosition);
		return;
	}

	GameObjectState newState = m_mainChar->getState();
	
	bool newFacingRight = m_mainChar->isFacingRight();
	if (m_state != newState || newFacingRight != m_isFacingRight) {
		m_state = newState;
		m_isFacingRight = newFacingRight;
		if (m_state != GameObjectState::Dead) {
			setCurrentAnimation(getAnimation(m_state), !m_isFacingRight);
		}
		else {
			loopCurrentAnimation(false);
			m_animatedSprite.stop();
		}
	}
	if (m_mainChar->isUpsideDown() != m_animatedSprite.isFlippedY()) {
		m_animatedSprite.setFlippedY(m_mainChar->isUpsideDown());
		if (m_lightComponent != nullptr) m_lightComponent->flipOffsetY(m_mainChar->isUpsideDown());
	}

	sf::Vector2f newPosition;
	calculatePositionAccordingToMainChar(newPosition);
	setPosition(newPosition);
	AnimatedGameObject::update(frameTime);
	if (m_isCopyingMainCharColor)
		setSpriteColor(m_mainChar->getCurrentSpriteColor(), sf::milliseconds(1));
}

void LevelEquipment::setLightComponent(const LightData& data) {
	m_lightComponent = new LightComponent(data, this);
	addComponent(m_lightComponent);
}

ItemType LevelEquipment::getItemType() const {
	return m_itemType;
}

GameObjectType LevelEquipment::getConfiguredType() const {
	return GameObjectType::_Equipment;
}

void LevelEquipment::setCopyingMainCharColor(bool value) {
	m_isCopyingMainCharColor = value;
}
