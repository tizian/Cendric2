#include "ItemFactory.h"

void ItemFactory::loadItemBean(ItemBean& bean, ItemID id)
{
	switch (id)
	{
	case ItemID::Equipment_head_wizardhat_blue:
		bean.description = "ItemDescriptionWizardhat";
		bean.name = "ItemNameBlueWizardhat";
		bean.type = ItemType::Equipment_head;
		bean.iconTextureLocation = sf::Vector2i(100, 100);
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
		bean.iconTextureLocation = sf::Vector2i(150, 100);
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
		bean.iconTextureLocation = sf::Vector2i(200, 100);
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
		bean.iconTextureLocation = sf::Vector2i(250, 100);
		bean.levelEquipment = LevelEquipmentID::Weapon_rustysword;
		bean.value = 15;
		bean.attributes.damagePhysical = 5;
		break;
	case ItemID::Food_Cheese:
		bean.description = "ItemDescriptionCheese";
		bean.name = "ItemNameCheese";
		bean.type = ItemType::Food;
		bean.iconTextureLocation = sf::Vector2i(0, 100);
		bean.levelEquipment = LevelEquipmentID::Void;
		bean.attributes.currentHealthPoints = 40;
		bean.value = 3;
		break;
	case ItemID::Food_Bread:
		bean.description = "ItemDescriptionBread";
		bean.name = "ItemNameBread";
		bean.type = ItemType::Food;
		bean.iconTextureLocation = sf::Vector2i(50, 100);
		bean.levelEquipment = LevelEquipmentID::Void;
		bean.attributes.currentHealthPoints = 30;
		bean.value = 2;
		break;
	case ItemID::Food_Water:
		bean.description = "ItemDescriptionWater";
		bean.name = "ItemNameWater";
		bean.type = ItemType::Food;
		bean.iconTextureLocation = sf::Vector2i(100, 0);
		bean.levelEquipment = LevelEquipmentID::Void;
		bean.attributes.currentHealthPoints = 10;
		bean.attributes.currentManaPoints = 50;
		bean.value = 2;
		break;
	case ItemID::Food_CaveBerry:
		bean.description = "ItemDescriptionCaveBerry";
		bean.name = "ItemNameCaveBerry";
		bean.type = ItemType::Food;
		bean.iconTextureLocation = sf::Vector2i(0, 0);
		bean.levelEquipment = LevelEquipmentID::Void;
		bean.attributes.currentHealthPoints = 20;
		bean.value = 3;
		break;
	case ItemID::Food_GlowingShroom:
		bean.description = "ItemDescriptionGlowingShroom";
		bean.name = "ItemNameGlowingShroom";
		bean.type = ItemType::Food;
		bean.iconTextureLocation = sf::Vector2i(150, 50);
		bean.levelEquipment = LevelEquipmentID::Void;
		bean.attributes.currentHealthPoints = -20;
		bean.value = 2;
		break;
	case ItemID::Food_HealingHerb:
		bean.description = "ItemDescriptionHealingHerb";
		bean.name = "ItemNameHealingHerb";
		bean.type = ItemType::Food;
		bean.iconTextureLocation = sf::Vector2i(50, 50);
		bean.levelEquipment = LevelEquipmentID::Void;
		bean.attributes.currentHealthPoints = 50;
		bean.value = 5;
		break;
	case ItemID::Quest_Letter:
		bean.description = "ItemDescriptionLetter";
		bean.name = "ItemNameLetter";
		bean.type = ItemType::Quest;
		bean.iconTextureLocation = sf::Vector2i(200, 0);
		bean.levelEquipment = LevelEquipmentID::Void;
		bean.value = 0;
		break;
	case ItemID::Misc_GoldenGoblet:
		bean.description = "ItemDescriptionGoldenGoblet";
		bean.name = "ItemNameGoldenGoblet";
		bean.type = ItemType::Misc;
		bean.iconTextureLocation = sf::Vector2i(0, 50);
		bean.levelEquipment = LevelEquipmentID::Void;
		bean.value = 15;
		break;
	case ItemID::Equipment_ring_ringOfLesserHealth:
		bean.description = "ItemDescriptionRingOfLesserHealth";
		bean.name = "ItemNameRingOfLesserHealth";
		bean.type = ItemType::Equipment_ring_1;
		bean.iconTextureLocation = sf::Vector2i(100, 50);
		bean.levelEquipment = LevelEquipmentID::Ring_ringOfLesserHealth;
		bean.value = 20;
		break;
	default:
		bean.id = ItemID::Void;
		return;
	}

	bean.id = id;
}