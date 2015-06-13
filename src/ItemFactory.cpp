#include "ItemFactory.h"

void ItemFactory::loadItemBean(ItemBean& bean, ItemID id)
{
	switch (id)
	{
	case ItemID::Equipment_head_wizardhat_blue:
		bean.description = "ItemDescriptionWizardhat";
		bean.name = "ItemNameBlueWizardhat";
		bean.type = ItemType::Equipment_head;
		bean.icon = ResourceID::Void; // TODO
		bean.levelEquipment = LevelEquipmentID::Head_wizardhat_blue;
		bean.value = 20;
		bean.attributes.damageIce = 5;
		bean.attributes.resistanceIce = 2;
		bean.attributes.resistancePhysical = 1;
		bean.attributes.manaRegenerationPerS = 2;
		bean.attributes.maxManaPoints = 5;
		break;
	case ItemID::Equipment_head_wizardhat_grey:
		bean.description = "ItemDescriptionWizardhat";
		bean.name = "ItemNameGreyWizardhat";
		bean.type = ItemType::Equipment_head;
		bean.icon = ResourceID::Void; // TODO
		bean.levelEquipment = LevelEquipmentID::Head_wizardhat_grey;
		bean.value = 20;
		bean.attributes.manaRegenerationPerS = 4;
		bean.attributes.resistanceFire = 2;
		bean.attributes.resistancePhysical = 1;
		bean.attributes.maxManaPoints = 5;
		bean.attributes.maxHealthPoints = 5;
		break;
	case ItemID::Equipment_weapon_staff_ice:
		bean.description = "ItemDescriptionIcestaff";
		bean.name = "ItemNameIcestaff";
		bean.type = ItemType::Equipment_weapon;
		bean.icon = ResourceID::Void; // TODO
		bean.levelEquipment = LevelEquipmentID::Weapon_icestaff;
		bean.value = 120;
		bean.attributes.manaRegenerationPerS = 4;
		bean.attributes.maxManaPoints = 10;
		bean.attributes.maxHealthPoints = 10;
		bean.attributes.damageIce = 30;
		bean.attributes.damagePhysical = 15;
		break;
	case ItemID::Equipment_weapon_rustysword:
		bean.description = "ItemDescriptionRustysword";
		bean.name = "ItemNameRustysword";
		bean.type = ItemType::Equipment_weapon;
		bean.icon = ResourceID::Void; // TODO
		bean.levelEquipment = LevelEquipmentID::Weapon_rustysword;
		bean.value = 15;
		bean.attributes.damagePhysical = 5;
		break;
	case ItemID::Food_Cheese:
		bean.description = "ItemDescriptionCheese";
		bean.name = "ItemNameCheese";
		bean.type = ItemType::Food;
		bean.icon = ResourceID::Void; // TODO
		bean.levelEquipment = LevelEquipmentID::Void;
		bean.attributes.maxHealthPoints = 40;
		bean.value = 3;
		break;
	case ItemID::Food_Bread:
		bean.description = "ItemDescriptionBread";
		bean.name = "ItemNameBread";
		bean.type = ItemType::Food;
		bean.icon = ResourceID::Void; // TODO
		bean.levelEquipment = LevelEquipmentID::Void;
		bean.attributes.maxHealthPoints = 30;
		bean.value = 2;
		break;
	case ItemID::Food_Water:
		bean.description = "ItemDescriptionWater";
		bean.name = "ItemNameWater";
		bean.type = ItemType::Food;
		bean.icon = ResourceID::Void; // TODO
		bean.levelEquipment = LevelEquipmentID::Void;
		bean.attributes.maxHealthPoints = 10;
		bean.attributes.maxManaPoints = 50;
		bean.value = 2;
		break;
	default:
		bean.id = ItemID::Void;
		return;
	}

	bean.id = id;
}