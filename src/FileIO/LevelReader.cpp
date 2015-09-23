#include "FileIO/LevelReader.h"

#ifndef XMLCheckResult
	#define XMLCheckResult(result) if (result != XML_SUCCESS) {g_logger->logError("LevelReader", "XML file could not be read, error: " + std::to_string(static_cast<int>(result))); return false; }
#endif

LevelReader::LevelReader()
{
	initMaps();
}

LevelReader::~LevelReader()
{
	m_levelItemMap.clear();
	m_enemyMap.clear();
}

void LevelReader::initMaps()
{
	m_enemyMap.insert({
		{ 1, EnemyID::Rat },
		{ 2, EnemyID::FireRat },
	});
}

bool LevelReader::readLevel(const std::string& fileName, LevelData& data)
{
	XMLDocument xmlDoc;
	XMLError result = xmlDoc.LoadFile(fileName.c_str());
	XMLCheckResult(result);

	XMLElement* map = xmlDoc.FirstChildElement("map");
	if (map == nullptr) 
	{
		g_logger->logError("LevelReader", "XML file could not be read, no map node found.");
		return false;
	}
	
	if (!readLevelProperties(map, data)) return false;
	if (!readFirstGridIDs(map, data)) return false;
	if (!readLayers(map, data)) return false;
	if (!readObjects(map, data)) return false;

	updateData(data);
	if (!checkData(data)) return false;
	return true;
}

bool LevelReader::readLights(XMLElement* objectgroup, LevelData& data) const
{
	XMLElement* object = objectgroup->FirstChildElement("object");

	while (object != nullptr)
	{
		int x;
		XMLError result = object->QueryIntAttribute("x", &x);
		XMLCheckResult(result);

		int y;
		result = object->QueryIntAttribute("y", &y);
		XMLCheckResult(result);

		int width;
		result = object->QueryIntAttribute("width", &width);
		XMLCheckResult(result);

		int height;
		result = object->QueryIntAttribute("height", &height);
		XMLCheckResult(result);

		LightBean bean;
		bean.radius.x = width / 2.f;
		bean.radius.y = height / 2.f;
		bean.center.x = x + bean.radius.x;
		bean.center.y = y + bean.radius.y;

		// brightness for light bean
		XMLElement* properties = object->FirstChildElement("properties");
		if (properties != nullptr)
		{
			XMLElement* _property = properties->FirstChildElement("property");
			while (_property != nullptr)
			{
				const char* textAttr = nullptr;
				textAttr = _property->Attribute("name");
				if (textAttr == nullptr)
				{
					g_logger->logError("LevelReader", "XML file could not be read, no objectgroup->object->properties->property->name attribute found for light bean.");
					return false;
				}
				std::string name = textAttr;

				if (name.compare("brightness") == 0)
				{
					float brightness;
					result = _property->QueryFloatAttribute("value", &brightness);
					XMLCheckResult(result);
					if (brightness < 0.f || brightness > 1.f)
					{
						brightness = 1.f;
						g_logger->logWarning("LevelReader", "Brightness must be between 0 and 1. It was " + std::to_string(brightness) + ", it is now 1");
					}
					bean.brightness = brightness;
				}
				else
				{
					g_logger->logError("LevelReader", "XML file could not be read, unknown objectgroup->object->properties->property->name attribute found for light bean.");
					return false;
				}
				_property = _property->NextSiblingElement("property");
			}
		}

		data.lights.push_back(bean);
		object = object->NextSiblingElement("object");
	}
	return true;
}

