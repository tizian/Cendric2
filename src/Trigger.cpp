#include "Trigger.h"
#include "Screens/WorldScreen.h"

Trigger::Trigger(WorldScreen* screen, const TriggerData& data) {
	m_worldScreen = screen;
	m_mainChar = screen->getMainCharacter();
	m_data = data;
	if (m_data.isKeyGuarded) {
		m_isOnTrigger = false;
		const sf::Texture* texture = g_resourceManager->getTexture(ResourceID::Texture_GUI_exit_arrow);
		m_sprite.setTexture(*texture);
		m_sprite.setPosition(data.triggerRect.left + 0.5f * (data.triggerRect.width - texture->getSize().x), data.triggerRect.top + data.triggerRect.height - 2.f * m_mainChar->getSize().y);
	}

	setBoundingBox(data.triggerRect);
	setDebugBoundingBox(sf::Color::Magenta);
}

void Trigger::update(const sf::Time& frameTime) {
	GameObject::update(frameTime);
	m_time += frameTime;

	if (m_data.isKeyGuarded) {
		sf::Vector2f pos = m_sprite.getPosition();
		float variance = 6.f;
		float speed = 3.f;
		float offset = variance * std::cos(speed * m_time.asSeconds());
		float y = m_data.triggerRect.top + m_data.triggerRect.height - 2.f * m_mainChar->getSize().y - 0.5f * variance + offset;
		m_sprite.setPosition(pos.x, y);
	}

	bool intersects = m_mainChar->getBoundingBox()->intersects(m_data.triggerRect);
	m_showSprite = intersects;

	if (m_isOnTrigger && !intersects) {
		m_isOnTrigger = false;
	}
	if (m_isOnTrigger) return;
	if (!m_data.isTriggerable) return;
	if (m_data.isKeyGuarded && !g_inputController->isKeyJustPressed(Key::Up)) {
		return;
	}
	if (intersects) {
		for (auto& content : m_data.content) {
			TriggerContent::executeTrigger(content, m_worldScreen);
		}
		if (!m_data.isPersistent) {
			m_worldScreen->getCharacterCore()->setTriggerTriggered(m_data.worldID, m_data.objectID);
		}
		setDisposed();
	}
}

void Trigger::render(sf::RenderTarget& renderTarget) {
	GameObject::render(renderTarget);
	if (m_data.isKeyGuarded && m_showSprite) {
		renderTarget.draw(m_sprite);
	}
}

GameObjectType Trigger::getConfiguredType() const {
	return GameObjectType::_Overlay;
}

TriggerData& Trigger::getData() {
	return m_data;
}