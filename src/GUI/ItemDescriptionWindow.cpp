#include <iomanip>
#include <sstream>

#include "GUI/ItemDescriptionWindow.h"

using namespace std;

const float ItemDescriptionWindow::WIDTH = 340.f;
const float ItemDescriptionWindow::ICON_OFFSET = 24 * 8.f;

inline std::string toStrMaxDecimals(float value, int decimals) {
	std::ostringstream ss;
	ss << std::fixed << std::setprecision(decimals) << value;
	std::string s = ss.str();
	if (decimals > 0 && s[s.find_last_not_of('0')] == '.') {
		s.erase(s.size() - decimals + 1);
	}
	return std::string(s.begin(), s.end());
}

ItemDescriptionWindow::ItemDescriptionWindow() : Window(
	sf::FloatRect(0.f, 0.f, WIDTH, WIDTH),
	GUIOrnamentStyle::LARGE,
	GUIConstants::MAIN_COLOR,
	GUIConstants::ORNAMENT_COLOR) {
	m_titleText.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_titleText.setColor(COLOR_WHITE);

	m_descriptionText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	m_descriptionText.setColor(COLOR_LIGHT_GREY);

	m_whiteText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	m_greenText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	m_redText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	
	m_whiteText.setColor(COLOR_WHITE);
	m_greenText.setColor(COLOR_GOOD);
	m_redText.setColor(COLOR_BAD);
}

ItemDescriptionWindow::~ItemDescriptionWindow() {}

std::string ItemDescriptionWindow::getGoldText(const Item& item) const {
	std::string text;
	text.append(g_textProvider->getText("GoldValue"));
	text.append(": ");
	text.append(item.getValue() < 0 ? g_textProvider->getText("Unsalable") : to_string(item.getValue()));
	return text;
}

void ItemDescriptionWindow::setPosition(const sf::Vector2f& position) {
	Window::setPosition(position);

	sf::Vector2f pos(position);
	pos.x += GUIConstants::TEXT_OFFSET;
	pos.y += GUIConstants::TEXT_OFFSET;

	m_titleText.setPosition(pos);

	pos.y += m_titleText.getLocalBounds().height + GUIConstants::CHARACTER_SIZE_S;

	m_descriptionText.setPosition(pos);

	pos.y += m_descriptionText.getLocalBounds().height + 2 * GUIConstants::CHARACTER_SIZE_S;

	sf::Vector2f statsOrigin(pos);

	m_whiteText.setPosition(statsOrigin);
	m_greenText.setPosition(statsOrigin);
	m_redText.setPosition(statsOrigin);
}

void ItemDescriptionWindow::load(const Item& item) {
	m_titleText.setString(g_textProvider->getText(item.getID(), "item"));
	m_descriptionText.setString(g_textProvider->getCroppedText(item.getID(), "item_desc", GUIConstants::CHARACTER_SIZE_S, static_cast<int>(WIDTH - 2 * GUIConstants::TEXT_OFFSET)));

	int lines = 0;
	std::string white = "";
	std::string green = "";
	std::string red = "";

	if (item.getType() == ItemType::Permanent) {
		green.append(g_textProvider->getText("Permanent"));
		green.append("\n\n");
		white.append("\n\n");
		red.append("\n\n");
		lines += 2;
	}

	if (item.getType() == ItemType::Equipment_weapon && item.isWeapon()) {
		Weapon weapon(item.getID());

		white.append(g_textProvider->getText("WeaponDamage"));
		white.append(": ");
		white.append(to_string(weapon.getWeaponChopDamage()));
		white.append("\n");

		white.append(g_textProvider->getText("Cooldown"));
		white.append(": ");
		white.append(toStrMaxDecimals(weapon.getWeaponCooldown().asSeconds(), 1));
		white.append("s\n\n");

		green.append("\n\n\n");
		red.append("\n\n\n");
		lines += 3;
	}

	const AttributeData& attr = item.getAttributes();

	int numberPositive, numberNegative;
	AttributeData::appendPositiveAttributes(green, attr, numberPositive);
	red.append(std::string(numberPositive, '\n'));
	AttributeData::appendNegativeAttributes(red, attr, numberNegative);
	green.append(std::string(numberNegative, '\n'));
	white.append(std::string(numberPositive + numberNegative, '\n'));
	lines += numberPositive + numberNegative;

	if (item.getFoodDuration() > sf::Time::Zero) {
		white.append("\n");
		green.append("\n");
		red.append("\n");
		lines++;

		white.append(g_textProvider->getText("Duration"));
		white.append(": ");
		white.append(to_string(static_cast<int>(floor(item.getFoodDuration().asSeconds()))));
		white.append(" s\n");

		green.append("\n");
		red.append("\n");
		lines++;
	}

	if (item.getType() == ItemType::Equipment_weapon && item.isWeapon()) {
		Weapon weapon(item.getID());
		if (weapon.getWeaponSlots().size() > 0) {
			white.append("\n");
			white.append("<<< " + g_textProvider->getText("SpellSlots") + " >>>\n");
			lines += 2;
			for (auto& it : weapon.getWeaponSlots()) {
				white.append(g_textProvider->getText(EnumNames::getSpellTypeName(it.spellSlot.spellType)));
				white.append(": ");
				white.append(to_string(it.spellModifiers.size()));
				white.append(" ");
				white.append(g_textProvider->getText("GemSockets"));
				white.append("\n");
				lines++;
			}
		}
	}

	if (lines > 0) {
		white.append("\n");
		green.append("\n");
		red.append("\n");
		lines++;
	}
	
	white.append(getGoldText(item));
	lines++;

	m_whiteText.setString(white);
	m_greenText.setString(green);
	m_redText.setString(red);

	float height = GUIConstants::TEXT_OFFSET;
	height += m_titleText.getLocalBounds().height + GUIConstants::CHARACTER_SIZE_S;
	height += m_descriptionText.getLocalBounds().height + 2 * GUIConstants::CHARACTER_SIZE_S;
	height += lines * GUIConstants::CHARACTER_SIZE_S + (lines - 1) * 0.5f * GUIConstants::CHARACTER_SIZE_S;
	height += GUIConstants::TEXT_OFFSET;

	setHeight(height);
}

void ItemDescriptionWindow::render(sf::RenderTarget& renderTarget) {
	if (!m_isVisible) return;
	Window::render(renderTarget);
	renderTarget.draw(m_titleText);
	renderTarget.draw(m_descriptionText);
	renderTarget.draw(m_whiteText);
	renderTarget.draw(m_greenText);
	renderTarget.draw(m_redText);
}

void ItemDescriptionWindow::show() {
	m_isVisible = true;
}

void ItemDescriptionWindow::hide() {
	m_isVisible = false;
}

bool ItemDescriptionWindow::isVisible() const {
	return m_isVisible;
}


