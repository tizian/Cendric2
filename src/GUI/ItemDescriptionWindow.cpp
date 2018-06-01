#include <iomanip>
#include <sstream>

#include "GUI/ItemDescriptionWindow.h"
#include "GUI/GUIConstants.h"
#include "CharacterCore.h"
#include "World/Item.h"
#include "Enums/EnumNames.h"

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

std::string ItemDescriptionWindow::getGoldText(const Item& item, float goldMultiplier) const {
	std::string text;
	text.append(g_textProvider->getText("GoldValue"));
	text.append(": ");
	text.append(item.getValue() < 0 ? g_textProvider->getText("Unsalable") : std::to_string(static_cast<int>(std::ceil(item.getValue() * goldMultiplier))));
	return text;
}

std::string ItemDescriptionWindow::getReputationText(const Item& item) const {
	return "";
}

std::string ItemDescriptionWindow::getInteractionTextGamepad(const Item& item, const ItemDescriptionInfo& info) const {
	std::string interactionText;
	const auto type = item.getType();

	if (info.isSelling) {
		if (item.getValue() > 0) {
			interactionText = getGamepadText("ToSell", Key::Interact);
		}
	}
	else if (info.isEquipmentOrigin && !info.isInBossLevel) {
		interactionText = getGamepadText("ToUnequip", Key::Interact);
	}
	else if (!info.isInBossLevel && Item::isRingType(type)) {
		interactionText = 
			getGamepadText("ToEquip", Key::Interact) + "\n" +
			getGamepadText("ToEquipSlot1", Key::Attack) + "\n" +
			getGamepadText("ToEquipSlot2", Key::Jump);
	}
	else if (!info.isInBossLevel && Item::isEquipmentType(type)) {
		interactionText = getGamepadText("ToEquip", Key::Interact);
	}
	else if (type == ItemType::Consumable) {
		interactionText =
			(info.isInLevel ?
				"\n" + getGamepadText("ToConsume", Key::Interact) : "") +
			getGamepadText("ToEquipSlot1", Key::Attack) + "\n" +
			getGamepadText("ToEquipSlot2", Key::Jump);
	}
	else if (type == ItemType::Permanent) {
		interactionText = getGamepadText("ToConsume", Key::Interact);
	}
	else if (type == ItemType::Document) {
		interactionText = getGamepadText("ToRead", Key::Interact);
	}
	else if (type == ItemType::Convertible) {
		interactionText = getGamepadText("ToOpen", Key::Interact);
	}
	else if (type == ItemType::Spell) {
		interactionText = getGamepadText("ToLearn", Key::Interact);
	}

	return interactionText;
}

std::string ItemDescriptionWindow::getInteractionTextKeyboard(const Item& item, const ItemDescriptionInfo& info) const {
	std::string interactionText;
	const auto type = item.getType();

	if (info.isSelling) {
		if (item.getValue() > 0) {
			interactionText = getKeyboardText("ToSell");
		}
	}
	else if (info.isEquipmentOrigin && !info.isInBossLevel) {
		interactionText = getKeyboardText("ToUnequip");
	}
	else if (!info.isInBossLevel && Item::isEquipmentType(type)) {
		interactionText = getKeyboardText("ToEquip");
	}
	else if (type == ItemType::Consumable && info.isInLevel) {
		interactionText = getKeyboardText("ToConsume");
	}
	else if (type == ItemType::Permanent) {
		interactionText = getKeyboardText("ToConsume");
	}
	else if (type == ItemType::Document) {
		interactionText = getKeyboardText("ToRead");
	}
	else if (type == ItemType::Convertible) {
		interactionText = getKeyboardText("ToOpen");
	}
	else if (type == ItemType::Spell) {
		interactionText = getKeyboardText("ToLearn");
	}

	return interactionText;
}

std::string ItemDescriptionWindow::getInteractionText(const Item& item, const ItemDescriptionInfo& info) const {
	if (g_inputController->isGamepadConnected()) {
		return getInteractionTextGamepad(item, info);
	}

	return getInteractionTextKeyboard(item, info);
}

std::string ItemDescriptionWindow::getKeyboardText(const std::string& textKey) {
	return g_textProvider->getText("InteractRightClick") + " " + g_textProvider->getText(textKey);
}

