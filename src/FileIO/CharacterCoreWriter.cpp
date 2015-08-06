#include "FileIO/CharacterCoreWriter.h"

using namespace std;

bool CharacterCoreWriter::createFile(const std::string& filename) const
{
	if (std::ifstream(filename))
	{
		g_logger->logInfo("CharacterCoreWriter", "File already exists: " + filename);
		return false;
	}
	std::ofstream file(filename);
	g_logger->logInfo("CharacterCoreWriter", "Created new file: " + filename);
	if (!file)
	{
		g_logger->logError("CharacterCoreWriter", "File could not be created: " + filename);
		return false;
	}
	return true;
}

bool CharacterCoreWriter::saveToFile(const std::string& filename, const CharacterCoreData& data) const
{
	ofstream savefile(filename, ios::trunc);
	if (savefile.is_open())
	{
		savefile << writeTimePlayed(data);
		savefile << writeSaveGameName(data);
		savefile << writeDateSaved(data);
		savefile << writeLevelID(data);
		savefile << writeLevelPosition(data);
		savefile << writeMapID(data);
		savefile << writeMapPosition(data);
		savefile << writeAttributes(data);
		savefile << writeGold(data);
		savefile << writeItemID(data);
		savefile << writeEquippedWeaponSlots(data);
		savefile << writeEquippedItems(data);
		savefile << writeLevelKilled(data);
		savefile << writeLevelLooted(data);
		savefile << writeQuestStates(data);
		savefile << writeNPCStates(data);

		savefile.close();
	}
	else
	{
		g_logger->logError("CharacterCoreWriter", "Unable to open file: " + string(filename));
		return false;
	}
	return true;
}

std::string CharacterCoreWriter::writeTimePlayed(const CharacterCoreData& data) const
{
	string timePlayed = "# time played, in seconds\n";
    return timePlayed.append(string(TIME_PLAYED) + ":" + to_string(static_cast<int>(std::floor(data.timePlayed.asSeconds()))) + "\n");
}

std::string CharacterCoreWriter::writeSaveGameName(const CharacterCoreData& data) const
{
	return string(SAVE_GAME_NAME) + ":" + data.saveGameName + "\n";
}

std::string CharacterCoreWriter::writeDateSaved(const CharacterCoreData& data) const
{
	return string(DATE_SAVED) + ":" + to_string(data.dateSaved) + "\n";
}

std::string CharacterCoreWriter::writeMapID(const CharacterCoreData& data) const
{
	return string(MAP_ID) + ":" + to_string(static_cast<int>(data.currentMap)) + "\n";
}

std::string CharacterCoreWriter::writeMapPosition(const CharacterCoreData& data) const
{
	return string(MAP_POSITION) + ":" + to_string(data.currentMapPosition.x) + "," + to_string(data.currentMapPosition.y) + "\n";
}

std::string CharacterCoreWriter::writeLevelID(const CharacterCoreData& data) const
{
	return string(LEVEL_ID) + ":" + to_string(static_cast<int>(data.currentLevel)) + "\n";
}

std::string CharacterCoreWriter::writeLevelPosition(const CharacterCoreData& data) const
{
	return string(LEVEL_POSITION) + ":" + to_string(data.currentLevelPosition.x) + "," + to_string(data.currentLevelPosition.y) + "\n";
}

std::string CharacterCoreWriter::writeLevelKilled(const CharacterCoreData& data) const
{
	string levelsKilled = "# levels killed:\n"; 
	
	for (auto it : data.levelKilled)
	{
		string levelKilled = string(LEVEL_KILLED);
		levelKilled.append(":");
		levelKilled.append(to_string(static_cast<int>(it.first)));
		for (auto it2 : it.second)
		{
			string boolean = it2 ? "1" : "0";
			levelKilled.append("," + boolean);
		}
		levelKilled.append("\n");
		levelsKilled.append(levelKilled);
	}
	return levelsKilled;
}

std::string CharacterCoreWriter::writeLevelLooted(const CharacterCoreData& data) const
{
	string levelsLooted = "# levels looted:\n";

	for (auto it : data.levelLooted)
	{
		string levelLooted = string(LEVEL_LOOTED);
		levelLooted.append(":");
		levelLooted.append(to_string(static_cast<int>(it.first)));
		for (auto it2 : it.second)
		{
			string boolean = it2 ? "1" : "0";
			levelLooted.append("," + boolean);
		}
		levelLooted.append("\n");
		levelsLooted.append(levelLooted);
	}
	return levelsLooted;
}

