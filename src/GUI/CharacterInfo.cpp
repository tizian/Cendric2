#include "GUI/CharacterInfo.h"

int round_int(float r) {
	return static_cast<int>((r > 0.0) ? (r + 0.5) : (r - 0.5));
}

CharacterInfo::CharacterInfo(const AttributeBean* attributes)
{
	m_attributes = attributes;

	// init text
	std::wstring names = L"";
	names.append(g_textProvider->getText("Health") + L":\n");
	names.append(g_textProvider->getText("HealthRegeneration") + L":\n");
	names.append(g_textProvider->getText("CriticalHitChance") + L":\n");
	names.append(g_textProvider->getText("Haste") + L":\n\n");

	// dmg 
	names.append(g_textProvider->getText("PhysicalDamage") + L":\n");
	names.append(g_textProvider->getText("FireDamage") + L":\n");
	names.append(g_textProvider->getText("IceDamage") + L":\n");
	names.append(g_textProvider->getText("ShadowDamage") + L":\n");
	names.append(g_textProvider->getText("LightDamage") + L":\n\n");

	// resistance
	names.append(g_textProvider->getText("Armor") + L":\n");
	names.append(g_textProvider->getText("FireResistance") + L":\n");
	names.append(g_textProvider->getText("IceResistance") + L":\n");
	names.append(g_textProvider->getText("ShadowResistance") + L":\n");
	names.append(g_textProvider->getText("LightResistance") + L":\n");

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

CharacterInfo::~CharacterInfo()
{
	delete m_window;
}

bool CharacterInfo::isVisible() const
{
	return m_isVisible;
}

void CharacterInfo::render(sf::RenderTarget& target) const
{
	if (m_isVisible)
	{
		m_window->render(target);
		target.draw(m_title);
		target.draw(m_namesText);
		target.draw(m_attributeText);
	}
}

void CharacterInfo::reload()
{
	// update attributes
	std::wstring attributes = L"";
	attributes.append(std::to_wstring(m_attributes->currentHealthPoints));
	attributes.append(L"/");
	attributes.append(std::to_wstring(m_attributes->maxHealthPoints));
	attributes.append(L"\n");

	// health regeneration
	attributes.append(std::to_wstring(m_attributes->healthRegenerationPerS));
	attributes.append(L"/s\n");

	// crit
	attributes.append(std::to_wstring(m_attributes->criticalHitChance));
	attributes.append(L"%\n");

	// cooldown reduction
	attributes.append(std::to_wstring(m_attributes->haste));
	attributes.append(L" - ");
	attributes.append(std::to_wstring(-round_int(m_attributes->cooldownMultiplier * 100.f - 100.f)));
	attributes.append(L"% " + g_textProvider->getText("CooldownReduction"));
	attributes.append(L"\n\n");

	// dmg 
	attributes.append(std::to_wstring(m_attributes->damagePhysical) + L"\n");
	attributes.append(std::to_wstring(m_attributes->damageFire) + L"\n");
	attributes.append(std::to_wstring(m_attributes->damageIce) + L"\n");
	attributes.append(std::to_wstring(m_attributes->damageShadow) + L"\n");
	attributes.append(std::to_wstring(m_attributes->damageLight) + L"\n\n");

	// resistance
	attributes.append(std::to_wstring(m_attributes->resistancePhysical));
	attributes.append(L" - ");
	attributes.append(std::to_wstring(-round_int(m_attributes->physicalMultiplier * 100.f - 100.f)));
	attributes.append(L"% " + g_textProvider->getText("Reduction"));
	attributes.append(L"\n");

	attributes.append(std::to_wstring(m_attributes->resistanceFire));
	attributes.append(L" - ");
	attributes.append(std::to_wstring(-round_int(m_attributes->fireMultiplier * 100.f - 100.f)));
	attributes.append(L"% " + g_textProvider->getText("Reduction"));
	attributes.append(L"\n");

	attributes.append(std::to_wstring(m_attributes->resistanceIce));
	attributes.append(L" - ");
	attributes.append(std::to_wstring(-round_int(m_attributes->iceMultiplier * 100.f - 100.f)));
	attributes.append(L"% " + g_textProvider->getText("Reduction"));
	attributes.append(L"\n");

	attributes.append(std::to_wstring(m_attributes->resistanceShadow));
	attributes.append(L" - ");
	attributes.append(std::to_wstring(-round_int(m_attributes->shadowMultiplier * 100.f - 100.f)));
	attributes.append(L"% " + g_textProvider->getText("Reduction"));
	attributes.append(L"\n");

	attributes.append(std::to_wstring(m_attributes->resistanceLight));
	attributes.append(L" - ");
	attributes.append(std::to_wstring(-round_int(m_attributes->lightMultiplier * 100.f - 100.f)));
	attributes.append(L"% " + g_textProvider->getText("Reduction"));
	attributes.append(L"\n");
	m_attributeText.setString(attributes);

	m_window->setWidth(3 * GUIConstants::TEXT_OFFSET + m_namesText.getLocalBounds().width + m_attributeText.getLocalBounds().width);
}

void CharacterInfo::show()
{
	m_isVisible = true;
	reload();
}

void CharacterInfo::hide()
{
	m_isVisible = false;
}