bool LevelReader::readLevelExits(XMLElement* objectgroup, LevelData& data) const
{
	XMLElement* object = objectgroup->FirstChildElement("object");

	while (object != nullptr)
	{
		int x;
		XMLError result = object->QueryIntAttribute("x", &x);
		XMLCheckResult(result);

		int y;
		result = object->QueryIntAttribute("y", &y);
		XMLCheckResult(result);

		int width;
		result = object->QueryIntAttribute("width", &width);
		XMLCheckResult(result);

		int height;
		result = object->QueryIntAttribute("height", &height);
		XMLCheckResult(result);

		LevelExitBean bean;
		bean.levelExitRect = sf::FloatRect(static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height));
		bean.mapID = "";
		bean.mapSpawnPoint.x = -1.f;
		bean.mapSpawnPoint.y = -1.f;

		// map spawn point for level exit
		XMLElement* properties = object->FirstChildElement("properties");
		if (properties == nullptr)
		{
			g_logger->logError("LevelReader", "XML file could not be read, no objectgroup->object->properties attribute found for level exit.");
			return false;
		}

		XMLElement* _property = properties->FirstChildElement("property");
		while (_property != nullptr)
		{
			const char* textAttr = nullptr;
			textAttr = _property->Attribute("name");
			if (textAttr == nullptr)
			{
				g_logger->logError("LevelReader", "XML file could not be read, no objectgroup->object->properties->property->name attribute found for level exit.");
				return false;
			}
			std::string name = textAttr;

			if (name.compare("map id") == 0)
			{
				textAttr = nullptr;
				textAttr = _property->Attribute("value");
				if (textAttr == nullptr)
				{
					g_logger->logError("LevelReader", "XML file could not be read, no objectgroup->object->properties->property->value attribute found for level exit map id.");
					return false;
				}
				bean.mapID = textAttr;
			}
			else if (name.compare("x") == 0)
			{
				XMLError result = _property->QueryIntAttribute("value", &x);
				XMLCheckResult(result);
				bean.mapSpawnPoint.x = static_cast<float>(x);
			}
			else if (name.compare("y") == 0)
			{
				XMLError result = _property->QueryIntAttribute("value", &y);
				XMLCheckResult(result);
				bean.mapSpawnPoint.y = static_cast<float>(y);
			}
			else
			{
				g_logger->logError("LevelReader", "XML file could not be read, unknown objectgroup->object->properties->property->name attribute found for level exit.");
				return false;
			}
			_property = _property->NextSiblingElement("property");
		}
		data.levelExits.push_back(bean);
		object = object->NextSiblingElement("object");
	}
	return true;
}

bool LevelReader::readChestTiles(XMLElement* objectgroup, LevelData& data) const
{
	XMLElement* object = objectgroup->FirstChildElement("object");

	while (object != nullptr)
	{
		int id;
		XMLError result = object->QueryIntAttribute("id", &id);
		XMLCheckResult(result);

		int gid;
		result = object->QueryIntAttribute("gid", &gid);
		XMLCheckResult(result);

		int x;
		result = object->QueryIntAttribute("x", &x);
		XMLCheckResult(result);

		int y;
		result = object->QueryIntAttribute("y", &y);
		XMLCheckResult(result);

		int offset = static_cast<int>(DynamicTileID::Chest) + m_firstGidDynamicTiles - 1;
		int skinNr = (gid == 0) ? 0 : ((gid - offset) / DYNAMIC_TILE_COUNT) + 1;

		data.chests.insert({ id, std::pair<int, sf::Vector2f>(skinNr, sf::Vector2f(static_cast<float>(x), static_cast<float>(y))) });

		// chest loot
		XMLElement* loot = object->FirstChildElement("properties");
		if (loot != nullptr)
		{
			std::pair<std::map<std::string, int>, int> items;
			items.second = 0;
			items.first.clear();
			XMLElement* item = loot->FirstChildElement("property");
			while (item != nullptr)
			{
				const char* textAttr = nullptr;
				textAttr = item->Attribute("name");
				if (textAttr == nullptr)
				{
					g_logger->logError("LevelReader", "XML file could not be read, no objectgroup->object->properties->property->name attribute found.");
					return false;
				}
				std::string itemText = textAttr;

				int amount;
				result = item->QueryIntAttribute("value", &amount);
				XMLCheckResult(result);

				if (itemText.compare("gold") == 0 || itemText.compare("Gold") == 0)
				{
					items.second += amount;
				}
				else if (itemText.compare("strength") == 0 || itemText.compare("Strength") == 0)
				{
					if (amount < 0 || amount > 4)
					{
						g_logger->logError("LevelReader", "XML file could not be read, strength attribute for chest is out of bounds (must be between 0 and 4).");
						return false;
					}
					data.chestStrength.insert({ id, amount });
				}
				else
				{
					items.first.insert({ itemText, amount });
				}

				item = item->NextSiblingElement("property");
			}
			data.chestLoot.insert({ id, items });
		}

		object = object->NextSiblingElement("object");
	}
	return true;
}

