#include "ItemFactory.h"

void ItemFactory::loadItemBean(ItemBean& bean, ItemID id)
{
	switch (id)
	{
	case ItemID::Equipment_head_wizardhat_blue:
		bean.description = "Itd_Wizardhat";
		bean.name = "It_BlueWizardhat";
		bean.type = ItemType::Equipment_head;
		bean.iconTextureLocation = sf::Vector2i(100, 100); // TODO
		bean.levelEquipment = LevelEquipmentID::Head_wizardhat_blue;
		bean.value = 20;
		bean.attributes.damageIce = 5;
		bean.attributes.resistanceIce = 2;
		bean.attributes.resistancePhysical = 1;
		bean.attributes.manaRegenerationPerS = 2;
		bean.attributes.maxManaPoints = 5;
		break;
	case ItemID::Equipment_head_wizardhat_grey:
		bean.description = "Itd_Wizardhat";
		bean.name = "It_GreyWizardhat";
		bean.type = ItemType::Equipment_head;
		bean.iconTextureLocation = sf::Vector2i(150, 100); // TODO
		bean.levelEquipment = LevelEquipmentID::Head_wizardhat_grey;
		bean.value = 20;
		bean.attributes.manaRegenerationPerS = 4;
		bean.attributes.resistanceFire = 2;
		bean.attributes.resistancePhysical = 1;
		bean.attributes.maxManaPoints = 5;
		bean.attributes.maxHealthPoints = 5;
		break;
	case ItemID::Equipment_weapon_staff_ice:
		bean.description = "Itd_Icestaff";
		bean.name = "It_Icestaff";
		bean.type = ItemType::Equipment_weapon;
		bean.iconTextureLocation = sf::Vector2i(200, 100); // TODO
		bean.levelEquipment = LevelEquipmentID::Weapon_icestaff;
		bean.value = 120;
		bean.attributes.manaRegenerationPerS = 4;
		bean.attributes.maxManaPoints = 10;
		bean.attributes.maxHealthPoints = 10;
		bean.attributes.damageIce = 30;
		bean.attributes.damagePhysical = 15;
		break;
	case ItemID::Equipment_weapon_rustysword:
		bean.description = "Itd_Rustysword";
		bean.name = "It_Rustysword";
		bean.type = ItemType::Equipment_weapon;
		bean.iconTextureLocation = sf::Vector2i(250, 100); // TODO
		bean.levelEquipment = LevelEquipmentID::Weapon_rustysword;
		bean.value = 15;
		bean.attributes.damagePhysical = 5;
		break;
	case ItemID::Equipment_back_purple:
		bean.description = "Itd_Purpleback"; // TODO
		bean.name = "It_Purpleback"; // TODO
		bean.type = ItemType::Equipment_back;
		bean.iconTextureLocation = sf::Vector2i(250, 100); // TODO
		bean.levelEquipment = LevelEquipmentID::Back_purple;
		bean.value = 10;
		bean.attributes.resistancePhysical = 1;
		bean.attributes.resistanceIce = 5;
		break;
	case ItemID::Food_Cheese:
		bean.description = "Itd_Cheese";
		bean.name = "It_Cheese";
		bean.type = ItemType::Food;
		bean.iconTextureLocation = sf::Vector2i(0, 100);
		bean.levelEquipment = LevelEquipmentID::Void;
		bean.attributes.currentHealthPoints = 40;
		bean.value = 3;
		break;
	case ItemID::Food_Bread:
		bean.description = "Itd_Bread";
		bean.name = "It_Bread";
		bean.type = ItemType::Food;
		bean.iconTextureLocation = sf::Vector2i(50, 100);
		bean.levelEquipment = LevelEquipmentID::Void;
		bean.attributes.currentHealthPoints = 30;
		bean.value = 2;
		break;
	case ItemID::Food_Water:
		bean.description = "Itd_Water";
		bean.name = "It_Water";
		bean.type = ItemType::Food;
		bean.iconTextureLocation = sf::Vector2i(100, 0);
		bean.levelEquipment = LevelEquipmentID::Void;
		bean.attributes.currentHealthPoints = 10;
		bean.attributes.currentManaPoints = 50;
		bean.value = 2;
		break;
	case ItemID::Food_CaveBerry:
		bean.description = "Itd_CaveBerry";
		bean.name = "It_CaveBerry";
		bean.type = ItemType::Food;
		bean.iconTextureLocation = sf::Vector2i(0, 0);
		bean.levelEquipment = LevelEquipmentID::Void;
		bean.attributes.currentHealthPoints = 20;
		bean.value = 3;
		break;
	case ItemID::Food_GlowingShroom:
		bean.description = "Itd_GlowingShroom";
		bean.name = "It_GlowingShroom";
		bean.type = ItemType::Food;
		bean.iconTextureLocation = sf::Vector2i(150, 50);
		bean.levelEquipment = LevelEquipmentID::Void;
		bean.attributes.currentHealthPoints = -20;
		bean.value = 2;
		break;
	case ItemID::Food_HealingHerb:
		bean.description = "Itd_HealingHerb";
		bean.name = "It_HealingHerb";
		bean.type = ItemType::Food;
		bean.iconTextureLocation = sf::Vector2i(50, 50);
		bean.levelEquipment = LevelEquipmentID::Void;
		bean.attributes.currentHealthPoints = 50;
		bean.value = 5;
		break;
	case ItemID::Quest_Letter:
		bean.description = "Itd_Letter";
		bean.name = "It_Letter";
		bean.type = ItemType::Quest;
		bean.iconTextureLocation = sf::Vector2i(200, 0);
		bean.levelEquipment = LevelEquipmentID::Void;
		bean.value = 0;
		break;
	case ItemID::Misc_GoldenGoblet:
		bean.description = "Itd_GoldenGoblet";
		bean.name = "It_GoldenGoblet";
		bean.type = ItemType::Misc;
		bean.iconTextureLocation = sf::Vector2i(0, 50);
		bean.levelEquipment = LevelEquipmentID::Void;
		bean.value = 15;
		break;
	case ItemID::Equipment_ring_ringOfLesserHealth:
		bean.description = "Itd_RingOfLesserHealth";
		bean.name = "It_RingOfLesserHealth";
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