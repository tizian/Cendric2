#include "GUI/CharacterInfo.h"

int round_int(float r) {
	return static_cast<int>((r > 0.0) ? (r + 0.5) : (r - 0.5));
}

CharacterInfo::CharacterInfo(const AttributeBean* attributes)
{
	m_attributes = attributes;

	// init window
	sf::FloatRect box(CHARACTERINFO_LEFT, CHARACTERINFO_TOP, CHARACTERINFO_WIDTH, CHARACTERINFO_HEIGHT);
	sf::Color mainColor = sf::Color(50, 50, 50, 100);
	sf::Color backColor = sf::Color::Transparent;
	m_window = new Window(box, WindowOrnamentStyle::LARGE, mainColor, backColor, CENDRIC_COLOR_PURPLE);

	// init text
	m_characterInfoText = BitmapText("");
	m_characterInfoText.setPosition(sf::Vector2f(CHARACTERINFO_LEFT + TEXT_OFFSET, CHARACTERINFO_TOP + TEXT_OFFSET));
	m_characterInfoText.setColor(sf::Color::White);
	m_characterInfoText.setCharacterSize(12);
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
		target.draw(m_characterInfoText);
	}
}

void CharacterInfo::reload()
{
	// update text
	std::wstring infoText = L"";
	infoText.append(g_textProvider->getText("CharacterInfo"));
	infoText.append(L"\n\n");
	// health
	infoText.append(g_textProvider->getText("Health"));
	infoText.append(L": ");
	infoText.append(std::to_wstring(m_attributes->currentHealthPoints));
	infoText.append(L"/");
	infoText.append(std::to_wstring(m_attributes->maxHealthPoints));
	infoText.append(L"\n");

	// health regeneration
	infoText.append(g_textProvider->getText("HealthRegeneration"));
	infoText.append(L": ");
	infoText.append(std::to_wstring(m_attributes->healthRegenerationPerS));
	infoText.append(L"/s\n");

	// crit
	infoText.append(g_textProvider->getText("CriticalHitChance"));
	infoText.append(L": ");
	infoText.append(std::to_wstring(m_attributes->criticalHitChance));
	infoText.append(L"%\n");

	// cooldown reduction
	infoText.append(g_textProvider->getText("Haste"));
	infoText.append(L": ");
	infoText.append(std::to_wstring(m_attributes->haste));
	infoText.append(L" - ");
	infoText.append(std::to_wstring(-round_int(m_attributes->cooldownMultiplier * 100.f - 100.f)));
	infoText.append(L"% " + g_textProvider->getText("CooldownReduction"));
	infoText.append(L"\n");

	// dmg 
	infoText.append(g_textProvider->getText("PhysicalDamage"));
	infoText.append(L": ");
	infoText.append(std::to_wstring(m_attributes->damagePhysical));
	infoText.append(L"\n");
	infoText.append(g_textProvider->getText("FireDamage"));
	infoText.append(L": ");
	infoText.append(std::to_wstring(m_attributes->damageFire));
	infoText.append(L"\n");
	infoText.append(g_textProvider->getText("IceDamage"));
	infoText.append(L": ");
	infoText.append(std::to_wstring(m_attributes->damageIce));
	infoText.append(L"\n");
	infoText.append(g_textProvider->getText("ShadowDamage"));
	infoText.append(L": ");
	infoText.append(std::to_wstring(m_attributes->damageShadow));
	infoText.append(L"\n");
	infoText.append(g_textProvider->getText("LightDamage"));
	infoText.append(L": ");
	infoText.append(std::to_wstring(m_attributes->damageLight));
	infoText.append(L"\n");

	// resistance
	infoText.append(g_textProvider->getText("Armor"));
	infoText.append(L": ");
	infoText.append(std::to_wstring(m_attributes->resistancePhysical));
	infoText.append(L" - ");
	infoText.append(std::to_wstring(-round_int(m_attributes->physicalMultiplier * 100.f - 100.f)));
	infoText.append(L"% " + g_textProvider->getText("Reduction"));
	infoText.append(L"\n");
	infoText.append(g_textProvider->getText("FireResistance"));
	infoText.append(L": ");
	infoText.append(std::to_wstring(m_attributes->resistanceFire));
	infoText.append(L" - ");
	infoText.append(std::to_wstring(-round_int(m_attributes->fireMultiplier * 100.f - 100.f)));
	infoText.append(L"% " + g_textProvider->getText("Reduction"));
	infoText.append(L"\n");
	infoText.append(g_textProvider->getText("IceResistance"));
	infoText.append(L": ");
	infoText.append(std::to_wstring(m_attributes->resistanceIce));
	infoText.append(L" - ");
	infoText.append(std::to_wstring(-round_int(m_attributes->iceMultiplier * 100.f - 100.f)));
	infoText.append(L"% " + g_textProvider->getText("Reduction"));
	infoText.append(L"\n");
	infoText.append(g_textProvider->getText("ShadowResistance"));
	infoText.append(L": ");
	infoText.append(std::to_wstring(m_attributes->resistanceShadow));
	infoText.append(L" - ");
	infoText.append(std::to_wstring(-round_int(m_attributes->shadowMultiplier * 100.f - 100.f)));
	infoText.append(L"% " + g_textProvider->getText("Reduction"));

	infoText.append(L"\n");
	infoText.append(g_textProvider->getText("LightResistance"));
	infoText.append(L": ");
	infoText.append(std::to_wstring(m_attributes->resistanceLight));
	infoText.append(L" - ");
	infoText.append(std::to_wstring(-round_int(m_attributes->lightMultiplier * 100.f - 100.f)));
	infoText.append(L"% " + g_textProvider->getText("Reduction"));
	infoText.append(L"\n");
	m_characterInfoText.setString(infoText);
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