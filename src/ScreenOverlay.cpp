#include "ScreenOverlay.h"
#include "TextProvider.h"
#include "Enums/EnumNames.h"
#include "Item.h"
#include "Structs/SpellData.h"

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
	m_title.setTextStyle(TextStyle::Shadowed);

	m_subtitle.setColor(sf::Color(255, 255, 255, (m_fadeTime > sf::Time::Zero) ? 0 : 255));
	m_subtitle.setCharacterSize(32);
	m_subtitle.setTextStyle(TextStyle::Shadowed);

	m_sprite.setPosition(sf::Vector2f(0.f, 0.f));

	m_textureColor = sf::Color(255, 255, 255);

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

	m_sprite.setColor(sf::Color(m_textureColor.r, m_textureColor.g, m_textureColor.b, (sf::Uint8)(scale * 255)));
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

void ScreenOverlay::setTextureRect(const sf::IntRect& rect) {
	m_sprite.setTextureRect(rect);
}

void ScreenOverlay::setTextureColor(const sf::Color& color) {
	m_textureColor = color;
	m_sprite.setColor(color);
}

void ScreenOverlay::setSpritePosition(const sf::Vector2f& position) {
	m_sprite.setPosition(position);
}

void ScreenOverlay::setSpriteScale(const sf::Vector2f& factors) {
	m_sprite.setScale(factors);
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
	ScreenOverlay* questScreenOverlay = new ScreenOverlay(sf::seconds(2.f), sf::seconds(1.f));

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
	ScreenOverlay* spellScreenOverlay = new ScreenOverlay(sf::seconds(2.f), sf::seconds(1.f));

	spellScreenOverlay->setTitleColor(COLOR_MEDIUM_PURPLE);
	spellScreenOverlay->setTitleCharacterSize(32);

	spellScreenOverlay->setTitle("SpellLearned");

	spellScreenOverlay->setSubtitleCharacterSize(24);
	spellScreenOverlay->setSubtitle(EnumNames::getSpellIDName(id));

	spellScreenOverlay->setTexture(ResourceID::Texture_spellicons);

	const SpellData& bean = SpellData::getSpellData(id);
	spellScreenOverlay->setTextureRect(bean.iconTextureRect);

	spellScreenOverlay->setSpriteScale(sf::Vector2f(2.f, 2.f));
	spellScreenOverlay->setSpritePosition(sf::Vector2f(0.5f * (WINDOW_WIDTH - 100), 0.5f * (WINDOW_HEIGHT - 100)));

	return spellScreenOverlay;
}

ScreenOverlay* ScreenOverlay::createModifierLearnedLearnedScreenOverlay(const SpellModifier& modifier) {
	ScreenOverlay* modifierScreenOverlay = new ScreenOverlay(sf::seconds(2.f), sf::seconds(1.f));

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

ScreenOverlay* ScreenOverlay::createPermanentItemScreenOverlay(const Item& item) {
	ScreenOverlay* itemScreenOverlay = new ScreenOverlay(sf::seconds(2.f), sf::seconds(1.f));

	itemScreenOverlay->setTitleColor(COLOR_GOOD);
	itemScreenOverlay->setTitleCharacterSize(24);

	std::string title = g_textProvider->getText(item.getID(), "item") + " ";
	title.append(g_textProvider->getText("Consumed"));

	itemScreenOverlay->setTitleRaw(title);

	itemScreenOverlay->setSubtitleCharacterSize(16);

	std::string attributes = "";
	AttributeData::appendAttributes(attributes, item.getAttributes());
	itemScreenOverlay->setSubtitleRaw(attributes);

	return itemScreenOverlay;
}

ScreenOverlay* ScreenOverlay::createHintScreenOverlay(const std::string& hintKey) {
	if (!g_resourceManager->getConfiguration().isDisplayHints) {
		return nullptr;
	}

	ScreenOverlay* hintScreenOverlay = new ScreenOverlay(sf::seconds(2.f), sf::seconds(1.f));

	int characterSize = 16;
	hintScreenOverlay->setTitleColor(COLOR_GOOD);
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
	else if (hintKey.compare("LeaveLevel") == 0) {
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Up)) + " ");
	}
	else if (hintKey.compare("Scout") == 0) {
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Up)) + " ");
		hintText.append(g_textProvider->getText("And") + " ");
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Down)) + " ");
	}
	else if (hintKey.compare("Map") == 0) {
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Map)) + " ");
	}
	else if (hintKey.compare("Highlight") == 0) {
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::ToggleTooltips)) + " ");
	}
	else if (hintKey.compare("MapMove") == 0) {
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Up)) + ", ");
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Left)) + ", ");
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Down)) + " ");
		hintText.append(g_textProvider->getText("And") + " ");
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Right)) + " ");
	}
	else {
		hintText.clear();
	}
	hintText.append(g_textProvider->getText(hintKey, "hint"));
	hintText = g_textProvider->getCroppedString(hintText, characterSize, static_cast<int>(0.6f * WINDOW_WIDTH));
	hintScreenOverlay->setSubtitleRaw(hintText);

	return hintScreenOverlay;
}
