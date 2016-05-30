#include "ScreenOverlays/ScreenOverlay.h"
#include "ScreenOverlays/TextureScreenOverlay.h"
#include "ScreenOverlays/WindowScreenOverlay.h"
#include "TextProvider.h"
#include "Enums/EnumNames.h"
#include "Enums/Language.h"
#include "Item.h"
#include "Structs/SpellData.h"
#include "GUI/Hints.h"

ScreenOverlay::ScreenOverlay(const sf::Time& activeTime, const sf::Time& fadeTime) : GameObject() {
	m_activeTime = activeTime;
	m_fadeTime = fadeTime;
	m_isPermanent = false;

	load();
}

void ScreenOverlay::load() {
	m_scale = (m_fadeTime > sf::Time::Zero) ? 0.f : 1.f;
	m_title.setColor(sf::Color(255, 255, 255, (m_fadeTime > sf::Time::Zero) ? 0 : 255));
	m_title.setCharacterSize(40);
	m_title.setTextStyle(TextStyle::Shadowed);

	m_subtitle.setColor(sf::Color(255, 255, 255, (m_fadeTime > sf::Time::Zero) ? 0 : 255));
	m_subtitle.setCharacterSize(32);
	m_subtitle.setTextStyle(TextStyle::Shadowed);

	m_fadeInTimer = m_fadeTime;
	m_fadeOutTimer = m_fadeTime;

	m_isAlwaysUpdate = true;
	m_isInputInDefaultView = true;
	setBoundingBox(sf::FloatRect(0.f, 0.f, WINDOW_WIDTH, WINDOW_HEIGHT));
}

void ScreenOverlay::update(const sf::Time& frameTime) {
	if (m_fadeInTimer == sf::Time::Zero && m_activeTime > sf::Time::Zero) {
		updateTime(m_activeTime, frameTime);
		return;
	}

	const sf::Color& tc = m_title.getColor();
	const sf::Color& stc = m_subtitle.getColor();

	if (m_fadeInTimer > sf::Time::Zero) {
		updateTime(m_fadeInTimer, frameTime);
		m_scale = 1.f - m_fadeInTimer.asSeconds() / m_fadeTime.asSeconds();
	}
	else if (!m_isPermanent && m_fadeOutTimer > sf::Time::Zero) {
		updateTime(m_fadeOutTimer, frameTime);
		m_scale = m_fadeOutTimer.asSeconds() / m_fadeTime.asSeconds();
	}
	else if (!m_isPermanent) {
		setDisposed();
		return;
	}
	
	m_title.setColor(sf::Color(tc.r, tc.g, tc.b, (sf::Uint8)(m_scale * 255)));
	m_subtitle.setColor(sf::Color(stc.r, stc.g, stc.b, (sf::Uint8)(m_scale * 255)));
}

void ScreenOverlay::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_title);
	renderTarget.draw(m_subtitle);
}

void ScreenOverlay::setPermanent(bool permanent) {
	m_isPermanent = permanent;
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
	const sf::FloatRect& titleBounds = m_title.getLocalBounds();
	m_title.setPosition(0.5f * (WINDOW_WIDTH - titleBounds.width), 0.3f * (WINDOW_HEIGHT - titleBounds.height));
	const sf::FloatRect& subtitleBounds = m_subtitle.getLocalBounds();
	m_subtitle.setPosition(0.5f * (WINDOW_WIDTH - subtitleBounds.width), m_title.getPosition().y + titleBounds.height + subtitleBounds.height);
}

GameObjectType ScreenOverlay::getConfiguredType() const {
	return GameObjectType::_ScreenOverlay;
}

ScreenOverlay* ScreenOverlay::createQuestScreenOverlay(const std::string& questID, QuestState state) {
	ScreenOverlay* questScreenOverlay = new ScreenOverlay(sf::seconds(1.5f), sf::seconds(0.5f));

	std::string titleText = g_textProvider->getText("Quest") + " ";
	titleText.append(g_textProvider->getText(EnumNames::getQuestStateName(state)));
	questScreenOverlay->setTitleColor(state == QuestState::Completed ? COLOR_GOOD : state == QuestState::Failed ? COLOR_BAD : COLOR_NEUTRAL);
	questScreenOverlay->setTitleRaw(titleText);
	questScreenOverlay->setTitleCharacterSize(32);

	questScreenOverlay->setSubtitle(questID, "quest");
	questScreenOverlay->setSubtitleCharacterSize(24);

	return questScreenOverlay;
}

ScreenOverlay* ScreenOverlay::createLocationScreenOverlay(const std::string& locationKey) {
	ScreenOverlay* locationScreenOverlay = new ScreenOverlay(sf::seconds(1.f), sf::seconds(0.5f));

	locationScreenOverlay->setTitle(locationKey, "location");
	return locationScreenOverlay;
}

ScreenOverlay* ScreenOverlay::createSpellLearnedScreenOverlay(SpellID id) {
	TextureScreenOverlay* spellScreenOverlay = new TextureScreenOverlay(sf::seconds(3.f), sf::seconds(1.f));

	spellScreenOverlay->setTitleColor(COLOR_MEDIUM_PURPLE);
	spellScreenOverlay->setTitleCharacterSize(32);

	spellScreenOverlay->setTitle("SpellLearned");

	spellScreenOverlay->setSubtitleCharacterSize(32);
	spellScreenOverlay->setSubtitle(EnumNames::getSpellIDName(id));

	spellScreenOverlay->setTexture(ResourceID::Texture_spellicons);
	spellScreenOverlay->setBackgroundTexture(ResourceID::Texture_spellscroll);

	const SpellData& bean = SpellData::getSpellData(id);
	spellScreenOverlay->setTextureRect(bean.iconTextureRect);

	sf::Vector2f scale(2.f, 2.f);
	spellScreenOverlay->setSpriteScale(scale);
	spellScreenOverlay->setBackgroundScale(scale);

	sf::Vector2f pos(0.5f * (WINDOW_WIDTH - 128), 0.5f * (WINDOW_HEIGHT - 50));
	spellScreenOverlay->setSpritePosition(pos);
	spellScreenOverlay->setBackgroundPosition(pos);

	return spellScreenOverlay;
}

