#include "World/Trigger.h"
#include "Screens/WorldScreen.h"
#include "GlobalResource.h"

Trigger::Trigger(WorldScreen* screen, const TriggerData& data) {
	m_worldScreen = screen;
	m_mainChar = screen->getMainCharacter();
	m_data = data;
	if (m_data.isKeyGuarded) {
		m_isOnTrigger = false;
		const sf::Texture* texture = g_resourceManager->getTexture(GlobalResource::TEX_GUI_EXIT_ARROW);
		m_sprite.setTexture(*texture);

		float xPos = data.triggerRect.left + 0.5f * (data.triggerRect.width - texture->getSize().x);
		// Avoid aliasing when centering sprite over trigger with odd width
		if (static_cast<int>(data.triggerRect.width) % 2 == 1) {
			xPos += 0.5f;
		}
		m_sprite.setPosition(xPos, 0.f);	
	}

	if (m_data.isForced) {
		m_isOnTrigger = false;
	}

	setBoundingBox(data.triggerRect);
	setDebugBoundingBox(sf::Color::Magenta);
}

void Trigger::update(const sf::Time& frameTime) {
	GameObject::update(frameTime);
	m_time += frameTime;
	m_showSprite = false;

	if (!m_data.isTriggerable) return;

	bool intersects = fastIntersect(*m_mainChar->getBoundingBox(), m_data.triggerRect);
	if (m_data.isKeyGuarded && intersects) {
		m_showSprite = intersects;

		sf::Vector2f pos = m_sprite.getPosition();
		float variance = 4.f;
		float speed = 6.f;
		float offset = variance * std::cos(speed * m_time.asSeconds());
		float y = m_data.triggerRect.top + m_data.triggerRect.height - 2.f * m_mainChar->getSize().y - 0.5f * variance + offset;
		m_sprite.setPosition(pos.x, y);
	}

	if (m_isOnTrigger && !intersects) {
		m_isOnTrigger = false;
	}
	if (m_isOnTrigger) return;
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
	if (m_showSprite) {
		renderTarget.draw(m_sprite);
	}
}

GameObjectType Trigger::getConfiguredType() const {
	return _Overlay;
}

TriggerData& Trigger::getData() {
	return m_data;
}