bool LevelReader::readEnemies(XMLElement* objectgroup, LevelData& data) const
{
	XMLElement* object = objectgroup->FirstChildElement("object");

	while (object != nullptr)
	{
		int id;
		XMLError result = object->QueryIntAttribute("id", &id);
		XMLCheckResult(result);

		int gid;
		result = object->QueryIntAttribute("gid", &gid);
		XMLCheckResult(result);

		int x;
		result = object->QueryIntAttribute("x", &x);
		XMLCheckResult(result);

		int y;
		result = object->QueryIntAttribute("y", &y);
		XMLCheckResult(result);

		gid = (gid == 0) ? gid : gid - m_firstGidEnemies + 1;
		if (m_enemyMap.find(gid) == m_enemyMap.end())
		{
			g_logger->logError("LevelReader", "Enemy ID not recognized: " + std::to_string(gid));
			return false;
		}

		data.enemies.insert({ id, std::pair<EnemyID, sf::Vector2f>(m_enemyMap.at(gid), sf::Vector2f(static_cast<float>(x), static_cast<float>(y))) });

		// enemy loot
		XMLElement* loot = object->FirstChildElement("properties");
		if (loot != nullptr)
		{
			std::pair<std::map<std::string, int>, int> items;
			items.second = 0;
			items.first.clear();
			XMLElement* item = loot->FirstChildElement("property");
			while (item != nullptr)
			{
				const char* textAttr = nullptr;
				textAttr = item->Attribute("name");
				if (textAttr == nullptr)
				{
					g_logger->logError("LevelReader", "XML file could not be read, no objectgroup->object->properties->property->name attribute found.");
					return false;
				}
				std::string itemText = textAttr;

				if (itemText.compare("questtarget") == 0)
				{
					textAttr = item->Attribute("value");
					if (textAttr == nullptr)
					{
						g_logger->logError("LevelReader", "XML file could not be read, quest target value attribute is void.");
						return false;
					}
					std::string questtargetText = textAttr;
					std::string questID = questtargetText.substr(0, questtargetText.find(","));
					questtargetText.erase(0, questtargetText.find(",") + 1);
					data.enemyQuesttarget.insert({id, std::pair<std::string, std::string>(questID, questtargetText)});
				}
				else
				{
					int amount;
					result = item->QueryIntAttribute("value", &amount);
					XMLCheckResult(result);

					if (itemText.compare("gold") == 0 || itemText.compare("Gold") == 0)
					{
						items.second += amount;
					}
					else
					{
						items.first.insert({ itemText, amount });
					}
				}

				item = item->NextSiblingElement("property");
			}
			data.enemyLoot.insert({ id, items});
		}

		object = object->NextSiblingElement("object");
	}
	return true;
}

bool LevelReader::readObjects(XMLElement* map, LevelData& data) const
{
	XMLElement* objectgroup = map->FirstChildElement("objectgroup");

	const char* textAttr;
	while (objectgroup != nullptr)
	{
		textAttr = nullptr;
		textAttr = objectgroup->Attribute("name");
		if (textAttr == nullptr)
		{
			g_logger->logError("LevelReader", "XML file could not be read, no objectgroup->name attribute found.");
			return false;
		}

		std::string name = textAttr;

		if (name.find("levelexits") != std::string::npos)
		{
			if (!readLevelExits(objectgroup, data)) return false;
		}
		else if (name.find("enemies") != std::string::npos)
		{
			if (!readEnemies(objectgroup, data)) return false;
		}
		else if (name.find("chests") != std::string::npos)
		{
			if (!readChestTiles(objectgroup, data)) return false;
		}
		else if (name.find("light") != std::string::npos)
		{
			if (!readLights(objectgroup, data)) return false;
		}
		else
		{
			g_logger->logError("LevelReader", "Objectgroup with unknown name found in level.");
			return false;
		}

		objectgroup = objectgroup->NextSiblingElement("objectgroup");
	}
	return true;
}

bool LevelReader::readLevelItemLayer(const std::string& layer, LevelData& data) const
{
	std::string layerData = layer;
	
	size_t pos = 0;

	int id;
	std::string levelItem;
	while ((pos = layerData.find(",")) != std::string::npos)
	{
		id = std::stoi(layerData.substr(0, pos));
		id = (id == 0) ? -1 : id - m_firstGidItems;
		layerData.erase(0, pos + 1);
		if (m_levelItemMap.find(id) == m_levelItemMap.end())
		{
			g_logger->logError("LevelReader", "Level item ID not recognized: " + std::to_string(id));
			return false;
		}
		levelItem = m_levelItemMap.at(id);
		data.levelItems.push_back(levelItem);
	}
	id = std::stoi(layerData);
	id = (id == 0) ? -1 : id - m_firstGidItems;
	if (m_levelItemMap.find(id) == m_levelItemMap.end())
	{
		g_logger->logError("LevelReader", "Level item ID not recognized: " + std::to_string(id));
		return false;
	}
	levelItem = m_levelItemMap.at(id);
	data.levelItems.push_back(levelItem);

	return true;
}

