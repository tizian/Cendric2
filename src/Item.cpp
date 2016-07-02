#include "Item.h"
#include "Weapon.h"

Item::Item(const std::string& itemID) {
	if (!itemID.empty())
		initBeans(itemID);
	checkItem();
}

void Item::initBeans(const std::string& itemID) {
	m_itemBean = g_databaseManager->getItemBean(itemID);
	m_itemConvertibleBean = g_databaseManager->getItemConvertibleBean(itemID);
	m_itemSpellBean = g_databaseManager->getItemSpellBean(itemID);
	m_itemEquipmentBean = g_databaseManager->getItemEquipmentBean(itemID);
	m_itemEquipmentLightBean = g_databaseManager->getItemEquipmentLightBean(itemID);
	m_itemFoodBean = g_databaseManager->getItemFoodBean(itemID);
	m_levelItemBean = g_databaseManager->getLevelitemBean(itemID);
	m_levelItemFrameBeans = g_databaseManager->getLevelitemFrameBeans(itemID);
	m_levelitemLightBean = g_databaseManager->getLevelitemLightBean(itemID);
	m_attributeData = ZERO_ATTRIBUTES;
	m_attributeData.create(g_databaseManager->getItemAttributeBean(itemID));
	m_itemWeaponBean = g_databaseManager->getItemWeaponBean(itemID);
	m_itemWeaponSlotBeans = g_databaseManager->getItemWeaponSlotBeans(itemID);
}

Item::~Item() {
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

const sf::Time& Item::getFoodDuration() const {
	return m_itemFoodBean.food_duration;
}

const ItemConvertibleBean& Item::getConvertibleBean() const {
	return m_itemConvertibleBean;
}

const ItemSpellBean& Item::getSpellBean() const {
	return m_itemSpellBean;
}

const ItemEquipmentBean& Item::getEquipmentBean() const {
	return m_itemEquipmentBean;
}

const ItemEquipmentLightBean& Item::getEquipmentLightBean() const {
	return m_itemEquipmentLightBean;
}

const LevelitemBean& Item::getLevelitemBean() const {
	return m_levelItemBean;
}

const std::vector<LevelitemFrameBean>& Item::getFrames() const {
	return m_levelItemFrameBeans;
}

const LevelitemLightBean& Item::getLevelitemLightBean() const {
	return m_levelitemLightBean;
}

void Item::checkItem() {
	if (m_itemBean.status == BeanStatus::Filled) {
		m_isValid = true;
	}
	if (m_itemBean.item_type == ItemType::Consumable) {
		m_isConsumable = m_itemFoodBean.status == BeanStatus::Filled;
	}
	if (m_levelItemBean.status == BeanStatus::Filled && m_levelItemFrameBeans.size() > 0) {
		m_isLevelitem = true;
	}
	if (m_itemEquipmentBean.status == BeanStatus::Filled && isEquipmentType(m_itemBean.item_type)) {
		m_isEquipment = true;
	}
	if (m_isEquipment && m_itemEquipmentLightBean.status == BeanStatus::Filled) {
		m_isEquipmentLighted = true;
	}
	if (m_isLevelitem && m_levelitemLightBean.status == BeanStatus::Filled) {
		m_isLevelitemLighted = true;
	}
	if (m_itemWeaponBean.status == BeanStatus::Filled) {
		m_isWeapon = true;
	}
	if (m_itemConvertibleBean.status == BeanStatus::Filled) {
		m_isConvertible = true;
	}
	if (m_itemSpellBean.status == BeanStatus::Filled) {
		m_isSpell = true;
	}
}

bool Item::isValid() const {
	return m_isValid;
}

bool Item::isConsumable() const {
	return m_isConsumable;
}

bool Item::isWeapon() const {
	return m_isWeapon;
}

bool Item::isLevelitem() const {
	return m_isLevelitem;
}

bool Item::isConvertible() const {
	return m_isConvertible;
}

bool Item::isSpell() const {
	return m_isSpell;
}

bool Item::isEquipmentItem() const {
	return m_isEquipment;
}

bool Item::isLevelitemLightedItem() const {
	return m_isLevelitemLighted;
}

bool Item::isEquipmentLightedItem() const {
	return m_isEquipmentLighted;
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