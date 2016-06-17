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

	m_statsLabelsText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	m_statsLabelsText.setLineSpacing(1.75f);
	m_statsLabelsText.setColor(COLOR_WHITE);

	m_statsValuesText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	m_statsValuesText.setLineSpacing(1.75f);
	m_statsValuesText.setColor(COLOR_WHITE);
}

ItemDescriptionWindow::~ItemDescriptionWindow() {
	for (auto& it : m_sprites) {
		delete it.second;
	}
}

std::string ItemDescriptionWindow::getGoldLabelText() const {
	std::string text;
	text.append(g_textProvider->getText("GoldValue"));
	text.append(":\n");
	return text;
}

std::string ItemDescriptionWindow::getGoldValueText(const Item& item) const {
	std::string text;
	text.append(item.getValue() < 0 ? g_textProvider->getText("Unsalable") : to_string(item.getValue()));
	text.append("\n");
	return text;
}

void ItemDescriptionWindow::setPosition(const sf::Vector2f& position) {
	Window::setPosition(position);
	float y = position.y + GUIConstants::TEXT_OFFSET;
	m_titleText.setPosition(position.x + GUIConstants::TEXT_OFFSET, y);
	y += m_titleText.getLocalBounds().height + GUIConstants::CHARACTER_SIZE_S;
	m_descriptionText.setPosition(position.x + GUIConstants::TEXT_OFFSET, y);
	y += m_descriptionText.getLocalBounds().height + GUIConstants::CHARACTER_SIZE_S;

	if (m_statsLabelsText.getLocalBounds().height == GUIConstants::CHARACTER_SIZE_S) {
		y -= GUIConstants::TEXT_OFFSET - GUIConstants::CHARACTER_SIZE_S;
	}

	y += GUIConstants::CHARACTER_SIZE_S;

	sf::Vector2f statsOrigin(position.x + GUIConstants::TEXT_OFFSET, y);

	m_statsLabelsText.setPosition(statsOrigin);
	m_statsValuesText.setPosition(statsOrigin.x + ICON_OFFSET + 28.f, y);

	for (auto& it : m_sprites) {
		sf::Vector2f pos = statsOrigin + it.first;
		pos.y -= 6.f;
		it.second->setPosition(pos);
	}
}

sf::RectangleShape* ItemDescriptionWindow::getStatSprite(const sf::IntRect& rect) const {
	sf::RectangleShape *shape = new sf::RectangleShape();
	shape->setSize(sf::Vector2f(20.f, 20.f));
	shape->setTexture(g_resourceManager->getTexture(GlobalResource::TEX_GUI_STATS_ICONS));
	shape->setTextureRect(rect);
	return shape;
}

