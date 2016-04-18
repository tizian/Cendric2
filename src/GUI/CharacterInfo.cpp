#include "GUI/CharacterInfo.h"

const sf::Vector2f BUTTON_SIZE = sf::Vector2f(200.f, 40.f);

CharacterInfo::CharacterInfo(const CharacterCore* core, const AttributeData* attributes) {
	m_core = core;
	m_attributes = attributes;

	// init text
	std::string names = "";
	names.append(g_textProvider->getText("Health") + ":\n");
	names.append(g_textProvider->getText("HealthRegeneration") + ":\n");
	names.append(g_textProvider->getText("CriticalHitChance") + ":\n");
	names.append(g_textProvider->getText("Haste") + ":\n\n");

	// dmg 
	names.append(g_textProvider->getText("PhysicalDamage") + ":\n");
	names.append(g_textProvider->getText("FireDamage") + ":\n");
	names.append(g_textProvider->getText("IceDamage") + ":\n");
	names.append(g_textProvider->getText("ShadowDamage") + ":\n");
	names.append(g_textProvider->getText("LightDamage") + ":\n\n");

	// resistance
	names.append(g_textProvider->getText("Armor") + ":\n");
	names.append(g_textProvider->getText("FireResistance") + ":\n");
	names.append(g_textProvider->getText("IceResistance") + ":\n");
	names.append(g_textProvider->getText("ShadowResistance") + ":\n");
	names.append(g_textProvider->getText("LightResistance") + ":\n");

	m_namesText.setString(names);
	m_namesText.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_namesText.setColor(COLOR_WHITE);

	m_attributeText.setString("");
	m_attributeText.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_attributeText.setColor(COLOR_LIGHT_PURPLE);

	float width = 3 * GUIConstants::TEXT_OFFSET + 2 * m_namesText.getLocalBounds().width;
	float yOffset = GUIConstants::TOP + 3 * GUIConstants::TEXT_OFFSET + GUIConstants::CHARACTER_SIZE_M + BUTTON_SIZE.y;

	m_namesText.setPosition(sf::Vector2f(
		GUIConstants::LEFT + GUIConstants::TEXT_OFFSET,
		yOffset));
	m_attributeText.setPosition(sf::Vector2f(
		GUIConstants::LEFT + width / 2.f,
		yOffset));

	// init window
	sf::FloatRect box(GUIConstants::LEFT, GUIConstants::TOP, width, 0);
	m_window = new Window(box,
		WindowOrnamentStyle::FANCY,
		GUIConstants::MAIN_COLOR,
		GUIConstants::BACK_COLOR,
		GUIConstants::ORNAMENT_COLOR);

	m_window->addCloseButton(std::bind(&CharacterInfo::hide, this));

	reload();

	// init tab bar
	int nTabs = 2;
	float barWidth = nTabs * BUTTON_SIZE.x;
	float barHeight = BUTTON_SIZE.y;
	float x = GUIConstants::LEFT + 0.5f * (m_window->getSize().x - barWidth);
	float y = GUIConstants::TOP + GUIConstants::GUI_TABS_TOP;

	m_tabBar = new TabBar(sf::FloatRect(x, y, barWidth, barHeight), nTabs);
	m_tabBar->getTabButton(0)->setText("Stats");
	m_tabBar->getTabButton(1)->setText("Reputation");
	for (int i = 0; i < nTabs; ++i) {
		m_tabBar->getTabButton(i)->setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	}

	// init title
	m_title.setPosition(sf::Vector2f(GUIConstants::LEFT + GUIConstants::TEXT_OFFSET, GUIConstants::TOP + GUIConstants::TEXT_OFFSET));
	m_title.setColor(COLOR_WHITE);
	m_title.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_title.setString(g_textProvider->getText("CharacterInfo"));
	m_title.setPosition(
		m_window->getPosition().x +
		m_window->getSize().x / 2 -
		m_title.getLocalBounds().width / 2, m_title.getPosition().y);
}

CharacterInfo::~CharacterInfo() {
	delete m_window;
	delete m_tabBar;
}

bool CharacterInfo::isVisible() const {
	return m_isVisible;
}

void CharacterInfo::update(const sf::Time& frameTime) {
	if (!m_isVisible) return;
	m_window->update(frameTime);
	m_tabBar->update(frameTime);
	if (!m_isReloadNeeded) return;
	reload();
	m_isReloadNeeded = false;
}

void CharacterInfo::render(sf::RenderTarget& target) const {
	if (!m_isVisible) return;
	m_window->render(target);
	target.draw(m_title);
	m_tabBar->render(target);

	if (m_tabBar->getActiveTabIndex() == 0) {
		target.draw(m_namesText);
		target.draw(m_attributeText);
	}
	else {
		for (auto& text : m_reputationTexts) {
			target.draw(text);
		}
	}
}

void CharacterInfo::notifyChange() {
	m_isReloadNeeded = true;
}

void CharacterInfo::reload() {
	updateAttributes();
	updateReputation();
}

