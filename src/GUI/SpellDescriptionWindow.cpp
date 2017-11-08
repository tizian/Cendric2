#include <iomanip> 

#include "GUI/SpellDescriptionWindow.h"
#include "Spells/SpellCreator.h"

const float SpellDescriptionWindow::WIDTH = 340.f;

inline std::string toStrMaxDecimals(float value, int decimals) {
	std::ostringstream ss;
	ss << std::fixed << std::setprecision(decimals) << value;
	std::string s = ss.str();
	if (decimals > 0 && s[s.find_last_not_of('0')] == '.') {
		s.erase(s.size() - decimals + 1);
	}
	return std::string(s.begin(), s.end());
}

SpellDescriptionWindow::SpellDescriptionWindow() : Window(
	sf::FloatRect(0.f, 0.f, WIDTH, WIDTH),
	GUIOrnamentStyle::LARGE,
	GUIConstants::MAIN_COLOR,
	GUIConstants::ORNAMENT_COLOR) {
	m_titleText.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_titleText.setColor(COLOR_WHITE);

	m_descriptionText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	m_descriptionText.setColor(COLOR_LIGHT_GREY);

	m_whiteText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	m_coloredText.setColor(COLOR_WHITE);

	m_coloredText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	m_coloredText.setColor(COLOR_LIGHT_PURPLE);
}

void SpellDescriptionWindow::reload(SpellID id, const std::vector<SpellModifier>& modifiers, const AttributeData& attributes) {
	SpellData bean = SpellData::getSpellData(id);
	std::string strengthName;
	int strengthValue;

	SpellCreator* creator = SpellData::getSpellCreator(bean, modifiers, nullptr);
	bean = creator->getSpellData();
	creator->updateDamageAndHeal(bean, &attributes, false);
	strengthName = creator->getStrengthModifierName();
	strengthValue = creator->getStrengthModifierValue();

	delete creator;

	m_titleText.setString(g_textProvider->getText(EnumNames::getSpellIDName(bean.id)));
	m_descriptionText.setString(g_textProvider->getCroppedText(EnumNames::getSpellIDName(bean.id) + "Desc", GUIConstants::CHARACTER_SIZE_S, static_cast<int>(WIDTH - 2 * GUIConstants::TEXT_OFFSET)));

	int lines = 0;
	std::string white = "";
	std::string colored = "";

	// cooldown
	colored.append(g_textProvider->getText("Cooldown"));
	colored.append(": ");
	colored.append(toStrMaxDecimals(bean.cooldown.asSeconds(), 1));
	colored.append("s\n");
	white.append("\n");
	lines++;


	// damage type & damage
	if (bean.damageType != DamageType::VOID) {
		colored.append(g_textProvider->getText("DamageType"));
		colored.append(": ");
		colored.append(g_textProvider->getText(EnumNames::getDamageTypeName(bean.damageType)));
		colored.append("\n");
		white.append("\n");
		lines++;

		if (bean.damage > 0) {
			colored.append(g_textProvider->getText("Damage"));
			colored.append(": ");
			colored.append(std::to_string(bean.damage));
			colored.append("\n");
			white.append("\n");
			lines++;
		}

		if (bean.damagePerSecond > 0) {
			colored.append(g_textProvider->getText("DamagePerSecond"));
			colored.append(": ");
			colored.append(std::to_string(bean.damagePerSecond));
			colored.append("/s\n");
			white.append("\n");
			lines++;
		}
	}

	// heal
	if (bean.heal > 0) {
		colored.append(g_textProvider->getText("Heal"));
		colored.append(": ");
		colored.append(std::to_string(bean.heal));
		colored.append("\n");
		white.append("\n");
		lines++;
	}

	// reflection
	if (bean.reflectCount > 0) {
		colored.append(g_textProvider->getText("Reflection"));
		colored.append(": ");
		colored.append(std::to_string(bean.reflectCount));
		colored.append("\n");
		white.append("\n");
		lines++;
	}

	// speed
	if (bean.speed > 0) {
		colored.append(g_textProvider->getText("Speed"));
		colored.append(": ");
		colored.append(toStrMaxDecimals(bean.speed, 1));
		colored.append("\n");
		white.append("\n");
		lines++;
	}

	// count (is only displayed when there can be count modifier additions)
	if (bean.countModifierAddition > 0) {
		colored.append(g_textProvider->getText("Count"));
		colored.append(": ");
		colored.append(std::to_string(bean.count));
		colored.append("\n");
		white.append("\n");
		lines++;
	}

	// duration (is only displayed when duration is bigger than zero)
	if (bean.duration > sf::Time::Zero) {
		colored.append(g_textProvider->getText("Duration"));
		colored.append(": ");
		colored.append(toStrMaxDecimals(bean.duration.asSeconds(), 1));
		colored.append("s\n");
		white.append("\n");
		lines++;
	}

	// range (is only displayed when there can be range modifier additions)
	if (bean.rangeModifierAddition > 0.f) {
		colored.append(g_textProvider->getText("Range"));
		colored.append(": ");
		colored.append(toStrMaxDecimals(bean.range, 1));
		colored.append("\n");
		white.append("\n");
		lines++;
	}

	// strength (only if there is a strength modifier name set)
	if (!strengthName.empty()) {
		colored.append(g_textProvider->getText(strengthName));
		colored.append(": ");
		colored.append(std::to_string(strengthValue));
		colored.append("\n");
		white.append("\n");
		lines++;
	}

	// allowed modifiers
	white.append("\n");
	white.append("<<< " + g_textProvider->getText("AllowedModifiers") + " >>>");
	white.append("\n");
	lines += 2;

	for (auto& it : SpellData::getAllowedModifiers(id)) {
		white.append(g_textProvider->getText(EnumNames::getSpellModifierTypeName(it)));
		white.append("\n");
		lines++;
	}

	m_whiteText.setString(white);
	m_coloredText.setString(colored);

	float height = GUIConstants::TEXT_OFFSET;
	height += m_titleText.getLocalBounds().height + GUIConstants::CHARACTER_SIZE_S;
	height += m_descriptionText.getLocalBounds().height + 2 * GUIConstants::CHARACTER_SIZE_S;
	height += lines * GUIConstants::CHARACTER_SIZE_S + (lines - 1) * 0.5f * GUIConstants::CHARACTER_SIZE_S;
	height += GUIConstants::TEXT_OFFSET;
	
	setHeight(height);
	setPosition(getPosition());
}

void SpellDescriptionWindow::show() {
	m_isVisible = true;
}

void SpellDescriptionWindow::hide() {
	m_isVisible = false;
}

void SpellDescriptionWindow::setPosition(const sf::Vector2f& position) {
	Window::setPosition(position);

	sf::Vector2f pos(position);
	pos.x += GUIConstants::TEXT_OFFSET;
	pos.y += GUIConstants::TEXT_OFFSET;

	m_titleText.setPosition(pos);

	pos.y += m_titleText.getLocalBounds().height + GUIConstants::CHARACTER_SIZE_S;

	m_descriptionText.setPosition(pos);

	pos.y += m_descriptionText.getLocalBounds().height + 2 * GUIConstants::CHARACTER_SIZE_S;

	m_whiteText.setPosition(pos);
	m_coloredText.setPosition(pos);
}

void SpellDescriptionWindow::render(sf::RenderTarget& renderTarget) {
	if (!m_isVisible) return;
	Window::render(renderTarget);
	renderTarget.draw(m_titleText);
	renderTarget.draw(m_descriptionText);
	renderTarget.draw(m_whiteText);
	renderTarget.draw(m_coloredText);
}