#pragma once

#include "global.h"

#include "DatabaseManager.h"
#include "Structs/AttributeData.h"

#include "Beans/ItemAttributeBean.h"
#include "Beans/ItemBean.h"
#include "Beans/ItemConvertibleBean.h"
#include "Beans/ItemSpellBean.h"
#include "Beans/ItemEquipmentBean.h"
#include "Beans/ItemEquipmentParticleBean.h"
#include "Beans/ItemEquipmentLightBean.h"
#include "Beans/ItemFoodBean.h"
#include "Beans/ItemWeaponBean.h"
#include "Beans/ItemWeaponSlotBean.h"
#include "Beans/ItemDocumentPageBean.h"
#include "Beans/ItemDocumentQuestBean.h"
#include "Beans/LevelitemBean.h"
#include "Beans/LevelitemFrameBean.h"
#include "Beans/LevelitemLightBean.h"

struct ItemCheck {
	bool isValid = false;
	bool isConsumable = false;
	bool isDocument = false;
	bool isWeapon = false;
	bool isLevelitem = false;
	bool isLevelitemLighted = false;
	bool isEquipment = false;
	bool isEquipmentParticle = false;
	bool isEquipmentLighted = false;
	bool isConvertible = false;
	bool isSpell = false;
};

// An item in cendrics / a npcs / a mobs inventory
class Item {
	friend class ResourceManager;
protected:
	// constructor is protected, only the resource manager can create items
	Item(const std::string& itemID);
public:
	virtual ~Item();

	const std::string& getID() const;
	ItemType getType() const;
	const sf::Vector2i& getIconTextureLocation() const;
	const AttributeData& getAttributes() const;
	int getValue() const;
	int getRarity() const;
	sf::Color getRarityColor() const;

	// returns the first bean of the given type
	// returns nullptr if there's no bean of this type.
	template<typename T> const T* getBean() const {
		for (auto bean : m_beans) {
			if (const T* tBean = dynamic_cast<const T*>(bean)) {
				return tBean;
			}
		}
		return nullptr;
	}

	// returns all beans of the given type
	template<typename T> std::vector<const T*> getBeans() const {
		std::vector<const T*> tBeans;

		for (auto bean : m_beans) {
			if (const T* tBean = dynamic_cast<const T*>(bean)) {
				tBeans.push_back(tBean);
			}
		}
		return tBeans;
	}

	const ItemCheck& getCheck() const;

	static bool isEquipmentType(ItemType type);
	static sf::Color getRarityColor(int rarity);

protected:
	std::vector<DatabaseBean*> m_beans;
	ItemBean m_itemBean;
	AttributeData m_attributeData;

	void checkItem();
	void initBeans(const std::string& itemID);
	bool addBean(DatabaseBean* bean);
	bool addBeans(std::vector<DatabaseBean*> beans);

	ItemCheck m_check;
};