ScreenOverlay* ScreenOverlay::createModifierLearnedScreenOverlay(const SpellModifier& modifier) {
	TextureScreenOverlay* modifierScreenOverlay = new TextureScreenOverlay(sf::seconds(3.f), sf::seconds(1.f));

	modifierScreenOverlay->setTitleColor(COLOR_MEDIUM_PURPLE);
	modifierScreenOverlay->setTitleCharacterSize(32);

	modifierScreenOverlay->setTitle("ModifierLearned");

	modifierScreenOverlay->setSubtitleCharacterSize(24);

	std::string subtitle = g_textProvider->getText(EnumNames::getSpellModifierTypeName(modifier.type)) + " ";
	subtitle.append(g_textProvider->getText("Level") + " " + std::to_string(modifier.level));
	modifierScreenOverlay->setSubtitleRaw(subtitle);

	modifierScreenOverlay->setTexture(ResourceID::Texture_gems);
	modifierScreenOverlay->setTextureRect(sf::IntRect((modifier.level - 1) * 50, 50, 50, 50));
	modifierScreenOverlay->setTextureColor(SpellModifier::getSpellModifierColor(modifier.type));

	modifierScreenOverlay->setSpriteScale(sf::Vector2f(2.f, 2.f));
	modifierScreenOverlay->setSpritePosition(sf::Vector2f(0.5f * (WINDOW_WIDTH - 100), 0.5f * (WINDOW_HEIGHT - 100)));

	return modifierScreenOverlay;
}

ScreenOverlay* ScreenOverlay::createGameOverScreenOverlay() {
	TextureScreenOverlay* gameOverScreenOverlay = new TextureScreenOverlay(sf::seconds(1.f), sf::seconds(2.f));
	gameOverScreenOverlay->setPermanent(true);

	Language language = g_resourceManager->getConfiguration().language;
	if (language == Language::Lang_EN) {
		gameOverScreenOverlay->setTexture(ResourceID::Texture_screen_gameover_en);
	}
	else if (language == Language::Lang_DE) {
		gameOverScreenOverlay->setTexture(ResourceID::Texture_screen_gameover_de);
	}
	else if (language == Language::Lang_CH) {
		gameOverScreenOverlay->setTexture(ResourceID::Texture_screen_gameover_ch);
	}

	return gameOverScreenOverlay;
}

ScreenOverlay* ScreenOverlay::createGamePausedScreenOverlay() {
	TextureScreenOverlay* gamePausedScreenOverlay = new TextureScreenOverlay(sf::milliseconds(1), sf::seconds(0.1f));
	gamePausedScreenOverlay->setPermanent(true);

	Language language = g_resourceManager->getConfiguration().language;
	if (language == Language::Lang_EN) {
		gamePausedScreenOverlay->setTexture(ResourceID::Texture_screen_gamepaused_en);
	}
	else if (language == Language::Lang_DE || language == Language::Lang_CH) {
		gamePausedScreenOverlay->setTexture(ResourceID::Texture_screen_gamepaused_chde);
	}

	return gamePausedScreenOverlay;
}

ScreenOverlay* ScreenOverlay::createPermanentItemScreenOverlay(const Item& item) {
	TextureScreenOverlay* itemScreenOverlay = new TextureScreenOverlay(sf::seconds(2.f), sf::seconds(1.f));

	itemScreenOverlay->setTitleColor(COLOR_GOOD);
	itemScreenOverlay->setTitleCharacterSize(24);

	std::string title = g_textProvider->getText(item.getID(), "item") + " ";
	title.append(g_textProvider->getText("Consumed"));

	itemScreenOverlay->setTitleRaw(title);

	itemScreenOverlay->setSubtitleCharacterSize(16);

	itemScreenOverlay->setTexture(ResourceID::Texture_items);
	itemScreenOverlay->setTextureRect(sf::IntRect(item.getIconTextureLocation().x, item.getIconTextureLocation().y, 50, 50));

	itemScreenOverlay->setSpriteScale(sf::Vector2f(2.f, 2.f));
	itemScreenOverlay->setSpritePosition(sf::Vector2f(0.5f * (WINDOW_WIDTH - 100), 0.5f * (WINDOW_HEIGHT - 100)));

	std::string attributes = "";
	AttributeData::appendAttributes(attributes, item.getAttributes());
	itemScreenOverlay->setSubtitleRaw(attributes);

	return itemScreenOverlay;
}

ScreenOverlay* ScreenOverlay::createHintScreenOverlay(const std::string& hintKey) {
	if (!g_resourceManager->getConfiguration().isDisplayHints) {
		return nullptr;
	}

	WindowScreenOverlay* hintScreenOverlay = new WindowScreenOverlay(sf::seconds(8.f), sf::seconds(0.5f));

	int characterSize = 12;
	hintScreenOverlay->setTitleColor(COLOR_GOOD);
	hintScreenOverlay->setTitle("Hint");
	hintScreenOverlay->setTitleCharacterSize(16);

	hintScreenOverlay->setSubtitleCharacterSize(characterSize);

	std::string hintText = getHintDescription(hintKey);
	
	hintText = g_textProvider->getCroppedString(hintText, characterSize, static_cast<int>(0.4f * WINDOW_WIDTH));
	hintScreenOverlay->setSubtitleRaw(hintText);

	return hintScreenOverlay;
}
