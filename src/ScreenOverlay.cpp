#include "ScreenOverlay.h"
#include "TextProvider.h"
#include "Enums/EnumNames.h"

ScreenOverlay::ScreenOverlay(const sf::Time& activeTime, const sf::Time& fadeTime) : GameObject() {
	m_activeTime = activeTime;
	m_fadeTime = fadeTime;

	load();
}

ScreenOverlay::~ScreenOverlay() {
}

void ScreenOverlay::load() {
	m_title.setColor(sf::Color(255, 255, 255, (m_fadeTime > sf::Time::Zero) ? 0 : 255));
	m_title.setCharacterSize(40);

	m_subtitle.setColor(sf::Color(255, 255, 255, (m_fadeTime > sf::Time::Zero) ? 0 : 255));
	m_subtitle.setCharacterSize(32);

	m_sprite.setPosition(sf::Vector2f(0.f, 0.f));

	m_fadeInTimer = m_fadeTime;
	m_fadeOutTimer = m_fadeTime;

	m_isAlwaysUpdate = true;
	m_isInputInDefaultView = true;
	setBoundingBox(sf::FloatRect(0.f, 0.f, WINDOW_WIDTH, WINDOW_HEIGHT));
}

void ScreenOverlay::update(const sf::Time& frameTime) {
	if (m_fadeInTimer == sf::Time::Zero && m_activeTime > sf::Time::Zero) {
		GameObject::updateTime(m_activeTime, frameTime);
		return;
	}

	float scale;
	const sf::Color& tc = m_title.getColor();
	const sf::Color& stc = m_subtitle.getColor();

	if (m_fadeInTimer > sf::Time::Zero) {
		GameObject::updateTime(m_fadeInTimer, frameTime);
		scale = 1.f - m_fadeInTimer.asSeconds() / m_fadeTime.asSeconds();
	}
	else if (m_fadeOutTimer > sf::Time::Zero) {
		GameObject::updateTime(m_fadeOutTimer, frameTime);
		scale = m_fadeOutTimer.asSeconds() / m_fadeTime.asSeconds();
	}
	else {
		setDisposed();
		return;
	}

	m_sprite.setColor(sf::Color(255, 255, 255, (sf::Uint8)(scale * 255)));
	m_title.setColor(sf::Color(tc.r, tc.g, tc.b, (sf::Uint8)(scale * 255)));
	m_subtitle.setColor(sf::Color(stc.r, stc.g, stc.b, (sf::Uint8)(scale * 255)));
}

void ScreenOverlay::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_sprite);
	renderTarget.draw(m_title);
	renderTarget.draw(m_subtitle);
}

void ScreenOverlay::setTexture(ResourceID texture) {
	m_sprite.setTexture(*g_resourceManager->getTexture(texture));
}

void ScreenOverlay::setTitle(const std::string& textKey, const std::string& textType) {
	setTitleRaw(g_textProvider->getText(textKey, textType));
}

void ScreenOverlay::setTitleRaw(const std::string& text) {
	m_title.setString(text);
	repositionText();
}

void ScreenOverlay::setTitleCharacterSize(int characterSize) {
	m_title.setCharacterSize(characterSize);
	repositionText();
}

void ScreenOverlay::setTitleColor(const sf::Color& color) {
	m_title.setColor(color);
}

void ScreenOverlay::setSubtitle(const std::string& textKey, const std::string& textType) {
	setSubtitleRaw(g_textProvider->getText(textKey, textType));
}

void ScreenOverlay::setSubtitleRaw(const std::string& text) {
	m_subtitle.setString(text);
	repositionText();
}

void ScreenOverlay::setSubtitleCharacterSize(int characterSize) {
	m_subtitle.setCharacterSize(characterSize);
	repositionText();
}

void ScreenOverlay::setSubtitleColor(const sf::Color& color) {
	m_subtitle.setColor(color);
}

void ScreenOverlay::repositionText() {
	sf::FloatRect& titleBounds = m_title.getLocalBounds();
	m_title.setPosition(0.5f * (WINDOW_WIDTH - titleBounds.width), 0.3f * (WINDOW_HEIGHT - titleBounds.height));
	sf::FloatRect& subtitleBounds = m_subtitle.getLocalBounds();
	m_subtitle.setPosition(0.5f * (WINDOW_WIDTH - subtitleBounds.width), m_title.getPosition().y + titleBounds.height + subtitleBounds.height);
}

GameObjectType ScreenOverlay::getConfiguredType() const {
	return GameObjectType::_ScreenOverlay;
}

ScreenOverlay* ScreenOverlay::createQuestScreenOverlay(const std::string& questID, QuestState state) {
	ScreenOverlay* questScreenOverlay = new ScreenOverlay(sf::seconds(2.f), sf::seconds(1.f));

	std::string titleText = g_textProvider->getText("Quest") + " ";
	titleText.append(g_textProvider->getText(EnumNames::getQuestStateName(state)));
	questScreenOverlay->setTitleColor(state == QuestState::Completed ? sf::Color::Green : state == QuestState::Failed ? sf::Color::Red : sf::Color::Yellow);
	questScreenOverlay->setTitleRaw(titleText);

	questScreenOverlay->setSubtitle(questID, "quest");

	return questScreenOverlay;
}

ScreenOverlay* ScreenOverlay::createLocationScreenOverlay(const std::string& locationKey) {
	ScreenOverlay* locationScreenOverlay = new ScreenOverlay(sf::seconds(1.f), sf::seconds(0.5f));

	locationScreenOverlay->setTitle(locationKey, "location");
	return locationScreenOverlay;
}

ScreenOverlay* ScreenOverlay::createHintScreenOverlay(const std::string& hintKey) {
	if (!g_resourceManager->getConfiguration().isDisplayHints) {
		return nullptr;
	}

	ScreenOverlay* hintScreenOverlay = new ScreenOverlay(sf::seconds(2.f), sf::seconds(1.f));

	int characterSize = 16;
	hintScreenOverlay->setTitleColor(sf::Color::Green);
	hintScreenOverlay->setTitle("Hint");
	hintScreenOverlay->setTitleCharacterSize(24);

	hintScreenOverlay->setSubtitleCharacterSize(characterSize);

	std::string hintText = (g_textProvider->getText("Press", "hint")) + " ";

	if (hintKey.compare("Inventory") == 0) {
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Inventory)) + " ");
	}
	else if (hintKey.compare("Chop") == 0) {
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Chop)) + " ");
	}
	else if (hintKey.compare("Journal") == 0) {
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Journal)) + " ");
	}
	else {
		hintText.clear();
	}
	hintText.append(g_textProvider->getCroppedText(hintKey, "hint", characterSize, WINDOW_WIDTH - 200));
	hintScreenOverlay->setSubtitleRaw(hintText);

	return hintScreenOverlay;
}