bool LevelReader::readDynamicTileLayer(DynamicTileID id, const std::string& layer, LevelData& data) const
{
	std::string layerData = layer;
	int offset = static_cast<int>(id) + m_firstGidDynamicTiles - 1;
	size_t pos = 0;
	std::vector<int> dynamicTileLayer;
	int skinNr;
	while ((pos = layerData.find(",")) != std::string::npos) 
	{
		skinNr = std::stoi(layerData.substr(0, pos));
		dynamicTileLayer.push_back(skinNr == 0 ? 0 : ((skinNr - offset) / DYNAMIC_TILE_COUNT) + 1);
		layerData.erase(0, pos + 1);
	}
	skinNr = std::stoi(layerData);
	dynamicTileLayer.push_back(skinNr == 0 ? 0 : ((skinNr - offset) / DYNAMIC_TILE_COUNT) + 1);

	data.dynamicTileLayers.push_back(std::pair<DynamicTileID, std::vector<int>>(id, dynamicTileLayer));

	return true;
}

bool LevelReader::readCollidableLayer(const std::string& layer, LevelData& data) const
{
	std::string layerData = layer;

	size_t pos = 0;
	std::vector<bool> collidableLayer;
	while ((pos = layerData.find(",")) != std::string::npos) {
		collidableLayer.push_back(std::stoi(layerData.substr(0, pos)) != 0);
		layerData.erase(0, pos + 1);
	}
	collidableLayer.push_back(std::stoi(layerData) != 0);

	data.collidableTiles = collidableLayer;
	return true;
}

bool LevelReader::readForegroundTileLayer(const std::string& layer, LevelData& data) const
{
	std::string layerData = layer;

	size_t pos = 0;
	std::vector<int> foregroundLayer;
	while ((pos = layerData.find(",")) != std::string::npos) {
		foregroundLayer.push_back(std::stoi(layerData.substr(0, pos)));
		layerData.erase(0, pos + 1);
	}
	foregroundLayer.push_back(std::stoi(layerData));

	data.foregroundTileLayers.push_back(foregroundLayer);
	return true;
}

bool LevelReader::readBackgroundTileLayer(const std::string& layer, LevelData& data) const
{
	std::string layerData = layer;
	
	size_t pos = 0;
	std::vector<int> backgroundLayer;
	while ((pos = layerData.find(",")) != std::string::npos) {
		backgroundLayer.push_back(std::stoi(layerData.substr(0, pos)));
		layerData.erase(0, pos + 1);
	}
	backgroundLayer.push_back(std::stoi(layerData));
	
	data.backgroundTileLayers.push_back(backgroundLayer);
	return true;
}

bool LevelReader::readLayers(XMLElement* map, LevelData& data) const
{
	XMLElement* layer = map->FirstChildElement("layer");

	const char* textAttr;
	while (layer != nullptr)
	{
		textAttr = nullptr;
		textAttr = layer->Attribute("name");
		if (textAttr == nullptr)
		{
			g_logger->logError("LevelReader", "XML file could not be read, no layer->name attribute found.");
			return false;
		}

		std::string name = textAttr;

		XMLElement* layerDataNode = layer->FirstChildElement("data");
		if (layerDataNode == nullptr)
		{
			g_logger->logError("LevelReader", "XML file could not be read, no layer->data found.");
			return false;
		}
		std::string layerData = layerDataNode->GetText();

		if (name.find("BG") != std::string::npos || name.find("bg") != std::string::npos)
		{
			if (!readBackgroundTileLayer(layerData, data)) return false;
		}
		else if (name.find("FG") != std::string::npos || name.find("fg") != std::string::npos)
		{
			if (!readForegroundTileLayer(layerData, data)) return false;
		}
		else if (name.find("dynamic checkpoint") != std::string::npos)
		{
			if (!readDynamicTileLayer(DynamicTileID::Checkpoint, layerData, data)) return false;
		}
		else if (name.find("dynamic ice") != std::string::npos)
		{
			if (!readDynamicTileLayer(DynamicTileID::Ice, layerData, data)) return false;
		}
		else if (name.find("dynamic spikestop") != std::string::npos)
		{
			if (!readDynamicTileLayer(DynamicTileID::SpikesTop, layerData, data)) return false;
		}
		else if (name.find("dynamic spikesbottom") != std::string::npos)
		{
			if (!readDynamicTileLayer(DynamicTileID::SpikesBottom, layerData, data)) return false;
		}
		else if (name.find("dynamic torch") != std::string::npos)
		{
			if (!readDynamicTileLayer(DynamicTileID::Torch, layerData, data)) return false;
		}
		else if (name.find("dynamic crumblyblock") != std::string::npos)
		{
			if (!readDynamicTileLayer(DynamicTileID::CrumblyBlock, layerData, data)) return false;
		}
		else if (name.find("dynamic water") != std::string::npos)
		{
			if (!readDynamicTileLayer(DynamicTileID::Water, layerData, data)) return false;
		}
		else if (name.find("dynamic shiftableblock") != std::string::npos)
		{
			if (!readDynamicTileLayer(DynamicTileID::ShiftableBlock, layerData, data)) return false;
		}
		else if (name.find("collidable") != std::string::npos)
		{
			if (!readCollidableLayer(layerData, data)) return false;
		}
		else if (name.find("items") != std::string::npos)
		{
			if (!readLevelItemLayer(layerData, data)) return false;
		}
		else
		{
			g_logger->logError("LevelReader", "Layer with unknown name found in level.");
			return false;
		}

		layer = layer->NextSiblingElement("layer");
	}
	return true;
}

