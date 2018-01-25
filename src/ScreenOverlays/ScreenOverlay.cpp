#include "ScreenOverlays/ScreenOverlay.h"
#include "ScreenOverlays/TextureScreenOverlay.h"
#include "ScreenOverlays/EnemyDefeatedScreenOverlay.h"
#include "ScreenOverlays/WindowScreenOverlay.h"
#include "TextProvider.h"
#include "Enums/EnumNames.h"
#include "Enums/Language.h"
#include "World/Item.h"
#include "Structs/SpellData.h"
#include "GUI/Hints.h"
#include "GlobalResource.h"

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
	m_title.setTextAlignment(TextAlignment::Center);

	m_subtitle.setColor(sf::Color(255, 255, 255, (m_fadeTime > sf::Time::Zero) ? 0 : 255));
	m_subtitle.setCharacterSize(32);
	m_subtitle.setTextStyle(TextStyle::Shadowed);
	m_subtitle.setTextAlignment(TextAlignment::Center);

	m_textPositionPercentage = 0.3f;

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
	m_title.setColor(sf::Color(color.r, color.g, color.b, (sf::Uint8)(m_scale * 255)));
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
	m_subtitle.setColor(sf::Color(color.r, color.g, color.b, (sf::Uint8)(m_scale * 255)));
}

void ScreenOverlay::setTextPosition(float percentage) {
	m_textPositionPercentage = percentage;
}

void ScreenOverlay::repositionText() {
	const sf::FloatRect& titleBounds = m_title.getLocalBounds();
	m_title.setPosition(0.5f * (WINDOW_WIDTH - titleBounds.width), m_textPositionPercentage * (WINDOW_HEIGHT - titleBounds.height));
	const sf::FloatRect& subtitleBounds = m_subtitle.getLocalBounds();
	m_subtitle.setPosition(0.5f * (WINDOW_WIDTH - subtitleBounds.width), m_title.getPosition().y + titleBounds.height + subtitleBounds.height);
}

