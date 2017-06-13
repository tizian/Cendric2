#include "World/Item.h"
#include "World/Weapon.h"

Item::Item(const std::string& itemID) {
	if (!itemID.empty())
		initBeans(itemID);
}

void Item::initBeans(const std::string& itemID) {
	if (ItemBean* bean = g_databaseManager->getItemBean(itemID)) {
		m_itemBean = *bean;
		m_check.isValid = true;
		delete bean;
	}
	else {
		return;
	}

	if (ItemAttributeBean* data = g_databaseManager->getItemAttributeBean(itemID)) {
		m_attributeData.create(data);
		delete data;
	}

	m_check.isConvertible = addBeans(g_databaseManager->getItemConvertibleBeans(itemID));
	m_check.isSpell = addBean(g_databaseManager->getItemSpellBean(itemID));
	m_check.isEquipment = addBean(g_databaseManager->getItemEquipmentBean(itemID));
	m_check.isEquipmentLighted = addBean(g_databaseManager->getItemEquipmentLightBean(itemID));
	m_check.isEquipmentParticle = addBean(g_databaseManager->getItemEquipmentParticleBean(itemID));
	m_check.isConsumable = addBean(g_databaseManager->getItemFoodBean(itemID));
	m_check.isLevelitem = addBean(g_databaseManager->getLevelitemBean(itemID));
	addBeans(g_databaseManager->getLevelitemFrameBeans(itemID));
	m_check.isLevelitemLighted = addBean(g_databaseManager->getLevelitemLightBean(itemID));
	m_check.isWeapon = addBean(g_databaseManager->getItemWeaponBean(itemID));
	addBeans(g_databaseManager->getItemWeaponSlotBeans(itemID));
	m_check.isDocument = addBeans(g_databaseManager->getItemDocumentPageBeans(itemID));
	addBean(g_databaseManager->getItemDocumentQuestBean(itemID));

	checkItem();
}

Item::~Item() {
	CLEAR_VECTOR(m_beans);
}

bool Item::addBean(DatabaseBean* bean) {
	if (!bean) return false;

	m_beans.push_back(bean);
	return true;
}

bool Item::addBeans(std::vector<DatabaseBean*> beans) {
	if (beans.empty()) return false;

	for (auto bean : beans) {
		m_beans.push_back(bean);
	}
	return true;
}

const std::string& Item::getID() const {
	return m_itemBean.item_id;
}

ItemType Item::getType() const {
	return m_itemBean.item_type;
}

const sf::Vector2i& Item::getIconTextureLocation() const {
	return m_itemBean.icon_location;
}

int Item::getValue() const {
	return m_itemBean.gold_value;
}

const AttributeData& Item::getAttributes() const {
	return m_attributeData;
}

void Item::checkItem() {
	m_check.isConsumable = m_check.isConsumable && m_itemBean.item_type == ItemType::Consumable;
	m_check.isLevelitem = m_check.isLevelitem && !getBeans<LevelitemFrameBean>().empty();
	m_check.isEquipment = m_check.isEquipment && isEquipmentType(m_itemBean.item_type);
	m_check.isEquipmentLighted = m_check.isEquipment && m_check.isEquipmentLighted;
	m_check.isEquipmentParticle = m_check.isEquipment && m_check.isEquipmentParticle;
	m_check.isLevelitemLighted = m_check.isLevelitem && m_check.isLevelitemLighted;
}

const ItemCheck& Item::getCheck() const {
	return m_check;
}

bool Item::isEquipmentType(ItemType type) {
	return type == ItemType::Equipment_back ||
		type == ItemType::Equipment_body ||
		type == ItemType::Equipment_head ||
		type == ItemType::Equipment_neck ||
		type == ItemType::Equipment_ring_1 ||
		type == ItemType::Equipment_ring_2 ||
		type == ItemType::Equipment_weapon;
}