std::string ItemDescriptionWindow::getGamepadText(const std::string& textKey, Key key) {
	auto const resolvedKey = EnumNames::getGamepadAxisName(
		g_resourceManager->getConfiguration().gamepadKeyMap.at(key));
	return resolvedKey + " " + g_textProvider->getText(textKey);
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

void ItemDescriptionWindow::addCooldownComparision(float this_, float other) {
	if (this_ == other) return;
	float diff = this_ - other;
	std::string text = " (" + ((diff > 0) ? std::string("+") : std::string("")) + toStrMaxDecimals(diff, 1) + ")";
	sf::Vector2f offset = m_texts.at(m_texts.size() - 1)->getPositionOffset();
	offset.x += m_texts.at(m_texts.size() - 1)->getSize().x;

	auto btext = new BitmapTextHolder(text, diff < 0 ? COLOR_GOOD : COLOR_BAD, offset);
	m_texts.push_back(btext);
}

void ItemDescriptionWindow::addText(const std::string& text, const sf::Color& color, sf::Vector2f& offset, int lines) {
	if (text.empty()) return;
	auto btext = new BitmapTextHolder(text, color, offset);
	m_texts.push_back(btext);
	offset.y += btext->getSize().y;
	offset.y += lines * GUIConstants::CHARACTER_SIZE_S;
}

void ItemDescriptionWindow::addText(const std::string& text, const sf::Color& color, sf::Vector2f& offset, int lines, int characterSize) {
	if (text.empty()) return;
	auto btext = new BitmapTextHolder(text, color, offset);
	btext->setCharacterSize(characterSize);
	m_texts.push_back(btext);
	offset.y += btext->getSize().y;
	offset.y += lines * GUIConstants::CHARACTER_SIZE_S;
}

void ItemDescriptionWindow::loadAttributes(const Item& item, const CharacterCore* core, sf::Vector2f& offset) {
	// check item type and maybe redirect
	if (item.getType() == ItemType::Equipment_weapon && item.getCheck().isWeapon) {
		auto const weapon = core->getWeapon();
		if (weapon && weapon->getID() != item.getID()) {
			compareWeaponAttributes(Weapon(item.getID()), *weapon, offset);
		}
		else {
			loadWeaponAttributes(Weapon(item.getID()), offset);
		}
		return;
	}

	if (Item::isRingType(item.getType())) {
		auto& ring1 = core->getEquippedItem(ItemType::Equipment_ring_1);
		auto& ring2 = core->getEquippedItem(ItemType::Equipment_ring_2);
		if (!ring1.empty() && !ring2.empty()) {
			compareDoubleAttributes(item, *g_resourceManager->getItem(ring1), *g_resourceManager->getItem(ring2), offset);
			return;
		}
		if (!ring1.empty()) {
			compareAttributes(item, *g_resourceManager->getItem(ring1), offset);
			return;
		}
		if (!ring2.empty()) {
			compareAttributes(item, *g_resourceManager->getItem(ring2), offset);
			return;
		}
	}
	else if (Item::isEquipmentType(item.getType())) {
		auto& eqId = core->getEquippedItem(item.getType());
		if (!eqId.empty() && eqId != item.getID()) {
			compareAttributes(item, *g_resourceManager->getItem(eqId), offset);
			return;
		}
	}

	loadDefaultAttributes(item, offset);
}

void ItemDescriptionWindow::loadWeaponAttributes(const Weapon& item, sf::Vector2f& offset) {
	auto const weaponBean = item.getBean<ItemWeaponBean>();
	addText(g_textProvider->getText("WeaponDamage") + ": " + std::to_string(weaponBean->chop_damage),
		COLOR_WHITE, offset);

	addText(g_textProvider->getText("Cooldown") + ": " + toStrMaxDecimals(weaponBean->chop_cooldown.asSeconds(), 1) + "s",
		COLOR_WHITE, offset, 1);

	loadDefaultAttributes(item, offset);

	if (!item.getWeaponSlots().empty()) {
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

			addText(str, COLOR_LIGHT_PURPLE, offset);
		}

		offset.y += GUIConstants::CHARACTER_SIZE_S;
	}
}

void ItemDescriptionWindow::loadDefaultAttributes(const Item& item, sf::Vector2f& offset) {
	const ItemFoodBean* food = item.getBean<ItemFoodBean>();
	if (food && food->food_duration > sf::Time::Zero) {
		addText(g_textProvider->getText("Duration") + ": " + std::to_string(static_cast<int>(floor(food->food_duration.asSeconds()))) + "s",
			COLOR_WHITE, offset, 1);
	}

	const AttributeData& attr = item.getAttributes();

	for (auto& it : AttributeData::ATTR_MAP) {
		sf::Color col = item.getType() == ItemType::Permanent ? COLOR_GOOD : it.second(attr) > 0 ? COLOR_LIGHT_GOOD : COLOR_LIGHT_BAD;
		addText(AttributeData::getAttributeText(it.first, it.second(attr)), col, offset);
	}

	offset.y += GUIConstants::CHARACTER_SIZE_S;
}

