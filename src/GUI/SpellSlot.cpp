#include "GUI/SpellSlot.h"
#include "GUI/GUIConstants.h"
#include "ResourceManager.h"
#include "GlobalResource.h"

const float SpellSlot::SIZE = 86.f;
const float SpellSlot::ICON_OFFSET = 18.f;
const float SpellSlot::GEM_SIZE = 10.f;

SpellSlot::~SpellSlot() {
	m_gems.clear();
}

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

SpellSlot::SpellSlot(SpellID id, bool isMeta) {
	m_spellID = id;
	const SpellData& bean = SpellData::getSpellData(id);
	m_iconTextureRect = bean.iconTextureRect;
	m_spellType = isMeta ? SpellType::Meta : bean.spellType;
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
	m_inputKey.setCharacterSize((m_inputKey.getLocalBounds().width > SIZE - 10.f) ?
		GUIConstants::CHARACTER_SIZE_S :
		GUIConstants::CHARACTER_SIZE_L);
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

	m_cooldownRect.setSize(ICON_SIZE, ICON_SIZE);
	m_cooldownRect.setFillColor(sf::Color(200, 200, 200, 128));

	m_lockedRect.setSize(sf::Vector2f(SIZE, SIZE));
	m_lockedRect.setTexture(g_resourceManager->getTexture(GlobalResource::TEX_GUI_SLOT_SPELL_LOCKED));

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

	if (m_isEmpty) {
		switch (m_spellType) {
		case SpellType::Elemental:
			m_iconRect.setFillColor(COLOR_ELEMENTAL_INACTIVE);
			break;
		case SpellType::Twilight:
			m_iconRect.setFillColor(COLOR_TWILIGHT_INACTIVE);
			break;
		case SpellType::Necromancy:
			m_iconRect.setFillColor(COLOR_NECROMANCY_INACTIVE);
			break;
		case SpellType::Divine:
			m_iconRect.setFillColor(COLOR_DIVINE_INACTIVE);
			break;
		default:
			m_iconRect.setFillColor(COLOR_WHITE);
			break;
		}
	}

	initSlot();
}

void SpellSlot::setPosition(const sf::Vector2f& pos) {
	Slot::setPosition(pos);
	sf::Vector2f positionOffset(SpellSlot::ICON_SIZE / 2.f - m_inputKey.getLocalBounds().width / 2.f, SpellSlot::ICON_SIZE + 18.f);
	m_inputKey.setPosition(pos + positionOffset);
	m_lockedRect.setPosition(pos.x - getConfiguredIconOffset(), pos.y - getConfiguredIconOffset());
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
	if (m_isLocked) {
		renderTarget.draw(m_lockedRect);
	}
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
	Slot::update(frameTime);
}

void SpellSlot::setLocked(bool isLocked) {
	if (m_isEmpty) return;
	m_isLocked = isLocked;
	m_iconRect.setFillColor(m_isLocked ? COLOR_TRANS_WHITE : COLOR_WHITE);
}

bool SpellSlot::isLocked() const {
	return m_isLocked;
}

void SpellSlot::select() {
	if (m_isSelected || m_isEmpty || m_isLocked) return;
	Slot::select();
	m_inputKey.setColor(COLOR_BAD);
}

void SpellSlot::deselect() {
	if (!m_isSelected || m_isEmpty) return;
	Slot::deselect();
	m_inputKey.setColor(COLOR_WHITE);
}
