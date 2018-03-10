#include "Export/ItemExport.h"
#include "ResourceManager.h"
#include "GlobalResource.h"
#include "TextProvider.h"
#include "World/Item.h"
#include "World/Weapon.h"
#include "Enums/EnumNames.h"

#include <iomanip>
#include <sstream>

const std::string ItemExport::EXPORT_PATH = "export/item/";

inline static std::string toStrMaxDecimals(float value, int decimals) {
	std::ostringstream ss;
	ss << std::fixed << std::setprecision(decimals) << value;
	std::string s = ss.str();
	if (decimals > 0 && s[s.find_last_not_of('0')] == '.') {
		s.erase(s.size() - decimals + 1);
	}
	return std::string(s.begin(), s.end());
}

static inline std::string getTypeName(ItemType type) {
	switch (type)
	{
	case ItemType::Equipment_head:
		return "Equipment (Head)";
	case ItemType::Equipment_ring_1:
	case ItemType::Equipment_ring_2:
		return "Equipment (Ring)";
	case ItemType::Equipment_weapon:
		return "Equipment (Weapon)";
	case ItemType::Equipment_body:
		return "Equipment (Body)";
	case ItemType::Equipment_neck:
		return "Equipment (Neck)";
	case ItemType::Equipment_back:
		return "Equipment (Back)";
	case ItemType::Consumable:
		return "Consumable";
	case ItemType::Quest:
		return "Quest";
	case ItemType::Document:
		return "Document";
	case ItemType::Misc:
		return "Miscellaneous";
	case ItemType::Gold:
		return "Gold";
	case ItemType::Permanent:
		return "Permanent";
	case ItemType::Convertible:
		return "Convertible";
	case ItemType::Spell:
		return "Spell Scroll";
	case ItemType::Key:
		return "Key";
	default:
	case ItemType::VOID:
		return "";
	}
}

static inline std::string getRarity(int rarity) {
	switch (rarity)
	{
	case 0:
	default:
		return "Common (0)";
	case 1:
		return "Uncommon (1)";
	case 2:
		return "Rare (2)";
	case 3:
		return "Epic (3)";
	case 4:
		return "Legendary (4)";
	}
}

void ItemExport::exportItems() {
	auto allIds = g_resourceManager->getAllItemIds();

	for (auto id : allIds) {
		Item* item = g_resourceManager->getItem(id);

		if (item == nullptr) {
			continue;
		}

		exportItem(item);
	}
}

void ItemExport::exportItem(Item* item) {
	savePicture(item);

	auto name = g_textProvider->getText(item->getID(), "item");

	std::ofstream outfile(name + ".txt");
	writeInfobox(item, outfile);
	writeDescription(item, outfile);
	writeAttributes(item, outfile);
	writeWeapon(item, outfile);
	writeConvertible(item, outfile);
	writeDocument(item, outfile);
	writeCategories(item, outfile);

	outfile.close();
}

void ItemExport::writeDescription(Item* item, std::ofstream& out) {
	if (item->getType() == ItemType::Gold) {
		// no real description
		out << "== Description ==\n\n";
		out << "A gold item, which results in " + std::to_string(item->getValue()) + " gold, when picked up.\n\n";
		return;
	}

	auto desc = g_textProvider->getText(item->getID(), "item_desc");

	out << "== Description ==\n\n";
	out << "''" + desc + "''\n\n";
}

