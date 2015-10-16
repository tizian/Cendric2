#include "GUI/CharacterInfo.h"

int round_int(float r) {
	return static_cast<int>((r > 0.0) ? (r + 0.5) : (r - 0.5));
}

CharacterInfo::CharacterInfo(const AttributeBean* attributes) {
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
	m_namesText.setColor(sf::Color::White);

	m_attributeText.setString("");
	m_attributeText.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_attributeText.setColor(CENDRIC_COLOR_LIGHT_PURPLE);

	m_title = BitmapText(g_textProvider->getText("CharacterInfo"));
	m_title.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_title.setColor(sf::Color::White);

	float width = 3 * GUIConstants::TEXT_OFFSET + 2 * m_namesText.getLocalBounds().width;
	float height = 3 * GUIConstants::TEXT_OFFSET + m_title.getLocalBounds().height + m_namesText.getLocalBounds().height;

	m_title.setPosition(sf::Vector2f(
		GUIConstants::LEFT + ((width - m_title.getLocalBounds().width) / 2.f),
		GUIConstants::TOP + GUIConstants::TEXT_OFFSET));
	m_namesText.setPosition(sf::Vector2f(
		GUIConstants::LEFT + GUIConstants::TEXT_OFFSET,
		m_title.getPosition().y + m_title.getLocalBounds().height + GUIConstants::TEXT_OFFSET));
	m_attributeText.setPosition(sf::Vector2f(
		GUIConstants::LEFT + width / 2.f,
		m_title.getPosition().y + m_title.getLocalBounds().height + GUIConstants::TEXT_OFFSET));

	// init window
	sf::FloatRect box(GUIConstants::LEFT, GUIConstants::TOP, width, height);
	m_window = new Window(box,
		WindowOrnamentStyle::LARGE,
		GUIConstants::MAIN_COLOR,
		GUIConstants::BACK_COLOR,
		GUIConstants::ORNAMENT_COLOR);
}

CharacterInfo::~CharacterInfo() {
	delete m_window;
}

bool CharacterInfo::isVisible() const {
	return m_isVisible;
}

void CharacterInfo::render(sf::RenderTarget& target) const {
	if (m_isVisible) {
		m_window->render(target);
		target.draw(m_title);
		target.draw(m_namesText);
		target.draw(m_attributeText);
	}
}

void CharacterInfo::reload() {
	// update attributes
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
}

void CharacterInfo::show() {
	m_isVisible = true;
	reload();
}

void CharacterInfo::hide() {
	m_isVisible = false;
}