#include "GUI/BuffSlot.h"
#include "GUI/GUIConstants.h"
#include "Spell.h"
#include "Enums/EnumNames.h"
#include "ScreenOverlay.h"

using namespace std;

const float BuffSlot::MARGIN = 3.f;
const float BuffSlot::SIZE = 50.f + 2 * MARGIN;
const sf::Time BuffSlot::FLASHING_TIME = sf::seconds(5);
const sf::Time BuffSlot::FLASHING_INTERVAL = sf::seconds(0.5);

const sf::Vector2f TEXT_OFFSET = sf::Vector2f(10.f, 10.f);

BuffSlot::BuffSlot(BuffType type, const sf::IntRect& textureLocation, const sf::Time& duration) : m_tooltipWindow(Window(sf::FloatRect(), WindowOrnamentStyle::NONE, sf::Color(0, 0, 0, 100), sf::Color(0, 0, 0, 100), COLOR_WHITE)) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, SIZE, SIZE));
	setDebugBoundingBox(COLOR_BAD);
	m_buffType = type;
	setInputInDefaultView(true);

	m_duration = duration;
	m_timeUntilFlash = std::max(FLASHING_INTERVAL, duration - FLASHING_TIME);
	m_isVisible = true;

	sf::Texture* texture;

	m_outside.setSize(sf::Vector2f(SIZE, SIZE));
	m_outside.setTexture(g_resourceManager->getTexture(ResourceID::Texture_GUI_slot_buff));

	switch (type) {
	case BuffType::Food:
		m_outside.setFillColor(COLOR_GOOD);
		texture = g_resourceManager->getTexture(ResourceID::Texture_items);
		break;
	case BuffType::Spell:
		texture = g_resourceManager->getTexture(ResourceID::Texture_spellicons);
		break;
	case BuffType::DamageOverTime:
		m_outside.setFillColor(COLOR_BAD);
		texture = g_resourceManager->getTexture(ResourceID::Texture_damageTypes);
		break;
	default:
		break;
	}
	
	m_inside.setSize(sf::Vector2f(50.f, 50.f));
	m_inside.setTexture(texture);
	m_inside.setTextureRect(textureLocation);

	m_back.setSize(sf::Vector2f(50.f, 50.f));
	m_back.setFillColor(COLOR_TRANS_GREY);

	m_durationText.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_tooltipText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
}

void BuffSlot::setPosition(const sf::Vector2f& pos) {
	m_outside.setPosition(pos);
	m_inside.setPosition(pos + sf::Vector2f(MARGIN, MARGIN));
	m_back.setPosition(pos + sf::Vector2f(MARGIN, MARGIN));
	m_durationText.setPosition(pos + sf::Vector2f((SIZE - m_durationText.getLocalBounds().width) / 2.f, SIZE));
	m_tooltipWindow.setPosition(pos + sf::Vector2f(SIZE + 2.f, 0.f));
	m_tooltipText.setPosition(pos + sf::Vector2f(SIZE + 2.f, 0.f) + TEXT_OFFSET);
	GameObject::setPosition(pos);
}

void BuffSlot::render(sf::RenderTarget& renderTarget) {
	if (m_isVisible) {
		renderTarget.draw(m_outside);
		renderTarget.draw(m_back);
		renderTarget.draw(m_inside);
	}

	renderTarget.draw(m_durationText);
}

void BuffSlot::renderAfterForeground(sf::RenderTarget& renderTarget) {
	GameObject::renderAfterForeground(renderTarget);
	if (m_tooltipTime > sf::Time::Zero) {
		m_tooltipWindow.render(renderTarget);
		renderTarget.draw(m_tooltipText);
	}
}

SpellID BuffSlot::getSpellID() const {
	return m_ownerSpell == nullptr ? SpellID::VOID : m_ownerSpell->getSpellID();
}

void BuffSlot::onMouseOver() {
	m_tooltipTime = sf::seconds(0.1f);
}

void BuffSlot::onRightClick() {
	if (m_ownerSpell == nullptr) return;
	m_ownerSpell->setDisposed();
	setDisposed();
}

