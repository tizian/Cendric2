#include "GUI/SpeechBubble.h"
#include "GUI/GUIConstants.h"
#include "Level/LevelMovableGameObject.h"
#include "GlobalResource.h"

const float SpeechBubble::MIN_SIZE = 20.f;
const float SpeechBubble::MAX_LENGTH = 250.f;
const float SpeechBubble::TEXT_MARGIN = 10.f;

SpeechBubble::SpeechBubble(LevelMovableGameObject* owner) : GameObject() {
	m_owner = owner;
	setBoundingBox(sf::FloatRect({0.f, 0.f}, {MIN_SIZE, MIN_SIZE}));

	m_mainLayer = SlicedSprite(g_resourceManager->getTexture(GlobalResource::TEX_GUI_ROUNDED_RECTANGLE), COLOR_TRANS_BLACK, m_boundingBox.size.x, m_boundingBox.size.y);
	m_ornamentLayer = SlicedSprite(g_resourceManager->getTexture(GlobalResource::TEX_GUI_ORNAMENT_NONE), COLOR_WHITE, m_boundingBox.size.x, m_boundingBox.size.y);
	m_pointer->setTexture(*g_resourceManager->getTexture(GlobalResource::TEX_GUI_SPEECHBUBBLE_POINTER));

	m_text.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_text.setColor(COLOR_WHITE);
	m_isAlwaysUpdate = true;
}

void SpeechBubble::setPosition(const sf::Vector2f& position) {
	GameObject::setPosition(position);
	sf::Vector2f posOffset;
	if (m_boundingBox.position.x - (g_inputController->getMousePosition().x - g_inputController->getDefaultViewMousePosition().x) + m_boundingBox.size.x > WINDOW_WIDTH) {
		posOffset.x = -m_boundingBox.size.x + m_pointer->getLocalBounds().size.x;
	}
	m_mainLayer.setPosition(position + posOffset);
	m_ornamentLayer.setPosition(position + posOffset);
	m_text.setPosition(position + sf::Vector2f(TEXT_MARGIN, TEXT_MARGIN) + posOffset);
	m_pointer->setPosition({position.x, position.y + m_boundingBox.size.y});
}

void SpeechBubble::setFloatingHeight(float height) {
	m_floatingHeight = height;
}

void SpeechBubble::setText(const std::string& text) {
	m_text.setString(g_textProvider->getCroppedText(text, "speech", GUIConstants::CHARACTER_SIZE_M, static_cast<int>(MAX_LENGTH)));

	sf::Vector2f newSize(m_text.getLocalBounds().size.x, m_text.getLocalBounds().size.y);
	m_boundingBox.size.x = newSize.x + 2 * TEXT_MARGIN;
	m_boundingBox.size.y = newSize.y + 2 * TEXT_MARGIN;

	m_mainLayer.setSize(m_boundingBox.size.x, m_boundingBox.size.y);
	m_ornamentLayer.setSize(m_boundingBox.size.x, m_boundingBox.size.y);
}

void SpeechBubble::render(sf::RenderTarget& renderTarget) {
	if (!m_isVisible) return;
	renderTarget.draw(*m_pointer);
	renderTarget.draw(m_mainLayer);
	renderTarget.draw(m_ornamentLayer);
	renderTarget.draw(m_text);
}

void SpeechBubble::update(const sf::Time& frameTime) {
	if (!m_isVisible) return;

	setPosition(sf::Vector2f(m_owner->getPosition().x, 
		m_owner->getPosition().y - m_boundingBox.size.y - m_floatingHeight));
}

GameObjectType SpeechBubble::getConfiguredType() const {
	return _Overlay;
}

void SpeechBubble::show() {
	m_isVisible = true;
}

void SpeechBubble::hide() {
	m_isVisible = false;
}