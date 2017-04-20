#include <iomanip>
#include <sstream>

#include "GUI/ItemDescriptionWindow.h"
#include "CharacterCore.h"

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

void ItemDescriptionWindow::clearTexts() {
	for (auto t : m_texts) {
		delete t;
	}
	m_texts.clear();
}

ItemDescriptionWindow::ItemDescriptionWindow() : Window(
	sf::FloatRect(0.f, 0.f, WIDTH, WIDTH),
	GUIOrnamentStyle::LARGE,
	GUIConstants::MAIN_COLOR,
	GUIConstants::ORNAMENT_COLOR) {
}

ItemDescriptionWindow::~ItemDescriptionWindow() {
	clearTexts();
}

std::string ItemDescriptionWindow::getGoldText(const Item& item) const {
	std::string text;
	text.append(g_textProvider->getText("GoldValue"));
	text.append(": ");
	text.append(item.getValue() < 0 ? g_textProvider->getText("Unsalable") : std::to_string(item.getValue()));
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

void ItemDescriptionWindow::addIntComparision(int this_, int other) {
	if (this_ == other) return;
	int diff = this_ - other;
	std::string text = " (" + ((diff > 0) ? std::string("+") : std::string("")) + std::to_string(diff) + ")";
	sf::Vector2f offset = m_texts.at(m_texts.size() - 1)->getPositionOffset();
	offset.x += m_texts.at(m_texts.size() - 1)->getSize().x;
	
	auto btext = new BitmapTextHolder(text, diff > 0 ? COLOR_GOOD : COLOR_BAD, offset);
	m_texts.push_back(btext);
}

void ItemDescriptionWindow::addText(const std::string& text, const sf::Color& color, sf::Vector2f& offset, int lines) {
	if (text.empty()) return;
	auto btext = new BitmapTextHolder(text, COLOR_WHITE, offset);
	m_texts.push_back(btext);
	offset.y += btext->getSize().y;
	offset.y += lines * GUIConstants::CHARACTER_SIZE_S;
}

void ItemDescriptionWindow::addText(const std::string& text, const sf::Color& color, sf::Vector2f& offset, int lines, int characterSize) {
	if (text.empty()) return;
	auto btext = new BitmapTextHolder(text, COLOR_WHITE, offset);
	btext->setCharacterSize(characterSize);
	offset.y += btext->getSize().y;
	offset.y += lines * GUIConstants::CHARACTER_SIZE_S;
}

void ItemDescriptionWindow::loadAttributes(const Item& item, const CharacterCore* core, sf::Vector2f& offset) {
	// check item type and maybe redirect
	if (item.getType() == ItemType::Equipment_weapon && item.isWeapon()) {
		auto const weapon = core->getWeapon();
		if (weapon && weapon->getID().compare(item.getID()) != 0) {
			compareWeaponAttributes(Weapon(item.getID()), *weapon, offset);
		}
		else {
			loadWeaponAttributes(Weapon(item.getID()), offset);
		}
		return;
	}

	if (item.getType() == ItemType::Equipment_ring_1 || item.getType() == ItemType::Equipment_ring_2) {
		// TODO
		return;
	}
	else if (item.isEquipmentItem()) {
		auto const eqId = core->getEquippedItem(item.getType());
		if (!eqId.empty() && eqId.compare(item.getID()) != 0) {
			compareAttributes(item, *g_resourceManager->getItem(eqId), offset);
			return;
		}
	}

	loadDefaultAttributes(item, offset);
}

void ItemDescriptionWindow::loadWeaponAttributes(const Weapon& item, sf::Vector2f& offset) {
	addText(g_textProvider->getText("WeaponDamage") + ": " + std::to_string(item.getWeaponChopDamage()),
		COLOR_WHITE, offset);

	addText(g_textProvider->getText("Cooldown") + ": " + toStrMaxDecimals(item.getWeaponCooldown().asSeconds(), 1) + "s",
		COLOR_WHITE, offset, 1);

	loadDefaultAttributes(item, offset);

	if (item.getWeaponSlots().size() > 0) {
		offset.y += GUIConstants::CHARACTER_SIZE_S;

		addText("<<< " + g_textProvider->getText("SpellSlots") + " >>>",
			COLOR_WHITE, offset, 1);

		for (auto& it : item.getWeaponSlots()) {
			std::string str;
			str.append(g_textProvider->getText(EnumNames::getSpellTypeName(it.spellSlot.spellType)));
			str.append(": ");
			size_t numberSlots = it.spellModifiers.size();
			str.append(std::to_string(numberSlots));
			str.append(" ");
			if (numberSlots > 1) {
				str.append(g_textProvider->getText("GemSockets"));
			}
			else {
				str.append(g_textProvider->getText("GemSocket"));
			}
			
			addText(str, COLOR_WHITE, offset);
		}
	}
}

void ItemDescriptionWindow::loadDefaultAttributes(const Item& item, sf::Vector2f& offset) {
	auto const attributeColor = item.getType() == ItemType::Permanent ? COLOR_GOOD : COLOR_LIGHT_PURPLE;

	if (item.getFoodDuration() > sf::Time::Zero) {
		addText(g_textProvider->getText("Duration") + ": " + std::to_string(static_cast<int>(floor(item.getFoodDuration().asSeconds()))) + "s",
			COLOR_WHITE, offset, 1);
	}

	const AttributeData& attr = item.getAttributes();

	addText(AttributeData::getAttributeText("Health", attr.maxHealthPoints), attributeColor, offset);
	addText(AttributeData::getAttributeText("Haste", attr.haste), attributeColor, offset);
	addText(AttributeData::getAttributeText("Critical", attr.critical), attributeColor, offset);
	addText(AttributeData::getAttributeText("Heal", attr.heal), attributeColor, offset);
	addText(AttributeData::getAttributeText("PhysicalDamage", attr.damagePhysical), attributeColor, offset);
	addText(AttributeData::getAttributeText("FireDamage", attr.damageFire), attributeColor, offset);
	addText(AttributeData::getAttributeText("IceDamage", attr.damageIce), attributeColor, offset);
	addText(AttributeData::getAttributeText("LightDamage", attr.damageLight), attributeColor, offset);
	addText(AttributeData::getAttributeText("ShadowDamage", attr.damageShadow), attributeColor, offset);
	addText(AttributeData::getAttributeText("Armor", attr.resistancePhysical), attributeColor, offset);
	addText(AttributeData::getAttributeText("FireResistance", attr.resistanceFire), attributeColor, offset);
	addText(AttributeData::getAttributeText("IceResistance", attr.resistanceIce), attributeColor, offset);
	addText(AttributeData::getAttributeText("LightResistance", attr.resistanceLight), attributeColor, offset);
	addText(AttributeData::getAttributeText("ShadowResistance", attr.resistanceShadow), attributeColor, offset);

	offset.y += GUIConstants::CHARACTER_SIZE_S;
}

void ItemDescriptionWindow::compareAttributes(const Item& item, const Item& comp, sf::Vector2f& offset) {
	const AttributeData& attr = item.getAttributes();
	const AttributeData& attr2 = comp.getAttributes();

	addText(AttributeData::getAttributeText("Health", attr.maxHealthPoints, attr2.maxHealthPoints != 0), COLOR_WHITE, offset);
	addIntComparision(attr.maxHealthPoints, attr2.maxHealthPoints);

	addText(AttributeData::getAttributeText("Haste", attr.haste, attr2.haste != 0), COLOR_WHITE, offset);
	addIntComparision(attr.haste, attr2.haste);

	addText(AttributeData::getAttributeText("Critical", attr.critical, attr2.critical != 0), COLOR_WHITE, offset);
	addIntComparision(attr.critical, attr2.critical);

	addText(AttributeData::getAttributeText("Heal", attr.heal, attr2.heal != 0), COLOR_WHITE, offset);
	addIntComparision(attr.heal, attr2.heal);

	addText(AttributeData::getAttributeText("PhysicalDamage", attr.damagePhysical, attr2.damagePhysical != 0), COLOR_WHITE, offset);
	addIntComparision(attr.heal, attr2.heal);

	addText(AttributeData::getAttributeText("FireDamage", attr.damageFire, attr2.damageFire != 0), COLOR_WHITE, offset);
	addIntComparision(attr.heal, attr2.heal);

	addText(AttributeData::getAttributeText("IceDamage", attr.damageIce, attr2.damageIce != 0), COLOR_WHITE, offset);
	addIntComparision(attr.heal, attr2.heal);

	addText(AttributeData::getAttributeText("LightDamage", attr.damageLight, attr2.damageLight != 0), COLOR_WHITE, offset);
	addIntComparision(attr.heal, attr2.heal);

	addText(AttributeData::getAttributeText("ShadowDamage", attr.damageShadow, attr2.damageShadow != 0), COLOR_WHITE, offset);
	addIntComparision(attr.heal, attr2.heal);

	addText(AttributeData::getAttributeText("Armor", attr.resistancePhysical, attr2.resistancePhysical != 0), COLOR_WHITE, offset);
	addIntComparision(attr.heal, attr2.heal);

	addText(AttributeData::getAttributeText("FireResistance", attr.resistanceFire, attr2.resistanceFire != 0), COLOR_WHITE, offset);
	addIntComparision(attr.heal, attr2.heal);

	addText(AttributeData::getAttributeText("IceResistance", attr.resistanceIce, attr2.resistanceIce != 0), COLOR_WHITE, offset);
	addIntComparision(attr.heal, attr2.heal);

	addText(AttributeData::getAttributeText("LightResistance", attr.resistanceLight, attr2.resistanceLight != 0), COLOR_WHITE, offset);
	addIntComparision(attr.heal, attr2.heal);

	addText(AttributeData::getAttributeText("ShadowResistance", attr.resistanceShadow, attr2.resistanceShadow != 0), COLOR_WHITE, offset);
	addIntComparision(attr.heal, attr2.heal);

	offset.y += GUIConstants::CHARACTER_SIZE_S;
}

void ItemDescriptionWindow::compareWeaponAttributes(const Weapon& item, const Weapon& comp, sf::Vector2f& offset) {


	compareAttributes(item, comp, offset);
}

void ItemDescriptionWindow::setPosition(const sf::Vector2f& position) {
	Window::setPosition(position);

	sf::Vector2f pos(position);
	pos.x += GUIConstants::TEXT_OFFSET;
	pos.y += GUIConstants::TEXT_OFFSET;

	for (auto text : m_texts) {
		text->setPosition(pos);
	}
}

void ItemDescriptionWindow::load(const Item& item, const CharacterCore* core) {
	clearTexts();
	int maxWidth = static_cast<int>(WIDTH - 2 * GUIConstants::TEXT_OFFSET);
	sf::Vector2f currentOffset;

	// load title
	addText(g_textProvider->getCroppedText(item.getID(), "item", GUIConstants::CHARACTER_SIZE_M,
		maxWidth), COLOR_WHITE, currentOffset, 1, GUIConstants::CHARACTER_SIZE_M);

	// load description
	addText(g_textProvider->getCroppedText(item.getID(), "item_desc", GUIConstants::CHARACTER_SIZE_S,
		maxWidth), COLOR_WHITE, currentOffset, 2);

	// load attributes
	if (item.getType() == ItemType::Permanent) {
		addText(g_textProvider->getText("Permanent"), COLOR_GOOD, currentOffset, 2);
	}

	loadAttributes(item, core, currentOffset);
	currentOffset.y += GUIConstants::CHARACTER_SIZE_S;

	addText(getGoldText(item), COLOR_WHITE, currentOffset);
	addText(getReputationText(item), m_isReputationReached ? COLOR_GOOD : COLOR_NEUTRAL, currentOffset);
	addText(getInteractionText(item), COLOR_NEUTRAL, currentOffset);

	float height = 2 * GUIConstants::TEXT_OFFSET + currentOffset.y;
	setHeight(height);
}

void ItemDescriptionWindow::render(sf::RenderTarget& renderTarget) {
	if (!m_isVisible) return;
	Window::render(renderTarget);
	for (auto t : m_texts) {
		t->render(renderTarget);
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
