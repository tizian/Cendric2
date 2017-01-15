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
	m_coloredText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	
	m_interactionText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	m_interactionText.setColor(COLOR_NEUTRAL);
}

ItemDescriptionWindow::~ItemDescriptionWindow() {}

std::string ItemDescriptionWindow::getGoldText(const Item& item) const {
	std::string text;
	text.append(g_textProvider->getText("GoldValue"));
	text.append(": ");
	text.append(item.getValue() < 0 ? g_textProvider->getText("Unsalable") : to_string(item.getValue()));
	return text;
}

std::string ItemDescriptionWindow::getReputationText(const Item& item) const {
	return "";
}

std::string ItemDescriptionWindow::getInteractionText(const Item& item) const {
	std::string interactionText;

	if (item.getType() == ItemType::Document) {
		interactionText = g_textProvider->getText("RightClickRead");
	}
	else if (item.getType() == ItemType::Convertible) {
		interactionText = g_textProvider->getText("RightClickOpen");
	}
	else if (item.getType() == ItemType::Spell) {
		interactionText = g_textProvider->getText("RightClickLearn");
	}

	return interactionText;
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

	m_whiteText.setPosition(pos);
	m_coloredText.setPosition(pos);
	m_reputationText.setPosition(pos);
	m_interactionText.setPosition(pos);
}

void ItemDescriptionWindow::load(const Item& item) {
	m_titleText.setString(g_textProvider->getCroppedText(item.getID(), "item", GUIConstants::CHARACTER_SIZE_M, static_cast<int>(WIDTH - 2 * GUIConstants::TEXT_OFFSET)));
	m_descriptionText.setString(g_textProvider->getCroppedText(item.getID(), "item_desc", GUIConstants::CHARACTER_SIZE_S, static_cast<int>(WIDTH - 2 * GUIConstants::TEXT_OFFSET)));

	int lines = 0;
	std::string white = "";
	std::string colored = "";
	std::string reputation = "";
	std::string interaction = "";

	if (item.getType() == ItemType::Permanent) {
		colored.append(g_textProvider->getText("Permanent"));
		colored.append("\n\n");
		white.append("\n\n");
		lines += 2;

		m_coloredText.setColor(COLOR_GOOD);
	}
	else {
		m_coloredText.setColor(COLOR_LIGHT_PURPLE);
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

		colored.append("\n\n\n");
		lines += 3;
	}

	if (item.getFoodDuration() > sf::Time::Zero) {
		white.append(g_textProvider->getText("Duration"));
		white.append(": ");
		white.append(to_string(static_cast<int>(floor(item.getFoodDuration().asSeconds()))));
		white.append(" s\n\n");

		colored.append("\n\n");
		lines += 2;
	}

	const AttributeData& attr = item.getAttributes();

	int numberAttributes;
	AttributeData::appendItemDescriptionAttributes(colored, attr, numberAttributes);
	white.append(std::string(numberAttributes, '\n'));
	lines += numberAttributes;

	if (item.getType() == ItemType::Equipment_weapon && item.isWeapon()) {
		Weapon weapon(item.getID());
		if (weapon.getWeaponSlots().size() > 0) {
			white.append("\n");
			white.append("<<< " + g_textProvider->getText("SpellSlots") + " >>>\n");
			lines += 2;
			for (auto& it : weapon.getWeaponSlots()) {
				white.append(g_textProvider->getText(EnumNames::getSpellTypeName(it.spellSlot.spellType)));
				white.append(": ");
				size_t numberSlots = it.spellModifiers.size();
				white.append(to_string(numberSlots));
				white.append(" ");
				if (numberSlots > 1) {
					white.append(g_textProvider->getText("GemSockets"));
				}
				else {
					white.append(g_textProvider->getText("GemSocket"));
				}
				white.append("\n");
				lines++;
			}
		}
	}

	if (lines > 0) {
		white.append("\n");
		colored.append("\n");
		lines++;
	}
	
	white.append(getGoldText(item));
	lines++;

	std::string repText = getReputationText(item);
	if (!repText.empty()) {
		for (int i = 0; i < lines; ++i) {
			reputation.append("\n");
		}
		reputation.append(repText);
		m_reputationText.setColor(m_isReputationReached ? COLOR_GOOD : COLOR_NEUTRAL);
		lines++;
	}

	std::string intText = getInteractionText(item);
	if (!intText.empty()) {
		for (int i = 0; i < lines; ++i) {
			interaction.append("\n");
		}
		interaction.append(intText);
		lines++;
	}

	m_interactionText.setString(interaction);
	m_reputationText.setString(reputation);
	m_whiteText.setString(white);
	m_coloredText.setString(colored);

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
	renderTarget.draw(m_coloredText);
	renderTarget.draw(m_reputationText);
	renderTarget.draw(m_interactionText);
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