void BuffSlot::update(const sf::Time& frameTime) {
	// update time
	GameObject::updateTime(m_duration, frameTime);
	GameObject::updateTime(m_tooltipTime, frameTime);
	if (m_duration == sf::Time::Zero) setDisposed();
	GameObject::updateTime(m_timeUntilFlash, frameTime);
	if (m_timeUntilFlash == sf::Time::Zero) {
		m_isVisible = !m_isVisible;
		m_timeUntilFlash = FLASHING_INTERVAL;
	}

	// update duration text
	m_durationText.setString(to_string(static_cast<int>(floor(m_duration.asSeconds()))) + "s");
	m_durationText.setPosition(getPosition() + sf::Vector2f((SIZE - m_durationText.getLocalBounds().width) / 2.f, SIZE));
	GameObject::update(frameTime);
}

GameObjectType BuffSlot::getConfiguredType() const {
	return GameObjectType::_Interface;
}

void BuffSlot::setSpellAttributes(Spell* owner, const AttributeData& attributes) {
	if (m_buffType != BuffType::Spell) return;
	m_ownerSpell = owner;

	switch (m_ownerSpell->getSpellType()) {
	case SpellType::Elemental:
		m_outside.setFillColor(COLOR_ELEMENTAL);
		break;
	case SpellType::Twilight:
		m_outside.setFillColor(COLOR_TWILIGHT);
		break;
	case SpellType::Necromancy:
		m_outside.setFillColor(COLOR_NECROMANCY);
		break;
	case SpellType::Divine:
		m_outside.setFillColor(COLOR_DIVINE);
		break;
	default:
		break;
	}

	std::string tooltip = "";
	tooltip.append(g_textProvider->getText(EnumNames::getSpellIDName(m_ownerSpell->getSpellID())) + "\n\n");
	AttributeData::appendAttributes(tooltip, attributes);

	m_tooltipText.setString(tooltip);
	m_tooltipWindow.setWidth(2 * TEXT_OFFSET.x + m_tooltipText.getLocalBounds().width);
	m_tooltipWindow.setHeight(2 * TEXT_OFFSET.y + m_tooltipText.getLocalBounds().height);
}

void BuffSlot::setFoodAttributes(const std::string& item_id, const AttributeData& attributes) {
	if (m_buffType != BuffType::Food) return;

	std::string tooltip = "";
	tooltip.append(g_textProvider->getText(item_id, "item") + "\n\n");
	AttributeData::appendAttributes(tooltip, attributes);

	m_tooltipText.setString(tooltip);
	m_tooltipWindow.setWidth(2 * TEXT_OFFSET.x + m_tooltipText.getLocalBounds().width);
	m_tooltipWindow.setHeight(2 * TEXT_OFFSET.y + m_tooltipText.getLocalBounds().height);
}

void BuffSlot::setDotAttributes(const DamageOverTimeData& data) {
	if (m_buffType != BuffType::DamageOverTime) return;

	std::string tooltip = "";

	if (data.isFeared) {
		tooltip.append(g_textProvider->getText("Feared") + "\n");
		if (m_duration > sf::Time::Zero && m_screen->getObjects(GameObjectType::_ScreenOverlay)->empty()) {
			ScreenOverlay* fearedOverlay = new ScreenOverlay(m_duration, sf::seconds(0.1f));
			fearedOverlay->setTexture(ResourceID::Texture_screen_overlay_feared);
			m_screen->addObject(fearedOverlay);
		}
	}
	else if (data.isStunned) {
		tooltip.append(g_textProvider->getText("Stunned") + "\n");
		if (m_duration > sf::Time::Zero && m_screen->getObjects(GameObjectType::_ScreenOverlay)->empty()) {
			ScreenOverlay* stunnedOverlay = new ScreenOverlay(m_duration, sf::seconds(0.1f));
			stunnedOverlay->setTexture(ResourceID::Texture_screen_overlay_stunned);
			m_screen->addObject(stunnedOverlay);
		}
	}
	if (data.damageType != DamageType::VOID) {
		tooltip.append(g_textProvider->getText(EnumNames::getDamageTypeName(data.damageType)) + "\n\n");
		tooltip.append(g_textProvider->getText("DamagePerSecond") + ": " + std::to_string(data.damage));
	}

	m_tooltipText.setString(tooltip);
	m_tooltipWindow.setWidth(2 * TEXT_OFFSET.x + m_tooltipText.getLocalBounds().width);
	m_tooltipWindow.setHeight(2 * TEXT_OFFSET.y + m_tooltipText.getLocalBounds().height);
}