bool LevelReader::readItemIDs(XMLElement* _tile)
{
	m_levelItemMap.clear();
	m_levelItemMap.insert({ -1, "" });
	XMLElement* tile = _tile;

	while (tile != nullptr)
	{
		int tileID;
		XMLError result = tile->QueryIntAttribute("id", &tileID);
		XMLCheckResult(result);

		XMLElement* properties = tile->FirstChildElement("properties");
		if (properties == nullptr)
		{
			g_logger->logError("LevelReader", "Could not read item tile properties, no tileset->tile->properties tag found.");
			return false;
		}
		XMLElement* _property = properties->FirstChildElement("property");
		if (_property == nullptr)
		{
			g_logger->logError("LevelReader", "Could not read item tile properties, no tileset->tile->properties->property tag found.");
			return false;
		}
		const char* textAttr = nullptr;
		textAttr = _property->Attribute("name");
		if (textAttr == nullptr)
		{
			g_logger->logError("LevelReader", "XML file could not be read, no tileset->tile->properties->property name attribute found.");
			return false;
		}
		std::string name = textAttr;
		if (name.compare("id") != 0)
		{
			g_logger->logError("LevelReader", "XML file could not be read, wrong tile property (not \"id\").");
			return false;
		}
		textAttr = nullptr;
		textAttr = _property->Attribute("value");
		if (textAttr == nullptr)
		{
			g_logger->logError("LevelReader", "XML file could not be read, no tileset->tile->properties->property value attribute found.");
			return false;
		}

		m_levelItemMap.insert({ tileID, std::string(textAttr) });

		tile = tile->NextSiblingElement("tile");
	}

	return true;
}

bool LevelReader::readFirstGridIDs(XMLElement* map, LevelData& data)
{
	XMLElement* tileset = map->FirstChildElement("tileset");

	m_firstGidDynamicTiles = 0;
	m_firstGidEnemies = 0;
	m_firstGidItems = 0;
	const char* textAttr;
	while (tileset != nullptr)
	{
		textAttr = nullptr;
		textAttr = tileset->Attribute("name");
		if (textAttr == nullptr)
		{
			g_logger->logError("LevelReader", "XML file could not be read, no tileset->name attribute found.");
			return false;
		}
		std::string name = textAttr;
		
		int gid;
		XMLError result = tileset->QueryIntAttribute("firstgid", &gid);
		XMLCheckResult(result);
		
		if (name.find("dynamic_tiles") != std::string::npos)
		{
			m_firstGidDynamicTiles = gid;
		}
		else if (name.find("enemies") != std::string::npos)
		{
			m_firstGidEnemies = gid;
		}
		else if (name.find("levelitems") != std::string::npos)
		{
			m_firstGidItems = gid;
			if (!readItemIDs(tileset->FirstChildElement("tile"))) return false;
		}

		tileset = tileset->NextSiblingElement("tileset");
	}

	if (m_firstGidItems <= 0 || m_firstGidDynamicTiles <= 0 || m_firstGidEnemies <= 0)
	{
		g_logger->logError("LevelReader", "Could not read firstgids, at least one of the required tilesets is missing.");
		return false;
	}
	return true;
}

