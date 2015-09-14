#include <iomanip>

#include "GUI/InventoryDescriptionWindow.h"

using namespace std;

const float InventoryDescriptionWindow::WIDTH = 340.f;

inline std::wstring toStrMaxDecimals(float value, int decimals)
{
	std::ostringstream ss;
	ss << std::fixed << std::setprecision(decimals) << value;
	std::string s = ss.str();
	if (decimals > 0 && s[s.find_last_not_of('0')] == '.') {
		s.erase(s.size() - decimals + 1);
	}
	return std::wstring(s.begin(), s.end());
}

InventoryDescriptionWindow::InventoryDescriptionWindow(const Item& item) : Window(
	sf::FloatRect(0.f, 0.f, WIDTH, WIDTH),
	WindowOrnamentStyle::LARGE,
	GUIConstants::MAIN_COLOR,
	GUIConstants::BACK_COLOR,
	GUIConstants::ORNAMENT_COLOR)
{
	m_titleText.setString(g_textProvider->getText(item.getID()));
	m_titleText.setCharacterSize(12);
	m_titleText.setColor(CENDRIC_COLOR_WHITE);
	
	m_descriptionText.setString(g_textProvider->getCroppedText(item.getDescription(), GUIConstants::CHARACTER_SIZE_S, static_cast<int>(WIDTH - 2 * GUIConstants::TEXT_OFFSET)));
	m_descriptionText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	m_descriptionText.setColor(CENDRIC_COLOR_LIGHT_GREY);

	wstring stats = L"\n";
	const AttributeBean& attr = item.getAttributes();
	stats.append(getAttributeText("HealthRegenerationPerS", attr.healthRegenerationPerS));
	stats.append(getAttributeText("Haste", attr.haste));
	stats.append(getAttributeText("Critical", attr.critical));
	stats.append(getAttributeText("PhysicalDamage", attr.damagePhysical));
	stats.append(getAttributeText("FireDamage", attr.damageFire));
	stats.append(getAttributeText("IceDamage", attr.damageIce));
	stats.append(getAttributeText("LightDamage", attr.damageLight));
	stats.append(getAttributeText("ShadowDamage", attr.damageShadow));
	stats.append(getAttributeText("Armor", attr.resistancePhysical));
	stats.append(getAttributeText("FireResistance", attr.resistanceFire));
	stats.append(getAttributeText("IceResistance", attr.resistanceIce));
	stats.append(getAttributeText("LightResistance", attr.resistanceLight));
	stats.append(getAttributeText("ShadowResistance", attr.resistanceShadow));
	stats.append(L"\n");

	if (item.getBean().foodDuration > sf::Time::Zero)
	{
		stats.append(g_textProvider->getText("Duration"));
		stats.append(L": ");
		stats.append(to_wstring(static_cast<int>(floor(item.getBean().foodDuration.asSeconds()))));
		stats.append(L" s\n");
	} 

	stats.append(g_textProvider->getText("GoldValue"));
	stats.append(L": ");
	stats.append(to_wstring(item.getBean().goldValue));

	if (item.getType() == ItemType::Equipment_weapon)
	{
		stats.append(L"\n\n");

		stats.append(g_textProvider->getText("WeaponDamage"));
		stats.append(L": ");
		stats.append(to_wstring(item.getBean().weaponChopDamage));
		stats.append(L"\n");

		stats.append(g_textProvider->getText("Cooldown"));
		stats.append(L": ");
		stats.append(toStrMaxDecimals(item.getBean().weaponChopCooldown.asSeconds(), 1));
		stats.append(L"s\n");

		if (item.getBean().weaponSlots.size() > 0)
		{
			stats.append(L"\n");
			stats.append(L"<<< " + g_textProvider->getText("SpellSlots") + L" >>>\n");
			for (auto& it : item.getBean().weaponSlots)
			{
				stats.append(g_textProvider->getText(EnumNames::getSpellTypeName(it.type)));
				stats.append(L" - " + g_textProvider->getText("GemSockets") + L": ");
				stats.append(to_wstring(it.modifierCount) + L"\n");
			}
		}
	}

	m_statsText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	m_statsText.setColor(CENDRIC_COLOR_WHITE);
	m_statsText.setString(stats);

	float height = 2 * GUIConstants::TEXT_OFFSET;
	height += m_titleText.getLocalBounds().height + GUIConstants::CHARACTER_SIZE_S * 2;
	height += m_descriptionText.getLocalBounds().height + GUIConstants::CHARACTER_SIZE_S;
	height += m_statsText.getLocalBounds().height;
	setHeight(height);
}

std::wstring InventoryDescriptionWindow::getAttributeText(const std::string& name, int value)
{
	if (value == 0) return L"";
	wstring s;
	s.append(g_textProvider->getText(name));
	s.append(L": ");
	s.append(to_wstring(value));
	s.append(L"\n");
	return s;
}

void InventoryDescriptionWindow::setPosition(const sf::Vector2f& position)
{
	Window::setPosition(position);
	float y = position.y + GUIConstants::TEXT_OFFSET;
	m_titleText.setPosition(position.x + GUIConstants::TEXT_OFFSET, y);
	y += m_titleText.getLocalBounds().height + GUIConstants::CHARACTER_SIZE_S;
	m_descriptionText.setPosition(position.x + GUIConstants::TEXT_OFFSET, y);
	y += m_descriptionText.getLocalBounds().height + GUIConstants::CHARACTER_SIZE_S;
	m_statsText.setPosition(position.x + GUIConstants::TEXT_OFFSET, y);
}

void InventoryDescriptionWindow::render(sf::RenderTarget& renderTarget)
{
	Window::render(renderTarget);
	renderTarget.draw(m_titleText);
	renderTarget.draw(m_descriptionText);
	renderTarget.draw(m_statsText);
}


