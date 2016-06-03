#include "GUI/SpellSlot.h"
#include "ResourceManager.h"
#include "GlobalResource.h"

using namespace std;

const float SpellSlot::SIZE = 86.f;
const float SpellSlot::ICON_OFFSET = 18.f;
const float SpellSlot::GEM_SIZE = 10.f;

SpellSlot::SpellSlot(SpellType type) {
	m_spellType = type;
	m_spellID = SpellID::VOID;
	m_isChopSlot = false;
	m_inputKeyID = Key::VOID;
	m_isEmpty = true;

	m_iconTextureRect = sf::IntRect(250, (static_cast<int>(m_spellType) - 1) * 50, 50, 50);
	m_tooltipWindow.setText(
		g_textProvider->getText("EmptySlot") + " (" + 
		g_textProvider->getText(EnumNames::getSpellTypeName(type)) + ")");

	initSpellSlot();
}

SpellSlot::SpellSlot(SpellID id) {
	m_spellID = id;
	const SpellData& bean = SpellData::getSpellData(id);
	m_iconTextureRect = bean.iconTextureRect;
	m_spellType = bean.spellType;
	m_isChopSlot = (id == SpellID::Chop);
	m_inputKeyID = Key::VOID;
	m_isEmpty = false;
	m_tooltipWindow.setText(g_textProvider->getText(EnumNames::getSpellIDName(id)));

	initSpellSlot();
}

SpellSlot::SpellSlot(const SpellData& bean) {
	m_spellID = bean.id;
	m_iconTextureRect = bean.iconTextureRect;
	m_spellType = bean.spellType;
	m_isChopSlot = (bean.id == SpellID::Chop);
	m_inputKeyID = bean.inputKey;
	m_isEmpty = false;
	m_tooltipWindow.setText(g_textProvider->getText(EnumNames::getSpellIDName(bean.id)));

	initSpellSlot();
}

void SpellSlot::initSpellSlot() {
	m_inputKey.setString(m_inputKeyID != Key::VOID ?
		EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap[m_inputKeyID]) :
		"");
	m_inputKey.setCharacterSize(16);
	if (m_inputKey.getLocalBounds().width > SIZE - 10.f) m_inputKey.setCharacterSize(8);
	m_inputKey.setColor(COLOR_WHITE);

	m_borderTexture = g_resourceManager->getTexture(GlobalResource::TEX_GUI_SLOT_SPELL);
	m_borderTextureSelected = g_resourceManager->getTexture(GlobalResource::TEX_GUI_SLOT_SPELL_SELECTED);

	m_highlightTexture = g_resourceManager->getTexture(GlobalResource::TEX_GUI_SLOT_HIGHLIGHT);

	if (m_spellID != SpellID::VOID) {
		m_iconTexture = m_isChopSlot ?
			g_resourceManager->getTexture(GlobalResource::TEX_ITEMS) :
			g_resourceManager->getTexture(GlobalResource::TEX_SPELLICONS);
	}
	else {
		m_iconTexture = g_resourceManager->getTexture(GlobalResource::TEX_SPELLICONS);
	}

	m_cooldownRect.
		setSize(ICON_SIZE, ICON_SIZE);
	m_cooldownRect.setFillColor(sf::Color(200, 200, 200, 128));

	for (int i = 0; i < 4; ++i) {
		sf::RectangleShape gem;
		gem.setSize(sf::Vector2f(GEM_SIZE, GEM_SIZE));
		gem.setTexture(g_resourceManager->getTexture(GlobalResource::TEX_GUI_SLOT_SPELL_GEM));
		m_gems.push_back(gem);
	}

	switch (m_spellType) {
	case SpellType::Elemental:
		for (auto& gem : m_gems) 
			gem.setFillColor(COLOR_ELEMENTAL);
		break;
	case SpellType::Twilight:
		for (auto& gem : m_gems)
			gem.setFillColor(COLOR_TWILIGHT);
		break;
	case SpellType::Necromancy:
		for (auto& gem : m_gems)
			gem.setFillColor(COLOR_NECROMANCY);
		break;
	case SpellType::Divine:
		for (auto& gem : m_gems)
			gem.setFillColor(COLOR_DIVINE);
		break;
	case SpellType::Meta:
		m_gems[0].setFillColor(COLOR_ELEMENTAL);
		m_gems[1].setFillColor(COLOR_TWILIGHT);
		m_gems[2].setFillColor(COLOR_NECROMANCY);
		m_gems[3].setFillColor(COLOR_DIVINE);
		break;
	default:
		for (auto& gem : m_gems)
			gem.setFillColor(COLOR_WHITE);
		break;
	}

	initSlot();
}

void SpellSlot::setPosition(const sf::Vector2f& pos) {
	Slot::setPosition(pos);
	sf::Vector2f positionOffset(SpellSlot::ICON_SIZE / 2.f - m_inputKey.getLocalBounds().width / 2.f, SpellSlot::ICON_SIZE + 20.f);
	m_inputKey.setPosition(pos + positionOffset);
	m_cooldownRect.setPosition(pos);
	m_gems[0].setPosition(pos + sf::Vector2f(20.f, -12.f));
	m_gems[1].setPosition(pos + sf::Vector2f(-12.f, 20.f));
	m_gems[2].setPosition(pos + sf::Vector2f(20.f, 52.f));
	m_gems[3].setPosition(pos + sf::Vector2f(52.f, 20.f));
}

void SpellSlot::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_backgroundRect);
	renderTarget.draw(m_iconRect);
	renderTarget.draw(m_overlayRect);
	if (m_animating) {
		renderTarget.draw(m_cooldownRect);
	}
	renderTarget.draw(m_borderRect);
	for (auto& gem : m_gems)
		renderTarget.draw(gem);
	renderTarget.draw(m_inputKey);
}

void SpellSlot::playAnimation(const sf::Time& cooldown) {
	if (!m_isEmpty) {
		m_cooldown = cooldown;
		m_animationTime = sf::Time::Zero;
		m_animating = true;
	}
}

void SpellSlot::update(const sf::Time& frameTime) {
	float flashTime = 0.05f;	// seconds

	if (m_animating) {
		float t = m_animationTime.asSeconds() / m_cooldown.asSeconds();
		float angle = lerp(t, 0.f, 360.f);
		m_cooldownRect.setAngle(angle);

		m_animationTime += frameTime;

		if (m_animationTime.asSeconds() > m_cooldown.asSeconds()) {
			if (m_animationTime.asSeconds() < m_cooldown.asSeconds() + flashTime) {
				m_cooldownRect.setAngle(0.f);
			}
			else {
				m_animationTime = sf::Time::Zero;
				m_cooldownRect.setAngle(0.f);
				m_animating = false;
			}

		}
	}
	m_isClicked = false;
	m_isRightClicked = false;
	GameObject::update(frameTime);
}

void SpellSlot::select() {
	if (m_isSelected || m_isEmpty) return;
	Slot::select();
	m_inputKey.setColor(COLOR_BAD);
}

void SpellSlot::deselect() {
	if (!m_isSelected || m_isEmpty) return;
	Slot::deselect();
	m_inputKey.setColor(COLOR_WHITE);
}