bool LevelReader::readLevelName(XMLElement* _property, LevelData& data) const
{
	// we've found the property "name"
	const char* textAttr = nullptr;
	textAttr = _property->Attribute("value");
	if (textAttr == nullptr)
	{
		g_logger->logError("LevelReader", "XML file could not be read, no value attribute found (map->properties->property->name=name).");
		return false;
	}
	data.name = textAttr;
	return true;
}

bool LevelReader::readDimming(XMLElement* _property, LevelData& data) const
{
	// we've found the property "dimming"
	float dimming = 0.f;
	XMLError result = _property->QueryFloatAttribute("value", &dimming);
	XMLCheckResult(result);

	if (dimming < 0.0f || dimming > 1.0f)
	{
		g_logger->logError("LevelReader", "XML file could not be read, dimming value not allowed (only [0,1]).");
		return false;
	}
	data.dimming = dimming;
	return true;
}


bool LevelReader::readTilesetPath(XMLElement* _property, LevelData& data) const
{
	// we've found the property "tilesetpath"
	const char* textAttr = nullptr;
	textAttr = _property->Attribute("value");
	if (textAttr == nullptr)
	{
		g_logger->logError("LevelReader", "XML file could not be read, no value attribute found (map->properties->property->name=tilesetpath).");
		return false;
	}
	data.tileSetPath = textAttr;
	return true;
}

bool LevelReader::readMusicPath(XMLElement* _property, LevelData& data) const
{
	// we've found the property "musicpath"
	const char* textAttr = nullptr;
	textAttr = _property->Attribute("value");
	if (textAttr == nullptr)
	{
		g_logger->logError("LevelReader", "XML file could not be read, no value attribute found (map->properties->property->name=musicpath).");
		return false;
	}
	data.musicPath = textAttr;
	return true;
}

bool LevelReader::readBackgroundLayers(XMLElement* _property, LevelData& data) const
{
	// we've found the property "backgroundlayers"
	const char* textAttr = nullptr;
	textAttr = _property->Attribute("value");
	if (textAttr == nullptr)
	{
		g_logger->logError("LevelReader", "XML file could not be read, no value attribute found (map->properties->property->name=backgroundlayer).");
		return false;
	}
	std::string backgroundLayerText = textAttr;

	size_t pos = 0;
	float distance = 0.f;
	std::string backgroundLayer = "";
	while ((pos = backgroundLayerText.find(",")) != std::string::npos)
	{
		distance = static_cast<float>(atof(backgroundLayerText.substr(0, pos).c_str()));
		backgroundLayerText.erase(0, pos + 1);
		if ((pos = backgroundLayerText.find(",")) != std::string::npos)
		{
			backgroundLayer = backgroundLayerText.substr(0, pos);
			backgroundLayerText.erase(0, pos + 1);	
		}
		else
		{
			backgroundLayer = backgroundLayerText;
			backgroundLayerText.clear();
		}
		BackgroundLayer layer;
		layer.load(backgroundLayer, distance);
		data.backgroundLayers.push_back(layer);
	}
	
	return true;
}