void ItemDescriptionWindow::compareAttributes(const Item& item, const Item& comp, sf::Vector2f& offset) {
	const AttributeData& attr = item.getAttributes();
	const AttributeData& attr2 = comp.getAttributes();

	for (auto& it : AttributeData::ATTR_MAP) {
		sf::Color col = it.second(attr) == 0 ? COLOR_MEDIUM_GREY : it.second(attr) > 0 ? COLOR_LIGHT_GOOD : COLOR_LIGHT_BAD;
		addText(AttributeData::getAttributeText(it.first, it.second(attr), it.second(attr2) != 0), col, offset);
		addIntComparision(it.second(attr), it.second(attr2));
	}

	offset.y += GUIConstants::CHARACTER_SIZE_S;
}

void ItemDescriptionWindow::compareDoubleAttributes(const Item& item, const Item& comp1, const Item& comp2, sf::Vector2f& offset) {
	const AttributeData& attr = item.getAttributes();
	const AttributeData& attr1 = comp1.getAttributes();
	const AttributeData& attr2 = comp2.getAttributes();

	for (auto& it : AttributeData::ATTR_MAP) {
		sf::Color col = it.second(attr) == 0 ? COLOR_LIGHT_GREY : it.second(attr) > 0 ? COLOR_LIGHT_GOOD : COLOR_LIGHT_BAD;
		addText(AttributeData::getAttributeText(it.first, it.second(attr), (it.second(attr1) != 0 || it.second(attr2) != 0)), col, offset);
		addIntComparision(it.second(attr), it.second(attr1));
		addIntComparision(it.second(attr), it.second(attr2));
	}

	offset.y += GUIConstants::CHARACTER_SIZE_S;
}

void ItemDescriptionWindow::compareWeaponAttributes(const Weapon& item, const Weapon& comp, sf::Vector2f& offset) {
	auto const weaponBean = item.getBean<ItemWeaponBean>();
	addText(g_textProvider->getText("WeaponDamage") + ": " + std::to_string(weaponBean->chop_damage),
		COLOR_WHITE, offset);
	addIntComparision(weaponBean->chop_damage, weaponBean->chop_damage);

	addText(g_textProvider->getText("Cooldown") + ": " + toStrMaxDecimals(weaponBean->chop_cooldown.asSeconds(), 1) + "s",
		COLOR_WHITE, offset, 1);
	addCooldownComparision(weaponBean->chop_cooldown.asSeconds(), weaponBean->chop_cooldown.asSeconds());

	compareAttributes(item, comp, offset);

	if (!item.getWeaponSlots().empty()) {
		offset.y += GUIConstants::CHARACTER_SIZE_S;

		addText("<<< " + g_textProvider->getText("SpellSlots") + " >>>",
			COLOR_WHITE, offset, 1);

		for (auto& it : item.getWeaponSlots()) {
			std::string str;
			str.append(g_textProvider->getText(EnumNames::getSpellTypeName(it.spellSlot.spellType)));
			str.append(": ");
			auto const numberSlots = it.spellModifiers.size();
			str.append(std::to_string(numberSlots));
			str.append(" ");
			if (numberSlots > 1) {
				str.append(g_textProvider->getText("GemSockets"));
			}
			else {
				str.append(g_textProvider->getText("GemSocket"));
			}

			addText(str, COLOR_LIGHT_PURPLE, offset);
		}

		offset.y += GUIConstants::CHARACTER_SIZE_S;
	}
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

void ItemDescriptionWindow::load(const Item& item, const CharacterCore* core, float goldMultiplier, const ItemDescriptionInfo& info) {
	clearTexts();
	int maxWidth = static_cast<int>(WIDTH - 2 * GUIConstants::TEXT_OFFSET);
	sf::Vector2f currentOffset;

	// load title
	addText(g_textProvider->getCroppedText(item.getID(), "item", GUIConstants::CHARACTER_SIZE_M,
		maxWidth), item.getRarityColor(), currentOffset, 1, GUIConstants::CHARACTER_SIZE_M);

	// load description
	addText(g_textProvider->getCroppedText(item.getID(), "item_desc", GUIConstants::CHARACTER_SIZE_S,
		maxWidth), COLOR_LIGHT_GREY, currentOffset, 2);

	// load attributes
	if (item.getType() == ItemType::Permanent) {
		addText(g_textProvider->getText("Permanent"), COLOR_GOOD, currentOffset, 2);
	}

	loadAttributes(item, core, currentOffset);

	addText(getGoldText(item, goldMultiplier), COLOR_WHITE, currentOffset);
	addText(getReputationText(item), m_isReputationReached ? COLOR_GOOD : COLOR_BAD, currentOffset);
	addText(getInteractionText(item, info), COLOR_NEUTRAL, currentOffset);

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