GameObjectType ScreenOverlay::getConfiguredType() const {
	return _ScreenOverlay;
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

ScreenOverlay* ScreenOverlay::createLocationScreenOverlay(const std::string& locationKey, bool isBossLevel, bool isObserved) {
	ScreenOverlay* locationScreenOverlay = new ScreenOverlay(sf::seconds(isBossLevel ? 2.f : 1.f), sf::seconds(isBossLevel ? 1.f : 0.5f));

	locationScreenOverlay->setTitle(locationKey, "location");
	if (isObserved) {
		std::string subtitle = "\n(" + g_textProvider->getText("Observed") + ")";
		locationScreenOverlay->setSubtitleRaw(subtitle);
		locationScreenOverlay->setSubtitleColor(COLOR_BAD);
	}

	return locationScreenOverlay;
}

ScreenOverlay* ScreenOverlay::createSpellLearnedScreenOverlay(SpellID id) {
	TextureScreenOverlay* spellScreenOverlay = new TextureScreenOverlay(sf::seconds(3.f), sf::seconds(1.f));

	spellScreenOverlay->setTitleColor(COLOR_MEDIUM_PURPLE);
	spellScreenOverlay->setTitleCharacterSize(32);

	spellScreenOverlay->setTitle("SpellLearned");

	spellScreenOverlay->setSubtitleCharacterSize(32);
	spellScreenOverlay->setSubtitle(EnumNames::getSpellIDName(id));

	spellScreenOverlay->setSpriteTexture(g_resourceManager->getTexture(GlobalResource::TEX_SPELLICONS));
	spellScreenOverlay->setBackgroundTexture(g_resourceManager->getTexture(GlobalResource::TEX_SPELLSCROLL));

	const SpellData& bean = SpellData::getSpellData(id);
	spellScreenOverlay->setSpriteTextureRect(bean.iconTextureRect);

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

	modifierScreenOverlay->setSpriteTexture(g_resourceManager->getTexture(GlobalResource::TEX_GEMS));
	modifierScreenOverlay->setSpriteTextureRect(sf::IntRect((modifier.level - 1) * 50, 50, 50, 50));
	modifierScreenOverlay->setSpriteTextureColor(SpellModifier::getSpellModifierColor(modifier.type));

	modifierScreenOverlay->setSpriteScale(sf::Vector2f(2.f, 2.f));
	modifierScreenOverlay->setSpritePosition(sf::Vector2f(0.5f * (WINDOW_WIDTH - 100), 0.5f * (WINDOW_HEIGHT - 100)));

	return modifierScreenOverlay;
}

ScreenOverlay* ScreenOverlay::createGameOverScreenOverlay() {
	TextureScreenOverlay* gameOverScreenOverlay = new TextureScreenOverlay(sf::seconds(1.f), sf::seconds(2.f));
	gameOverScreenOverlay->setPermanent(true);

	gameOverScreenOverlay->setBackgroundTexture(g_resourceManager->getTexture(GlobalResource::TEX_SCREEN_GAMEOVER));

	const sf::Texture* text = g_resourceManager->getTexture(GlobalResource::TEX_TEXT_GAMEOVER);
	gameOverScreenOverlay->setSpriteTexture(text);
	gameOverScreenOverlay->setSpriteScale(sf::Vector2f(4.f, 4.f));
	gameOverScreenOverlay->setSpritePosition(sf::Vector2f(0.5f * (WINDOW_WIDTH - 4.f * text->getSize().x), 300.f - 0.5f * 4.f * 60.f));

	Language language = g_resourceManager->getConfiguration().language;
	gameOverScreenOverlay->setSpriteTextureRect(sf::IntRect(0,
		(static_cast<int>(language) - 1) * 60,
		static_cast<int>(text->getSize().x), 60));

	return gameOverScreenOverlay;
}

ScreenOverlay* ScreenOverlay::createGamePausedScreenOverlay() {
	TextureScreenOverlay* gamePausedScreenOverlay = new TextureScreenOverlay(sf::milliseconds(1), sf::seconds(0.1f));
	gamePausedScreenOverlay->setPermanent(true);

	gamePausedScreenOverlay->setBackgroundTexture(g_resourceManager->getTexture(GlobalResource::TEX_SCREEN_OVERLAY));

	const sf::Texture* text = g_resourceManager->getTexture(GlobalResource::TEX_TEXT_GAMEPAUSED);
	gamePausedScreenOverlay->setSpriteTexture(text);
	gamePausedScreenOverlay->setSpriteScale(sf::Vector2f(4.f, 4.f));
	gamePausedScreenOverlay->setSpritePosition(sf::Vector2f(0.5f * (WINDOW_WIDTH - 4.f * text->getSize().x), 200.f - 0.5f * 4.f * 60.f));

	Language language = g_resourceManager->getConfiguration().language;
	if (language == Language::Lang_EN) {
		gamePausedScreenOverlay->setSpriteTextureRect(sf::IntRect(0, 0, static_cast<int>(text->getSize().x), 60));
	}
	else if (language == Language::Lang_DE || language == Language::Lang_CH) {
		gamePausedScreenOverlay->setSpriteTextureRect(sf::IntRect(0, 60, static_cast<int>(text->getSize().x), 60));
	}
	else if (language == Language::LANG_ES) {
		gamePausedScreenOverlay->setSpriteTextureRect(sf::IntRect(0, 120, static_cast<int>(text->getSize().x), 60));
	}

	return gamePausedScreenOverlay;
}

ScreenOverlay* ScreenOverlay::createEnemyDefeatedScreenOverlay(std::map<std::string, int>& items, int gold) {
	EnemyDefeatedScreenOverlay* enemyDefeatedScreenOverlay = new EnemyDefeatedScreenOverlay(sf::seconds(3.f), sf::seconds(2.f));

	enemyDefeatedScreenOverlay->setBackgroundTexture(g_resourceManager->getTexture(GlobalResource::TEX_SCREEN_OVERLAY));

	const sf::Texture* text = g_resourceManager->getTexture(GlobalResource::TEX_TEXT_DEFEATED);
	enemyDefeatedScreenOverlay->setSpriteTexture(text);
	enemyDefeatedScreenOverlay->setSpriteScale(sf::Vector2f(3.f, 3.f));
	enemyDefeatedScreenOverlay->setSpritePosition(sf::Vector2f(0.5f * (WINDOW_WIDTH - 3.f * text->getSize().x), 200.f - 0.5f * 3.f * 60.f));

	Language language = g_resourceManager->getConfiguration().language;
	enemyDefeatedScreenOverlay->setSpriteTextureRect(sf::IntRect(0,
		(static_cast<int>(language) - 1) * 60,
		static_cast<int>(text->getSize().x), 60));

	enemyDefeatedScreenOverlay->setLoot(items, gold);
	return enemyDefeatedScreenOverlay;
}

ScreenOverlay* ScreenOverlay::createPermanentItemScreenOverlay(const Item* item) {
	if (item == nullptr) return nullptr;
	TextureScreenOverlay* itemScreenOverlay = new TextureScreenOverlay(sf::seconds(2.f), sf::seconds(1.f));

	itemScreenOverlay->setTitleColor(COLOR_GOOD);
	itemScreenOverlay->setTitleCharacterSize(24);

	std::string title = g_textProvider->getText(item->getID(), "item") + " ";
	title.append(g_textProvider->getText("Consumed"));

	itemScreenOverlay->setTitleRaw(title);

	itemScreenOverlay->setSubtitleCharacterSize(16);

	itemScreenOverlay->setSpriteTexture(g_resourceManager->getTexture(GlobalResource::TEX_ITEMS));
	itemScreenOverlay->setSpriteTextureRect(sf::IntRect(item->getIconTextureLocation().x, item->getIconTextureLocation().y, 50, 50));

	itemScreenOverlay->setSpriteScale(sf::Vector2f(2.f, 2.f));
	itemScreenOverlay->setSpritePosition(sf::Vector2f(0.5f * (WINDOW_WIDTH - 100), 0.5f * (WINDOW_HEIGHT - 100)));

	std::string attributes = "";
	AttributeData::appendAttributes(attributes, item->getAttributes());
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

ScreenOverlay* ScreenOverlay::createArrestedScreenOverlay() {
	TextureScreenOverlay* arrestedScreenOverlay = new TextureScreenOverlay(sf::seconds(1.f), sf::seconds(2.f));
	arrestedScreenOverlay->setPermanent(true);

	arrestedScreenOverlay->setBackgroundTexture(g_resourceManager->getTexture(GlobalResource::TEX_SCREEN_OVERLAY));

	const sf::Texture* text = g_resourceManager->getTexture(GlobalResource::TEX_TEXT_ARRESTED);
	arrestedScreenOverlay->setSpriteTexture(text);
	arrestedScreenOverlay->setSpriteScale(sf::Vector2f(4.f, 4.f));
	arrestedScreenOverlay->setSpritePosition(sf::Vector2f(0.5f * (WINDOW_WIDTH - 4.f * text->getSize().x), 300.f - 0.5f * 4.f * 60.f));

	Language language = g_resourceManager->getConfiguration().language;
	arrestedScreenOverlay->setSpriteTextureRect(sf::IntRect(0,
		(static_cast<int>(language) - 1) * 60,
		static_cast<int>(text->getSize().x), 60));

	return arrestedScreenOverlay;
}

ScreenOverlay* ScreenOverlay::createGuildJoinedScreenOverlay(FractionID id) {
	TextureScreenOverlay* guildScreenOverlay = new TextureScreenOverlay(sf::seconds(3.f), sf::seconds(1.f));

	guildScreenOverlay->setTextPosition(0.15f);

	guildScreenOverlay->setTitleColor(COLOR_WHITE);
	guildScreenOverlay->setTitleCharacterSize(32);

	guildScreenOverlay->setTitle("GuildJoined");

	guildScreenOverlay->setSubtitleCharacterSize(32);
	guildScreenOverlay->setSubtitle(EnumNames::getFractionIDName(id));

	guildScreenOverlay->setSpriteTexture(g_resourceManager->getTexture(GlobalResource::TEX_GUILD_BANNERS));

	if (id == FractionID::Cleric) {
		guildScreenOverlay->setSubtitleColor(COLOR_DIVINE);
	}
	else if (id == FractionID::Necromancer) {
		guildScreenOverlay->setSubtitleColor(COLOR_NECROMANCY);
	}
	else if (id == FractionID::Thief) {
		guildScreenOverlay->setSubtitleColor(COLOR_TWILIGHT);
	}

	sf::IntRect texRect((static_cast<int>(id) - 1) * 146, 0, 146, 162);
	guildScreenOverlay->setSpriteTextureRect(texRect);

	sf::Vector2f scale(2.f, 2.f);
	guildScreenOverlay->setSpriteScale(scale);

	sf::Vector2f pos(0.5f * (WINDOW_WIDTH - 2.f * 146), 0.35f * (WINDOW_HEIGHT - 50));
	guildScreenOverlay->setSpritePosition(pos);

	return guildScreenOverlay;
}