void CharacterInfo::updateAttributes() {
	// health
	std::string attributes = "";
	attributes.append(std::to_string(m_attributes->currentHealthPoints));
	attributes.append("/");
	attributes.append(std::to_string(m_attributes->maxHealthPoints));
	attributes.append("\n");

	// health regeneration
	attributes.append(std::to_string(m_attributes->healthRegenerationPerS));
	attributes.append("/s\n");

	// crit
	attributes.append(std::to_string(m_attributes->criticalHitChance));
	attributes.append("%\n");

	// cooldown reduction
	attributes.append(std::to_string(m_attributes->haste));
	attributes.append(" - ");
	attributes.append(std::to_string(-round_int(m_attributes->cooldownMultiplier * 100.f - 100.f)));
	attributes.append("% " + g_textProvider->getText("CooldownReduction"));
	attributes.append("\n\n");

	// dmg 
	attributes.append(std::to_string(m_attributes->damagePhysical) + "\n");
	attributes.append(std::to_string(m_attributes->damageFire) + "\n");
	attributes.append(std::to_string(m_attributes->damageIce) + "\n");
	attributes.append(std::to_string(m_attributes->damageShadow) + "\n");
	attributes.append(std::to_string(m_attributes->damageLight) + "\n\n");

	// resistance
	attributes.append(std::to_string(m_attributes->resistancePhysical));
	attributes.append(" - ");
	attributes.append(std::to_string(-round_int(m_attributes->physicalMultiplier * 100.f - 100.f)));
	attributes.append("% " + g_textProvider->getText("Reduction"));
	attributes.append("\n");

	attributes.append(std::to_string(m_attributes->resistanceFire));
	attributes.append(" - ");
	attributes.append(std::to_string(-round_int(m_attributes->fireMultiplier * 100.f - 100.f)));
	attributes.append("% " + g_textProvider->getText("Reduction"));
	attributes.append("\n");

	attributes.append(std::to_string(m_attributes->resistanceIce));
	attributes.append(" - ");
	attributes.append(std::to_string(-round_int(m_attributes->iceMultiplier * 100.f - 100.f)));
	attributes.append("% " + g_textProvider->getText("Reduction"));
	attributes.append("\n");

	attributes.append(std::to_string(m_attributes->resistanceShadow));
	attributes.append(" - ");
	attributes.append(std::to_string(-round_int(m_attributes->shadowMultiplier * 100.f - 100.f)));
	attributes.append("% " + g_textProvider->getText("Reduction"));
	attributes.append("\n");

	attributes.append(std::to_string(m_attributes->resistanceLight));
	attributes.append(" - ");
	attributes.append(std::to_string(-round_int(m_attributes->lightMultiplier * 100.f - 100.f)));
	attributes.append("% " + g_textProvider->getText("Reduction"));
	attributes.append("\n");
	m_attributeText.setString(attributes);

	m_window->setWidth(3 * GUIConstants::TEXT_OFFSET + m_namesText.getLocalBounds().width + m_attributeText.getLocalBounds().width);
	m_window->setHeight(m_namesText.getLocalBounds().height + 5 * GUIConstants::TEXT_OFFSET + GUIConstants::CHARACTER_SIZE_M + BUTTON_SIZE.y);
}

void CharacterInfo::updateReputation() {
	m_reputationTexts.clear();

	float yOffset = GUIConstants::TOP + 3 * GUIConstants::TEXT_OFFSET + GUIConstants::CHARACTER_SIZE_M + BUTTON_SIZE.y;
	
	if (m_core->getData().reputationProgress.empty()) {
		BitmapText noRep;
		noRep.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
		noRep.setPosition(2 * GUIConstants::TEXT_OFFSET + GUIConstants::LEFT, yOffset);
		noRep.setColor(COLOR_LIGHT_PURPLE);
		noRep.setString(g_textProvider->getCroppedText("NoReputation", GUIConstants::CHARACTER_SIZE_M, static_cast<int>(m_window->getSize().x - 2 * GUIConstants::TEXT_OFFSET)));
		m_reputationTexts.push_back(noRep);
		return;
	}
	
	for (auto const& rep : m_core->getData().reputationProgress) {
		BitmapText title;
		title.setString(g_textProvider->getText(EnumNames::getFractionIDName(rep.first)) + ": " + std::to_string(rep.second));
		title.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
		title.setPosition(2 * GUIConstants::TEXT_OFFSET + GUIConstants::LEFT, yOffset);
		title.setColor(COLOR_WHITE);
		m_reputationTexts.push_back(title);

		yOffset += title.getLocalBounds().height * (3 / 2.f);

		BitmapText subtitle;
		std::string key = EnumNames::getFractionIDName(rep.first) + "_" +
			(rep.second >= 75 ? "100" : rep.second >= 50 ? "75" : rep.second >= 25 ? "50" : "25");
		subtitle.setString(g_textProvider->getCroppedText(key, GUIConstants::CHARACTER_SIZE_M, static_cast<int>(m_window->getSize().x - 2 * GUIConstants::TEXT_OFFSET)));
		subtitle.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
		subtitle.setPosition(2 * GUIConstants::TEXT_OFFSET + GUIConstants::LEFT, yOffset);
		subtitle.setColor(COLOR_LIGHT_PURPLE);
		m_reputationTexts.push_back(subtitle);

		yOffset += subtitle.getLocalBounds().height + 2 * GUIConstants::CHARACTER_SIZE_M;
	}
}

void CharacterInfo::show() {
	m_isVisible = true;
}

void CharacterInfo::hide() {
	m_isVisible = false;
}