#include <iomanip> 

#include "GUI/SpellDescriptionWindow.h"
#include "SpellCreator.h"

using namespace std;

const float SpellDescriptionWindow::WIDTH = 340.f;
const float MARGIN = 20.f;
const int CHARACTER_SIZE = 8;

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

SpellDescriptionWindow::SpellDescriptionWindow() : Window(
	sf::FloatRect(0.f, 0.f, WIDTH, WIDTH),
	WindowOrnamentStyle::LARGE,
	CENDRIC_COLOR_TRANS_BLACK, // main
	sf::Color::Transparent, // back
	CENDRIC_COLOR_LIGHT_PURPLE) // ornament
{
	m_titleText.setCharacterSize(12);
	m_titleText.setColor(CENDRIC_COLOR_WHITE);

	m_descriptionText.setCharacterSize(CHARACTER_SIZE);
	m_descriptionText.setColor(CENDRIC_COLOR_LIGHT_GREY);

	m_statsText.setCharacterSize(CHARACTER_SIZE);
	m_statsText.setColor(CENDRIC_COLOR_WHITE);
}

void SpellDescriptionWindow::reload(SpellID id, const std::vector<SpellModifier>& modifiers, const AttributeBean* attributes)
{
	SpellBean bean = SpellBean::getSpellBean(id);
	std::string strengthName;
	int strengthValue;

	SpellCreator* creator = SpellBean::getSpellCreator(bean, modifiers, nullptr);
	bean = creator->getSpellBean();
	creator->updateDamage(bean, attributes);
	strengthName = creator->getStrengthModifierName();
	strengthValue = creator->getStrengthModifierValue();

	delete creator;

	m_titleText.setString(g_textProvider->getText(EnumNames::getSpellIDName(bean.id)));
	m_descriptionText.setString(g_textProvider->getCroppedText(EnumNames::getSpellIDName(bean.id) + "Desc", CHARACTER_SIZE, static_cast<int>(WIDTH - 2 * MARGIN)));

	wstring stats = L"\n";

	// cooldown
	stats.append(g_textProvider->getText("Cooldown"));
	stats.append(L": ");
	stats.append(toStrMaxDecimals(bean.cooldown.asSeconds(), 1));
	stats.append(L"s\n");

	// damage type & damage
	if (bean.damageType != DamageType::VOID)
	{
		stats.append(g_textProvider->getText("DamageType"));
		stats.append(L": ");
		stats.append(g_textProvider->getText(EnumNames::getDamageTypeName(bean.damageType)));
		stats.append(L"\n");

		stats.append(g_textProvider->getText("Damage"));
		stats.append(L": ");
		stats.append(to_wstring(bean.damage));
		stats.append(L"\n");
	}

	// heal
	if (bean.heal > 0)
	{
		stats.append(g_textProvider->getText("Heal"));
		stats.append(L": ");
		stats.append(to_wstring(bean.heal));
		stats.append(L"\n");
	}

	// reflection
	if (bean.reflectCount > 0)
	{
		stats.append(g_textProvider->getText("Reflection"));
		stats.append(L": ");
		stats.append(to_wstring(bean.reflectCount));
		stats.append(L"\n");
	}

	// speed
	if (bean.startVelocity > 0)
	{
		stats.append(g_textProvider->getText("Speed"));
		stats.append(L": ");
		stats.append(toStrMaxDecimals(bean.startVelocity, 1));
		stats.append(L"\n");
	}

	// count (is only displayed when there can be count modifier additions)
	if (bean.countModifierAddition > 0)
	{
		stats.append(g_textProvider->getText("Count"));
		stats.append(L": ");
		stats.append(to_wstring(bean.count));
		stats.append(L"\n");
	}

	// duration (is only displayed when there can be duration modifier additions)
	if (bean.durationModifierAddition > sf::Time::Zero)
	{
		stats.append(g_textProvider->getText("Duration"));
		stats.append(L": ");
		stats.append(toStrMaxDecimals(bean.duration.asSeconds(), 1));
		stats.append(L"\n");
	}

	// range (is only displayed when there can be range modifier additions)
	if (bean.rangeModifierAddition > 0.f)
	{
		stats.append(g_textProvider->getText("Range"));
		stats.append(L": ");
		stats.append(toStrMaxDecimals(bean.range, 1));
		stats.append(L"\n");
	}

	// strength (only if there is a strength modifier name set)
	if (!strengthName.empty())
	{
		stats.append(g_textProvider->getText(strengthName));
		stats.append(L": ");
		stats.append(to_wstring(strengthValue));
		stats.append(L"\n");
	}

	// allowed modifiers
	stats.append(L"\n");
	stats.append(L"<<< " + g_textProvider->getText("AllowedModifiers") + L" >>>");
	stats.append(L"\n");

	for (auto& it : SpellBean::getAllowedModifiers(id))
	{
		stats.append(g_textProvider->getText(EnumNames::getSpellModifierTypeName(it)));
		stats.append(L"\n");
	}

	m_statsText.setString(stats);

	float height = 2 * MARGIN;
	height += m_titleText.getLocalBounds().height + CHARACTER_SIZE * 2;
	height += m_descriptionText.getLocalBounds().height + CHARACTER_SIZE;
	height += m_statsText.getLocalBounds().height;
	setHeight(height);

	setPosition(getPosition());
}

void SpellDescriptionWindow::show()
{
	m_isVisible = true;
}

void SpellDescriptionWindow::hide()
{
	m_isVisible = false;
}

void SpellDescriptionWindow::setPosition(const sf::Vector2f& position)
{
	Window::setPosition(position);
	float y = position.y + MARGIN;
	m_titleText.setPosition(position.x + MARGIN, y);
	y += m_titleText.getLocalBounds().height + CHARACTER_SIZE;
	m_descriptionText.setPosition(position.x + MARGIN, y);
	y += m_descriptionText.getLocalBounds().height + CHARACTER_SIZE;
	m_statsText.setPosition(position.x + MARGIN, y);
}

void SpellDescriptionWindow::render(sf::RenderTarget& renderTarget)
{
	if (!m_isVisible) return;
	Window::render(renderTarget);
	renderTarget.draw(m_titleText);
	renderTarget.draw(m_descriptionText);
	renderTarget.draw(m_statsText);
}