void ItemExport::writeAttributes(Item* item, std::ofstream& out) {
	const ItemAttributeBean* a = item->getBean<ItemAttributeBean>();
	if (!a) return;

	const ItemFoodBean* food = item->getBean<ItemFoodBean>();
	auto& attr = item->getAttributes();

	out << "== Attributes ==\n\n";

	out << "{| class = \"wikitable\"\n";
	out << "| -\n";
	out << "!Attribute\n";
	out << "!Value\n";
	out << "| -\n";

	if (item->getType() == ItemType::Equipment_weapon && item->getCheck().isWeapon) {
		auto weaponBean = item->getBean<ItemWeaponBean>();
		out << "| [[Weapon Damage]]\n";
		out << "| " + std::to_string(weaponBean->chop_damage) + "\n";
		out << "| -\n";

		out << "| [[Cooldown]]\n";
		out << "| " + toStrMaxDecimals(weaponBean->chop_cooldown.asSeconds(), 1) + "s\n";
		out << "| -\n";
	}

	if (food && food->food_duration > sf::Time::Zero) {
		out << "| [[Duration]]\n";
		out << "| " + std::to_string(static_cast<int>(floor(food->food_duration.asSeconds()))) + "s\n";
	}

	for (auto& it : AttributeData::ATTR_MAP) {
		out << "| -\n";
		out << AttributeData::getAttributeTextWiki(it.first, it.second(attr));
	}

	out << "|}\n\n";
}

void ItemExport::writeConvertible(Item* item, std::ofstream& out) {
	const ItemConvertibleBean* c = item->getBean<ItemConvertibleBean>();
	if (!c) return;

	auto otherName = g_textProvider->getText(item->getID(), "item");

	out << "== Convertible ==\n\n";

	out << "This item can be converted to " + std::to_string(c->convertible_amount) + "x [[" + otherName + "]] with a probability of " + std::to_string((int)c->probability) + "%.\n\n";
}

void ItemExport::writeWeapon(Item* item, std::ofstream& out) {
	Weapon* wep = dynamic_cast<Weapon*>(item);
	if (!wep || wep->getWeaponSlots().empty()) {
		return;
	}

	out << "== Spell Slots ==\n\n";
    
    out << "{| class = \"wikitable\"\n";
    out << "| -\n";
    out << "!Magic Type\n";
    out << "!Gem Slots\n";  

	for (auto& it : wep->getWeaponSlots()) {
        out << "| -\n";
		out << "|[[" + g_textProvider->getText(EnumNames::getSpellTypeName(it.spellSlot.spellType)) + "]]\n";
		out << "|\n" std::to_string(it.spellModifiers.size()) + "\n";
	}

    out << "|}\n\n";
}

void ItemExport::writeDocument(Item* item, std::ofstream& out) {

}

void ItemExport::writeInfobox(Item* item, std::ofstream& out) {
	auto name = g_textProvider->getText(item->getID(), "item");
	auto typeName = getTypeName(item->getType());
	auto rarity = getRarity(item->getRarity());
	auto goldValue = item->getValue() < 0 ? "Unsalable" : std::to_string(item->getValue());

	out << std::string("{{ItemInfobox\n") +
		"| name = " + name + "\n" +
		"| image = " + item->getID() + ".png\n" +
		"| type = " + typeName + "\n" +
		"| rarity = " + rarity + "\n" +
		"| value = " + goldValue + "\n" +
		"| ID = " + item->getID() + "\n" +
		"}}\n\n";
}

void ItemExport::writeCategories(Item* item, std::ofstream& out) {
	out << "[[Category:Item]] ";

	out << "[[Category:" + getTypeName(item->getType) + " Item]]";

	if (item->isEquipmentType(item->getType)) {
		out << "[[Category:Equipment]]";
	}

	out << "\n\n";
}

void ItemExport::savePicture(Item* item) {
	sf::Image fullImg;
	fullImg.loadFromFile(getResourcePath(GlobalResource::TEX_ITEMS));

	sf::Image texImg;
	texImg.create(50, 50, COLOR_TRANSPARENT);
	texImg.copy(fullImg, 0, 0, sf::IntRect(item->getIconTextureLocation().x, item->getIconTextureLocation().y, 50, 50));

	auto name = g_textProvider->getText(item->getID(), "item");
	texImg.saveToFile(EXPORT_PATH + name + ".png");
}