std::string CharacterCoreWriter::writeAttributes(const CharacterCoreData& data) const
{
	string orderComment = "# in this order: maxHealthPoints, healthRegenerationPerS, haste, critical, \n# damagePhysical, damageFire, damageIce, damageShadow, damageLight, \n# resistancePhysical, resistanceFire, resistanceIce, resistanceShadow, resistanceIce. \n";
	string attributeString = orderComment.append(string(ATTRIBUTES));
	attributeString.append(":");
	attributeString.append(to_string(data.attributes.maxHealthPoints) + ",");
	attributeString.append(to_string(data.attributes.healthRegenerationPerS) + ",");
	attributeString.append(to_string(data.attributes.haste) + ",");
	attributeString.append(to_string(data.attributes.critical) + ",");

	attributeString.append(to_string(data.attributes.damagePhysical) + ",");
	attributeString.append(to_string(data.attributes.damageFire) + ",");
	attributeString.append(to_string(data.attributes.damageIce) + ",");
	attributeString.append(to_string(data.attributes.damageShadow) + ",");
	attributeString.append(to_string(data.attributes.damageLight) + ",");

	attributeString.append(to_string(data.attributes.resistancePhysical) + ",");
	attributeString.append(to_string(data.attributes.resistanceFire) + ",");
	attributeString.append(to_string(data.attributes.resistanceIce) + ",");
	attributeString.append(to_string(data.attributes.resistanceShadow) + ",");
	attributeString.append(to_string(data.attributes.resistanceLight));
	attributeString.append("\n");

	return attributeString;
}

std::string CharacterCoreWriter::writeGold(const CharacterCoreData& data) const
{
	return string(GOLD) + ":" + to_string(data.gold) + "\n";
}

std::string CharacterCoreWriter::writeQuestStates(const CharacterCoreData& data) const
{
	string quests = "# quest states:\n";

	for (auto it : data.questStates)
	{
		string quest = string(QUEST_STATE);
		quest.append(":");
		quest.append(to_string(static_cast<int>(it.first)));
		quest.append(",");
		quest.append(to_string(static_cast<int>(it.second)));
		quest.append("\n");
		quests.append(quest);
	}
	return quests;
}

std::string CharacterCoreWriter::writeNPCStates(const CharacterCoreData& data) const
{
	string npcs = "# npc states:\n";

	for (auto it : data.npcStates)
	{
		string npc = string(NPC_STATE);
		npc.append(":");
		npc.append(to_string(static_cast<int>(it.first)));
		npc.append(",");
		npc.append(to_string(static_cast<int>(it.second)));
		npc.append("\n");
		npcs.append(npc);
	}
	return npcs;
}

std::string CharacterCoreWriter::writeItemID(const CharacterCoreData& data) const
{
	string items = "# items in inventory:\n";

	for (auto it : data.items)
	{
		string item = string(ITEM_ID);
		item.append(":");
		item.append(it.first);
		item.append(",");
		item.append(to_string(it.second));
		item.append("\n");
		items.append(item);
	}
	return items;
}
std::string CharacterCoreWriter::writeEquippedWeaponSlots(const CharacterCoreData& data) const
{
	string weaponSlots = "# weapon slots. it is always: spell ID+, (modifier type, level)*:\n";
	for (auto& it : data.equippedWeaponSlots)
	{
		weaponSlots.append(string(EQUIPPED_SPELLSLOT) + ":");
		weaponSlots.append(to_string(static_cast<int>(it.first)));
		for (auto& it2 : it.second)
		{
			weaponSlots.append(",");
			weaponSlots.append(to_string(static_cast<int>(it2.type)) + ",");
			weaponSlots.append(to_string(it2.level));
		}
		weaponSlots.append("\n");
	}
	return weaponSlots;
}

std::string CharacterCoreWriter::writeEquippedItems(const CharacterCoreData& data) const
{
	string equipment = "# equipment:\n";

	equipment.append(string(EQUIPPED_BACK) + ":" + data.equippedBack + "\n");
	equipment.append(string(EQUIPPED_BODY) + ":" + data.equippedBody + "\n");
	equipment.append(string(EQUIPPED_HEAD) + ":" + data.equippedHead + "\n");
	equipment.append(string(EQUIPPED_NECK) + ":" + data.equippedNeck + "\n");
	equipment.append(string(EQUIPPED_RING_1) + ":" + data.equippedRing1 + "\n");
	equipment.append(string(EQUIPPED_RING_2) + ":" + data.equippedRing2 + "\n");
	equipment.append(string(EQUIPPED_WEAPON) + ":" + data.equippedWeapon + "\n");

	return equipment;
}