#include "GUI/SpeechBubble.h"
#include "GUI/GUIConstants.h"
#include "Level/LevelMovableGameObject.h"

using namespace std;

const float SpeechBubble::MIN_SIZE = 20.f;
const float SpeechBubble::MAX_LENGTH = 150.f;
const float SpeechBubble::TEXT_MARGIN = 5.f;
const float SpeechBubble::FLOATING_HEIGHT = 30.f;

SpeechBubble::SpeechBubble(LevelMovableGameObject* owner) : GameObject() {
	m_owner = owner;
	setBoundingBox(sf::FloatRect(0.f, 0.f, MIN_SIZE, MIN_SIZE));

	m_mainLayer = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_rounded_rectangle), sf::Color::Black, m_boundingBox.width, m_boundingBox.height);
	m_backLayer = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_rounded_rectangle), CENDRIC_COLOR_LIGHT_PURPLE, m_boundingBox.width, m_boundingBox.height);
	m_ornamentLayer = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_ornament_none), sf::Color::White, m_boundingBox.width, m_boundingBox.height);

	m_text.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	m_text.setColor(CENDRIC_COLOR_WHITE);
	m_isAlwaysUpdate = true;
}

void SpeechBubble::setPosition(const sf::Vector2f& position) {
	GameObject::setPosition(position);
	m_mainLayer.setPosition(position);
	m_backLayer.setPosition(position);
	m_ornamentLayer.setPosition(position);
	m_text.setPosition(position + sf::Vector2f(TEXT_MARGIN, TEXT_MARGIN));
	m_pointer.setPosition(sf::Vector2f(position.x, position.y + m_boundingBox.height));
}

void SpeechBubble::setText(const std::string& text) {
	m_text.setString(g_textProvider->getCroppedText(text, "speech", GUIConstants::CHARACTER_SIZE_S, static_cast<int>(MAX_LENGTH)));

	sf::Vector2f newSize(m_text.getLocalBounds().width, m_text.getLocalBounds().height);
	m_boundingBox.width = newSize.x + 2 * TEXT_MARGIN;
	m_boundingBox.height = newSize.y + 2 * TEXT_MARGIN;

	m_mainLayer.setSize(m_boundingBox.width, m_boundingBox.height);
	m_backLayer.setSize(m_boundingBox.width, m_boundingBox.height);
	m_ornamentLayer.setSize(m_boundingBox.width, m_boundingBox.height);
}

void SpeechBubble::render(sf::RenderTarget& renderTarget) {
	if (!m_isVisible) return;
	renderTarget.draw(m_pointer);
	renderTarget.draw(m_backLayer);
	renderTarget.draw(m_mainLayer);
	renderTarget.draw(m_ornamentLayer);
	renderTarget.draw(m_text);
}

void SpeechBubble::update(const sf::Time& frameTime) {
	if (!m_isVisible) return;
	setPosition(sf::Vector2f(m_owner->getPosition().x, 
		m_owner->getPosition().y - m_boundingBox.height - FLOATING_HEIGHT));
}

GameObjectType SpeechBubble::getConfiguredType() const {
	return GameObjectType::_Window;
}

void SpeechBubble::show() {
	m_isVisible = true;
}

void SpeechBubble::hide() {
	m_isVisible = false;
}