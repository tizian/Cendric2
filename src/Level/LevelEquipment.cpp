#include "Level/LevelEquipment.h"
#include "Level/LevelMainCharacter.h"
#include "Screens/LevelScreen.h"
#include "GameObjectComponents/LightComponent.h"
#include "GameObjectComponents/ParticleComponent.h"
#include "World/CustomParticleUpdaters.h"

const int LevelEquipment::EQ_SIZE = 120;

LevelEquipment::LevelEquipment(LevelMainCharacter* mainChar) : AnimatedGameObject() {
	m_mainChar = mainChar;
	m_screen = mainChar->getScreen();
	m_isAlwaysUpdate = true;
}

void LevelEquipment::load(const ItemEquipmentBean* itemBean, ItemType type) {
	if (itemBean == nullptr) return;
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
}

void LevelEquipment::loadComponents(const ItemEquipmentLightBean* light, const ItemEquipmentParticleBean* particles) {
	if (light != nullptr) {
		LightData lightData(LightData(light->light_offset + sf::Vector2f(0.5f * getBoundingBox()->width, 0), light->light_radius, light->brightness));
		setLightComponent(lightData);
	}

	if (particles != nullptr) {
		g_resourceManager->loadTexture(particles->texture_path, ResourceType::Level);

		ParticleComponentData data;
		data.particleCount = particles->particle_count;
		data.emitRate = particles->emit_rate;
		data.isAdditiveBlendMode = particles->is_additive_blend_mode;
		data.texturePath = particles->texture_path;
		data.particleTexture = &dynamic_cast<LevelScreen*>(m_screen)->getParticleEQRenderTexture();

		// Generators
		auto posGen = new particles::DiskSpawner();
		posGen->radius = particles->spawner_radius;
		data.spawner = posGen;

		auto sizeGen = new particles::SizeGenerator();
		sizeGen->minStartSize = particles->size_start_min;
		sizeGen->maxStartSize = particles->size_start_max;
		sizeGen->minEndSize = particles->size_end_min;
		sizeGen->maxEndSize = particles->size_end_max;
		data.sizeGen = sizeGen;

		auto colGen = new particles::ColorGenerator();
		colGen->minStartCol = particles->color_start_min;
		colGen->maxStartCol = particles->color_start_max;
		colGen->minEndCol = particles->color_end_min;
		colGen->maxEndCol = particles->color_end_max;
		data.colorGen = colGen;

		auto velGen = new particles::AimedCircleVelocityGenerator();
		velGen->minStartSpeed = particles->speed_min;
		velGen->maxStartSpeed = particles->speed_min;
		velGen->goalRadius = particles->goal_radius;
		data.velGen = velGen;

		auto timeGen = new particles::TimeGenerator();
		timeGen->minTime = particles->time_min;
		timeGen->maxTime = particles->time_max;
		data.timeGen = timeGen;

		calculatePositionAccordingToMainChar(m_position);
		auto eulerUpdater = new particles::AttractingEulerUpdater(&m_position, particles->attract_fraction);
		data.eulerUpdater = eulerUpdater;

		setParticleComponent(data, particles->spawner_offset, particles->goal_offset);
	}

	calculatePositionAccordingToMainChar(m_position);
	setPosition(m_position);
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

void LevelEquipment::updateParticlesVisibility() const {
	if (!m_particleComponent) return;
	if (m_itemType != ItemType::Equipment_weapon) {
		m_particleComponent->setVisible(m_state != GameObjectState::Dead);
	}
	else {
		m_particleComponent->setVisible(!(m_state == GameObjectState::Dead || m_state == GameObjectState::Climbing_1 || m_state == GameObjectState::Climbing_2));
	}
}

void LevelEquipment::setPosition(const sf::Vector2f& position) {
	if (m_lightComponent != nullptr) {
		m_lightComponent->flipOffsetX(!m_isFacingRight);
	}
	if (m_particleComponent != nullptr) {
		m_particleComponent->flipOffsetX(!m_isFacingRight);
	}
	AnimatedGameObject::setPosition(position);
}

void LevelEquipment::lockAnimation(bool lock) {
	m_isLocked = lock;
	if (m_isLocked) {
		loopCurrentAnimation(false);
		m_animatedSprite.stop();
	}
}

void LevelEquipment::update(const sf::Time& frameTime) {
	updateAnimation();
	checkAndSetPosition();

	if (m_isLocked) {
		return;
	}

	AnimatedGameObject::update(frameTime);
}

void LevelEquipment::updateAnimation() {
	GameObjectState newState = m_mainChar->getState();
	bool newFacingRight = m_mainChar->isFacingRight();
	if (m_state != newState || newFacingRight != m_isFacingRight) {
		m_state = newState;
		m_isFacingRight = newFacingRight;
		if (m_hasTexture && m_state != GameObjectState::Dead) {
			setCurrentAnimation(getAnimation(m_state), !m_isFacingRight);
		}
		updateParticlesVisibility();
	}
}

void LevelEquipment::checkAndSetPosition() {
	if (m_mainChar->isUpsideDown() != m_animatedSprite.isFlippedY()) {
		m_animatedSprite.setFlippedY(m_mainChar->isUpsideDown());
		if (m_lightComponent != nullptr) m_lightComponent->flipOffsetY(m_mainChar->isUpsideDown());
		if (m_particleComponent != nullptr) m_particleComponent->flipOffsetY(m_mainChar->isUpsideDown());
	}

	calculatePositionAccordingToMainChar(m_position);
	setPosition(m_position);
}

void LevelEquipment::setLightComponent(const LightData& data) {
	delete m_lightComponent;
	m_lightComponent = new LightComponent(data, this);
	addComponent(m_lightComponent);
}

void LevelEquipment::setParticleComponent(const ParticleComponentData& data, const sf::Vector2f& offset, const sf::Vector2f& goalOffset) {
	delete m_particleComponent;
	m_particleComponent = new ParticleComponent(data, this);
	m_particleComponent->setOffset(sf::Vector2f(0.5f * getBoundingBox()->width, 0) + offset);
	m_particleComponent->setGoalOffset(sf::Vector2f(0.5f * getBoundingBox()->width, 0) + goalOffset);
	addComponent(m_particleComponent);
}

ItemType LevelEquipment::getItemType() const {
	return m_itemType;
}

GameObjectType LevelEquipment::getConfiguredType() const {
	return _Equipment;
}
