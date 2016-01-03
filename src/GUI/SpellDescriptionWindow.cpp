#include <iomanip> 

#include "GUI/SpellDescriptionWindow.h"
#include "SpellCreator.h"

using namespace std;

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
	WindowOrnamentStyle::FANCY,
	GUIConstants::MAIN_COLOR,
	GUIConstants::BACK_COLOR,
	GUIConstants::ORNAMENT_COLOR) {
	m_titleText.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_titleText.setColor(CENDRIC_COLOR_WHITE);

	m_descriptionText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	m_descriptionText.setColor(CENDRIC_COLOR_LIGHT_GREY);

	m_statsText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	m_statsText.setColor(CENDRIC_COLOR_WHITE);
}

void SpellDescriptionWindow::reload(SpellID id, const std::vector<SpellModifier>& modifiers, const AttributeData* attributes) {
	SpellData bean = SpellData::getSpellData(id);
	std::string strengthName;
	int strengthValue;

	SpellCreator* creator = SpellData::getSpellCreator(bean, modifiers, nullptr);
	bean = creator->getSpellData();
	creator->updateDamage(bean, attributes);
	strengthName = creator->getStrengthModifierName();
	strengthValue = creator->getStrengthModifierValue();

	delete creator;

	m_titleText.setString(g_textProvider->getText(EnumNames::getSpellIDName(bean.id)));
	m_descriptionText.setString(g_textProvider->getCroppedText(EnumNames::getSpellIDName(bean.id) + "Desc", GUIConstants::CHARACTER_SIZE_S, static_cast<int>(WIDTH - 2 * GUIConstants::TEXT_OFFSET)));

	string stats = "\n";

	// cooldown
	stats.append(g_textProvider->getText("Cooldown"));
	stats.append(": ");
	stats.append(toStrMaxDecimals(bean.cooldown.asSeconds(), 1));
	stats.append("s\n");

	// damage type & damage
	if (bean.damageType != DamageType::VOID) {
		stats.append(g_textProvider->getText("DamageType"));
		stats.append(": ");
		stats.append(g_textProvider->getText(EnumNames::getDamageTypeName(bean.damageType)));
		stats.append("\n");

		if (bean.damage > 0) {
			stats.append(g_textProvider->getText("Damage"));
			stats.append(": ");
			stats.append(to_string(bean.damage));
			stats.append("\n");
		}

		if (bean.damagePerSecond > 0) {
			stats.append(g_textProvider->getText("DamagePerSecond"));
			stats.append(": ");
			stats.append(to_string(bean.damagePerSecond));
			stats.append("/s\n");
		}
	}

	// heal
	if (bean.heal > 0) {
		stats.append(g_textProvider->getText("Heal"));
		stats.append(": ");
		stats.append(to_string(bean.heal));
		stats.append("\n");
	}

	// reflection
	if (bean.reflectCount > 0) {
		stats.append(g_textProvider->getText("Reflection"));
		stats.append(": ");
		stats.append(to_string(bean.reflectCount));
		stats.append("\n");
	}

	// speed
	if (bean.startVelocity > 0) {
		stats.append(g_textProvider->getText("Speed"));
		stats.append(": ");
		stats.append(toStrMaxDecimals(bean.startVelocity, 1));
		stats.append("\n");
	}

	// count (is only displayed when there can be count modifier additions)
	if (bean.countModifierAddition > 0) {
		stats.append(g_textProvider->getText("Count"));
		stats.append(": ");
		stats.append(to_string(bean.count));
		stats.append("\n");
	}

	// duration (is only displayed when there can be duration modifier additions)
	if (bean.durationModifierAddition > sf::Time::Zero) {
		stats.append(g_textProvider->getText("Duration"));
		stats.append(": ");
		stats.append(toStrMaxDecimals(bean.duration.asSeconds(), 1));
		stats.append("\n");
	}

	// range (is only displayed when there can be range modifier additions)
	if (bean.rangeModifierAddition > 0.f) {
		stats.append(g_textProvider->getText("Range"));
		stats.append(": ");
		stats.append(toStrMaxDecimals(bean.range, 1));
		stats.append("\n");
	}

	// strength (only if there is a strength modifier name set)
	if (!strengthName.empty()) {
		stats.append(g_textProvider->getText(strengthName));
		stats.append(": ");
		stats.append(to_string(strengthValue));
		stats.append("\n");
	}

	// allowed modifiers
	stats.append("\n");
	stats.append("<<< " + g_textProvider->getText("AllowedModifiers") + " >>>");
	stats.append("\n");

	for (auto& it : SpellData::getAllowedModifiers(id)) {
		stats.append(g_textProvider->getText(EnumNames::getSpellModifierTypeName(it)));
		stats.append("\n");
	}

	m_statsText.setString(stats);

	float height = 2 * GUIConstants::TEXT_OFFSET;
	height += m_titleText.getLocalBounds().height + GUIConstants::CHARACTER_SIZE_S * 2;
	height += m_descriptionText.getLocalBounds().height + GUIConstants::CHARACTER_SIZE_S;
	height += m_statsText.getLocalBounds().height;
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
	float y = position.y + GUIConstants::TEXT_OFFSET;
	m_titleText.setPosition(position.x + GUIConstants::TEXT_OFFSET, y);
	y += m_titleText.getLocalBounds().height + GUIConstants::CHARACTER_SIZE_S;
	m_descriptionText.setPosition(position.x + GUIConstants::TEXT_OFFSET, y);
	y += m_descriptionText.getLocalBounds().height + GUIConstants::CHARACTER_SIZE_S;
	m_statsText.setPosition(position.x + GUIConstants::TEXT_OFFSET, y);
}

void SpellDescriptionWindow::render(sf::RenderTarget& renderTarget) {
	if (!m_isVisible) return;
	Window::render(renderTarget);
	renderTarget.draw(m_titleText);
	renderTarget.draw(m_descriptionText);
	renderTarget.draw(m_statsText);
}