bool LevelReader::readLevelProperties(XMLElement* map, LevelData& data) const
{
	// check if renderorder is correct
	const char* textAttr = nullptr;
	textAttr = map->Attribute("renderorder");
	if (textAttr == nullptr) 
	{
		g_logger->logError("LevelReader", "XML file could not be read, no renderorder attribute found.");
		return false;
	}
	std::string renderorder = textAttr;
	if (renderorder.compare("right-down") != 0)
	{
		g_logger->logError("LevelReader", "XML file could not be read, renderorder is not \"right-down\".");
		return false;
	}

	// check if orientation is correct
	textAttr = nullptr;
	textAttr = map->Attribute("orientation");
	if (textAttr == nullptr)
	{
		g_logger->logError("LevelReader", "XML file could not be read, no orientation attribute found.");
		return false;
	}
	std::string orientation = textAttr;
	if (orientation.compare("orthogonal") != 0)
	{
		g_logger->logError("LevelReader", "XML file could not be read, renderorder is not \"orthogonal\".");
		return false;
	}

	// read map width and height
	XMLError result = map->QueryIntAttribute("width", &data.mapSize.x);
	XMLCheckResult(result);
	result = map->QueryIntAttribute("height", &data.mapSize.y);
	XMLCheckResult(result);

	// read tile size
	result = map->QueryIntAttribute("tilewidth", &data.tileSize.x);
	XMLCheckResult(result);
	result = map->QueryIntAttribute("tileheight", &data.tileSize.y);
	XMLCheckResult(result);

	// read level properties
	XMLElement* properties = map->FirstChildElement("properties");
	if (properties == nullptr)
	{
		g_logger->logError("LevelReader", "XML file could not be read, no properties node found.");
		return false;
	}
	XMLElement* _property = properties->FirstChildElement("property");

	while (_property != nullptr)
	{
		textAttr = nullptr;
		textAttr = _property->Attribute("name");
		if (textAttr == nullptr)
		{
			g_logger->logError("LevelReader", "XML file could not be read, no property->name attribute found.");
			return false;
		}
		std::string name = textAttr;
		if (name.compare("name") == 0)
		{
			if (!readLevelName(_property, data)) return false;
		}
		else if (name.compare("backgroundlayers") == 0)
		{
			if (!readBackgroundLayers(_property, data)) return false;
		}
		else if (name.compare("tilesetpath") == 0)
		{
			if (!readTilesetPath(_property, data)) return false;
		}
		else if (name.compare("musicpath") == 0)
		{
			if (!readMusicPath(_property, data)) return false;
		}
		else if (name.compare("dimming") == 0)
		{
			if (!readDimming(_property, data)) return false;
		}
		else
		{
			g_logger->logError("LevelReader", "XML file could not be read, unknown name attribute found in properties (map->properties->property->name).");
			return false;
		}

		_property = _property->NextSiblingElement("property");
	}
	
	return true;
}

void LevelReader::updateData(LevelData& data)  const
{
	// calculate collidable tiles
	int x = 0;
	int y = 0;
	std::vector<bool> xLine;

	for (std::vector<bool>::iterator it = data.collidableTiles.begin(); it != data.collidableTiles.end(); ++it)
	{
		xLine.push_back((*it));
		if (x + 1 >= data.mapSize.x)
		{
			x = 0;
			y++;
			data.collidableTilePositions.push_back(xLine); // push back creates a copy of that vector.
			xLine.clear();
		}
		else
		{
			x++;
		}
	}

	// calculate dynamic tiles
	int tileWidth = data.tileSize.x;
	int tileHeight = data.tileSize.y;

	for (auto& it : data.dynamicTileLayers)
	{
		int x = 0;
		int y = 0;
		DynamicTileID id = it.first;

		if (id == DynamicTileID::Water)
		{
			// handle special dynamic tiles
			bool isReadingWaterTiles = false;
			float waterTileWidth = 0.f;
			DynamicTileBean bean;
			bean.id = id;
			for (auto& it2 : it.second)
			{
				if (it2 != 0)
				{
					if (isReadingWaterTiles)
					{
						waterTileWidth += tileWidth;
					}
					else
					{
						bean.position = sf::Vector2f(static_cast<float>(x * tileWidth), static_cast<float>(y * tileHeight));
						bean.skinNr = it2;
						bean.spawnPosition = y * data.mapSize.x + x;
						waterTileWidth = static_cast<float>(tileWidth);
						isReadingWaterTiles = true;
					}
				}
				else
				{
					if (isReadingWaterTiles)
					{
						isReadingWaterTiles = false;
						bean.size = sf::Vector2f(waterTileWidth, static_cast<float>(tileWidth));
						data.dynamicTiles.push_back(bean);
					}
				}
				if (x + 1 >= data.mapSize.x)
				{
					x = 0;
					y++;
					if (isReadingWaterTiles)
					{
						isReadingWaterTiles = false;
						bean.size = sf::Vector2f(waterTileWidth, static_cast<float>(tileWidth));
						data.dynamicTiles.push_back(bean);
					}
				}
				else
				{
					x++;
				}
			}
		}
		else
		{
			// normal tiles
			for (auto& it2 : it.second)
			{
				if (it2 != 0)
				{
					DynamicTileBean bean;
					bean.id = id;
					bean.position = sf::Vector2f(static_cast<float>(x * tileWidth), static_cast<float>(y * tileHeight));
					bean.skinNr = it2;
					bean.spawnPosition = y * data.mapSize.x + x;
					data.dynamicTiles.push_back(bean);
				}
				if (x + 1 >= data.mapSize.x)
				{
					x = 0;
					y++;
				}
				else
				{
					x++;
				}
			}
		}
	}

	// calculate level rect
	data.levelRect.left = 0;
	data.levelRect.top = 0;
	data.levelRect.height = static_cast<float>(data.tileSize.y * data.mapSize.y);
	data.levelRect.width = static_cast<float>(data.tileSize.x * data.mapSize.x);
}

