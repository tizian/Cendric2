#include "ScreenOverlay.h"

ScreenOverlay::ScreenOverlay(const std::string& text, const sf::Time& activeTime, const sf::Time& fadeTime) : GameObject() {
	m_bitmapText.setString(text);
	m_activeTime = activeTime;
	m_fadeTime = fadeTime;

	load();
}

ScreenOverlay::ScreenOverlay(ResourceID texture, const sf::Time& activeTime, const sf::Time& fadeTime) : GameObject() {
	m_sprite.setTexture(*g_resourceManager->getTexture(texture));
	m_bitmapText.setString("");
	m_activeTime = activeTime;
	m_fadeTime = fadeTime;

	load();
}

ScreenOverlay::ScreenOverlay(const std::string& text, ResourceID texture, const sf::Time& activeTime, const sf::Time& fadeTime) : GameObject() {
	m_bitmapText.setString(text);
	m_sprite.setTexture(*g_resourceManager->getTexture(texture));
	m_activeTime = activeTime;
	m_fadeTime = fadeTime;

	load();
}

ScreenOverlay::~ScreenOverlay() {
}

void ScreenOverlay::load() {
	m_bitmapText.setColor(sf::Color(255, 255, 255, (m_fadeTime > sf::Time::Zero) ? 0 : 255));
	m_bitmapText.setCharacterSize(40);
	centerText();

	m_sprite.setPosition(sf::Vector2f(0.f, 0.f));

	m_fadeInTimer = m_fadeTime;
	m_fadeOutTimer = m_fadeTime;

	m_isAlwaysUpdate = true;
	m_isInputInDefaultView = true;
	setBoundingBox(sf::FloatRect(0.f, 0.f, WINDOW_WIDTH, WINDOW_HEIGHT));
}

void ScreenOverlay::update(const sf::Time& frameTime) {
	if (m_fadeInTimer > sf::Time::Zero) {
		GameObject::updateTime(m_fadeInTimer, frameTime);
		float scale = 1.f - m_fadeInTimer.asSeconds() / m_fadeTime.asSeconds();
		m_sprite.setColor(sf::Color(255, 255, 255, (sf::Uint8)(scale * 255)));
		const sf::Color& c = m_bitmapText.getColor();
		m_bitmapText.setColor(sf::Color(c.r, c.g, c.b, (sf::Uint8)(scale * 255)));

		if (m_fadeInTimer == sf::Time::Zero) {
			m_sprite.setColor(sf::Color(255, 255, 255));
			const sf::Color& c = m_bitmapText.getColor();
			m_bitmapText.setColor(sf::Color(c.r, c.g, c.b));
		}
	}
	else if (m_activeTime > sf::Time::Zero) {
		GameObject::updateTime(m_activeTime, frameTime);
	}
	else if (m_fadeOutTimer > sf::Time::Zero) {
		GameObject::updateTime(m_fadeOutTimer, frameTime);
		float scale = m_fadeOutTimer.asSeconds() / m_fadeTime.asSeconds();
		m_sprite.setColor(sf::Color(255, 255, 255, (sf::Uint8)(scale * 255)));
		const sf::Color &c = m_bitmapText.getColor();
		m_bitmapText.setColor(sf::Color(c.r, c.g, c.b, (sf::Uint8)(scale * 255)));
	}
	else {
		setDisposed();
	}
}

void ScreenOverlay::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_sprite);
	renderTarget.draw(m_bitmapText);
}

void ScreenOverlay::setTexture(ResourceID texture) {
	m_sprite.setTexture(*g_resourceManager->getTexture(texture));
}

void ScreenOverlay::setText(const std::string& text) {
	m_bitmapText.setString(text);
	centerText();
}

void ScreenOverlay::setTextSize(int characterSize) {
	m_bitmapText.setCharacterSize(characterSize);
	centerText();
}

void ScreenOverlay::setTextColor(const sf::Color& color) {
	m_bitmapText.setColor(color);
}

void ScreenOverlay::centerText() {
	sf::FloatRect& bounds = m_bitmapText.getLocalBounds();
	m_bitmapText.setPosition(0.5f * (WINDOW_WIDTH - bounds.width), 0.33f * (WINDOW_HEIGHT - bounds.height));
}

GameObjectType ScreenOverlay::getConfiguredType() const {
	return GameObjectType::_ScreenOverlay;
}