void ItemDescriptionWindow::load(const Item& item) {
	for (auto& it : m_sprites) {
		delete it.second;
	}
	m_sprites.clear();

	m_titleText.setString(g_textProvider->getText(item.getID(), "item"));
	m_descriptionText.setString(g_textProvider->getCroppedText(item.getID(), "item_desc", GUIConstants::CHARACTER_SIZE_S, static_cast<int>(WIDTH - 2 * GUIConstants::TEXT_OFFSET)));

	string statsLabels = "";
	string statsValues = "";
	int position = 0;

	if (item.getType() == ItemType::Permanent) {
		m_statsLabelsText.setColor(COLOR_GOOD);
		m_statsValuesText.setColor(COLOR_GOOD);
		statsLabels.append(g_textProvider->getText("Permanent"));
		statsLabels.append("\n\n");
		position += 2;
	}
	else {
		m_statsLabelsText.setColor(COLOR_WHITE);
		m_statsValuesText.setColor(COLOR_WHITE);
	}

	const AttributeData& attr = item.getAttributes();
	AttributeData::appendAttributeLabels(statsLabels, attr);
	AttributeData::appendAttributeValues(statsValues, attr);

	std::vector<sf::IntRect> rects;
	AttributeData::getTextureRectangles(rects, attr);
	int nAttributes = static_cast<int>(rects.size());
	
	for (int i = 0; i < nAttributes; ++i) {
		m_sprites.push_back(std::pair<sf::Vector2f, sf::RectangleShape*>(sf::Vector2f(ICON_OFFSET, 22.f * position), getStatSprite(rects[i])));
		position++;
	}

	statsLabels.append("\n");
	statsValues.append("\n");
	position++;

	if (item.getFoodDuration() > sf::Time::Zero) {
		statsLabels.append(g_textProvider->getText("Duration"));
		statsLabels.append(": ");
		statsLabels.append("\n");

		statsValues.append(to_string(static_cast<int>(floor(item.getFoodDuration().asSeconds()))));
		statsValues.append(" s\n");

		position++;
	}

	statsLabels.append(getGoldLabelText());
	statsValues.append(getGoldValueText(item));
	m_sprites.push_back(std::pair<sf::Vector2f, sf::RectangleShape*>(sf::Vector2f(ICON_OFFSET, 22.f * position), getStatSprite(sf::IntRect(80, 0, 20, 20))));
	position++;

	statsLabels.append("\n");
	statsValues.append("\n");
	position++;

	if (item.getType() == ItemType::Equipment_weapon && item.isWeapon()) {
		Weapon weapon(item.getID());

		statsLabels.append(g_textProvider->getText("WeaponDamage"));
		statsLabels.append(": ");
		statsLabels.append("\n");

		m_sprites.push_back(std::pair<sf::Vector2f, sf::RectangleShape*>(sf::Vector2f(ICON_OFFSET, 22.f * position), getStatSprite(sf::IntRect(0, 20, 20, 20))));
		position++;
		
		statsValues.append(to_string(weapon.getWeaponChopDamage()));
		statsValues.append("\n");

		statsLabels.append(g_textProvider->getText("Cooldown"));
		statsLabels.append(": ");
		statsLabels.append("\n");

		statsValues.append(toStrMaxDecimals(weapon.getWeaponCooldown().asSeconds(), 1));
		statsValues.append("s\n");

		m_sprites.push_back(std::pair<sf::Vector2f, sf::RectangleShape*>(sf::Vector2f(ICON_OFFSET, 22.f * position), getStatSprite(sf::IntRect(60, 0, 20, 20))));
		position++;

		if (weapon.getWeaponSlots().size() > 0) {
			statsLabels.append("\n");
			statsLabels.append("<<< " + g_textProvider->getText("SpellSlots") + " >>>\n");
			for (auto& it : weapon.getWeaponSlots()) {
				statsLabels.append(g_textProvider->getText(EnumNames::getSpellTypeName(it.spellSlot.spellType)));
				statsLabels.append(" - " + g_textProvider->getText("GemSockets") + ": ");
				statsLabels.append(to_string(it.spellModifiers.size()) + "\n");
			}
		}
	}

	m_statsLabelsText.setString(statsLabels);
	m_statsValuesText.setString(statsValues);

	float height = 2 * GUIConstants::TEXT_OFFSET;
	height += m_titleText.getLocalBounds().height + GUIConstants::CHARACTER_SIZE_S * 2;
	height += m_descriptionText.getLocalBounds().height + GUIConstants::CHARACTER_SIZE_S;
	height += m_statsLabelsText.getLocalBounds().height;
	setHeight(height);
}

void ItemDescriptionWindow::render(sf::RenderTarget& renderTarget) {
	if (!m_isVisible) return;
	Window::render(renderTarget);
	renderTarget.draw(m_titleText);
	renderTarget.draw(m_descriptionText);
	renderTarget.draw(m_statsLabelsText);
	renderTarget.draw(m_statsValuesText);
	for (auto& it : m_sprites) {
		renderTarget.draw(*it.second);
	}
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