bool LevelReader::checkData(LevelData& data) const
{
	if (data.mapSize.x == 0 || data.mapSize.y == 0)
	{
		g_logger->logError("LevelReader", "Error in level data : map size not set / invalid");
		return false;
	}
	if (data.tileSize.x == 0 || data.tileSize.y == 0)
	{
		g_logger->logError("LevelReader", "Error in level data: tile size not set / invalid");
		return false;
	}
	if (data.name.empty())
	{
		g_logger->logError("LevelReader", "Error in level data : level name not set / empty");
		return false;
	}
	if (data.tileSetPath.empty())
	{
		g_logger->logError("LevelReader", "Error in level data : tileset-path not set / empty");
		return false;
	}
	if (data.backgroundTileLayers.empty())
	{
		g_logger->logInfo("LevelReader", "No background tile layers set");
	}
	for (int i = 0; i < data.backgroundTileLayers.size(); i++)
	{
		if (data.backgroundTileLayers[i].empty())
		{
			g_logger->logError("LevelReader", "Error in level data : background tile layer " + std::to_string(i) + std::string(" empty"));
			return false;
		}
		if (data.backgroundTileLayers[i].size() != data.mapSize.x * data.mapSize.y)
		{
			g_logger->logError("LevelReader", "Error in level data : background tile layer " + std::to_string(i) + std::string(" has not correct size (map size)"));
			return false;
		}
	}
	if (data.foregroundTileLayers.empty())
	{
		g_logger->logInfo("LevelReader", "No foreground tile layers set");
	}
	for (int i = 0; i < data.foregroundTileLayers.size(); i++)
	{
		if (data.foregroundTileLayers[i].empty())
		{
			g_logger->logError("LevelReader", "Error in level data : foreground tile layer " + std::to_string(i) + std::string(" empty"));
			return false;
		}
		if (data.foregroundTileLayers[i].size() != data.mapSize.x * data.mapSize.y)
		{
			g_logger->logError("LevelReader", "Error in level data : foreground tile layer " + std::to_string(i) + std::string(" has not correct size (map size)"));
			return false;
		}
	}
	for (int i = 0; i < data.dynamicTileLayers.size(); i++)
	{
		if (data.dynamicTileLayers[i].first == DynamicTileID::VOID)
		{
			g_logger->logError("LevelReader", "Error in level data : dynamic tile ID not recognized");
			return false;
		}
		if (data.dynamicTileLayers[i].second.empty() || data.dynamicTileLayers[i].second.size() != data.mapSize.x * data.mapSize.y)
		{
			g_logger->logError("LevelReader", "Error in level data : dynamic tile layer has not correct size (map size)");
			return false;
		}
	}
	for (auto& it : data.levelExits)
	{
		if (it.levelExitRect.height <= 0.f || it.levelExitRect.width <= 0.f)
		{
			g_logger->logError("LevelReader", "Error in level data : level exit rectangle has volume negative or null.");
			return false;
		}
		if (it.mapID.empty())
		{
			g_logger->logError("LevelReader", "Error in level data : level exit map id is empty.");
			return false;
		}
		if (it.mapSpawnPoint.x < 0.f || it.mapSpawnPoint.y < 0.f)
		{
			g_logger->logError("LevelReader", "Error in level data : level exit map spawn point is negative.");
			return false;
		}
	}
	for (auto& it : data.enemyQuesttarget)
	{
		if (it.second.first.empty() || it.second.second.empty())
		{
			g_logger->logError("LevelReader", "Error in level data : enemy quest target value strings (quest id and name) must not be empty.");
			return false;
		}
	}
	if (data.levelItems.size() != data.mapSize.x * data.mapSize.y)
	{
		g_logger->logError("LevelReader", "Error in level data : level item layer has not correct size (map size)");
		return false;
	}
	if (data.collidableTiles.empty())
	{
		g_logger->logError("LevelReader", "Error in level data : collidable layer is empty (can be all zeros but must be set)");
		return false;
	}
	if (data.collidableTiles.size() != data.mapSize.x * data.mapSize.y)
	{
		g_logger->logError("LevelReader", "Error in level data : collidable layer has not correct size (map size)");
		return false;
	}

	return true;
}