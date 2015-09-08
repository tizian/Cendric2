#include "FileIO/MapReader.h"

#ifndef XMLCheckResult
	#define XMLCheckResult(result) if (result != XML_SUCCESS) {g_logger->logError("MapReader", "XML file could not be read, error: " + std::to_string(static_cast<int>(result))); return false; }
#endif

using namespace std;

MapReader::MapReader()
{
}

MapReader::~MapReader()
{
	m_npcMap.clear();
}

bool MapReader::checkData(MapData& data) const
{
	if (data.mapSize.x == 0 || data.mapSize.y == 0)
	{
		g_logger->logError("MapReader", "Error in map data : map size not set / invalid");
		return false;
	}
	if (data.tileSize.x == 0 || data.tileSize.y == 0)
	{
		g_logger->logError("MapReader", "Error in map data : tile size not set / invalid");
		return false;
	}
	if (data.name.empty())
	{
		g_logger->logError("MapReader", "Error in map data : map name not set / empty");
		return false;
	}
	if (data.tileSetPath.empty())
	{
		g_logger->logError("MapReader", "Error in map data : tileset path not set / empty");
		return false;
	}
	if (data.backgroundLayers.empty())
	{
		g_logger->logError("MapReader", "Error in map data : no background layers set");
		return false;
	}
	for (int i = 0; i < data.backgroundLayers.size(); i++)
	{
		if (data.backgroundLayers[i].empty())
		{
			g_logger->logError("MapReader", "Error in map data : background layer " + std::to_string(i) + std::string(" empty"));
			return false;
		}
		if (data.backgroundLayers[i].size() != data.mapSize.x * data.mapSize.y)
		{
			g_logger->logError("MapReader", "Error in map data : background layer " + std::to_string(i) + std::string(" has not correct size (map size)"));
			return false;
		}
	}
	for (int i = 0; i < data.foregroundLayers.size(); i++)
	{
		if (data.foregroundLayers[i].empty())
		{
			g_logger->logError("MapReader", "Error in map data : foreground layer " + std::to_string(i) + std::string(" empty"));
			return false;
		}
		if (data.foregroundLayers[i].size() != data.mapSize.x * data.mapSize.y)
		{
			g_logger->logError("MapReader", "Error in map data : foreground layer " + std::to_string(i) + std::string(" has not correct size (map size)"));
			return false;
		}
	}
	for (auto it : data.mapExits)
	{
		if (it.mapExitRect.left < 0.0 || it.mapExitRect.top < 0.0 || it.mapExitRect.left >= data.mapSize.x * data.tileSize.x || it.mapExitRect.top >= data.mapSize.y * data.tileSize.y)
		{
			g_logger->logError("MapReader", "Error in map data : a map exit rect is out of range for this map.");
			return false;
		}
		if (it.levelID.empty())
		{
			g_logger->logError("MapReader", "Error in map data : map exit level id is empty.");
			return false;
		}
		if (it.levelSpawnPoint.x < 0.f || it.levelSpawnPoint.y < 0.f)
		{
			g_logger->logError("MapReader", "Error in map data : lmap exit level spawn point is negative.");
			return false;
		}
	}
	if (data.collidableTiles.empty())
	{
		g_logger->logError("MapReader", "Error in map data : collidable layer is empty");
		return false;
	}
	if (data.collidableTiles.size() != data.mapSize.x * data.mapSize.y)
	{
		g_logger->logError("MapReader", "Error in map data : collidable layer has not correct size (map size)");
		return false;
	}

	return true;
}

bool MapReader::readMapExits(XMLElement* objectgroup, MapData& data) const
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

		MapExitBean bean;
		bean.mapExitRect = sf::FloatRect(static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height));
		bean.levelID = "";
		bean.levelSpawnPoint.x = -1.f;
		bean.levelSpawnPoint.y = -1.f;

		// map spawn point for level exit
		XMLElement* properties = object->FirstChildElement("properties");
		if (properties == nullptr)
		{
			g_logger->logError("MapReader", "XML file could not be read, no objectgroup->object->properties attribute found for level exit.");
			return false;
		}

		XMLElement* _property = properties->FirstChildElement("property");
		while (_property != nullptr)
		{
			const char* textAttr = nullptr;
			textAttr = _property->Attribute("name");
			if (textAttr == nullptr)
			{
				g_logger->logError("MapReader", "XML file could not be read, no objectgroup->object->properties->property->name attribute found for level exit.");
				return false;
			}
			std::string name = textAttr;

			if (name.compare("level id") == 0)
			{
				textAttr = nullptr;
				textAttr = _property->Attribute("value");
				if (textAttr == nullptr)
				{
					g_logger->logError("MapReader", "XML file could not be read, no objectgroup->object->properties->property->value attribute found for level exit map id.");
					return false;
				}
				bean.levelID = textAttr;
			}
			else if (name.compare("x") == 0)
			{
				XMLError result = _property->QueryIntAttribute("value", &x);
				XMLCheckResult(result);
				bean.levelSpawnPoint.x = static_cast<float>(x);
			}
			else if (name.compare("y") == 0)
			{
				XMLError result = _property->QueryIntAttribute("value", &y);
				XMLCheckResult(result);
				bean.levelSpawnPoint.y = static_cast<float>(y);
			}
			else
			{
				g_logger->logError("MapReader", "XML file could not be read, unknown objectgroup->object->properties->property->name attribute found for level exit.");
				return false;
			}
			_property = _property->NextSiblingElement("property");
		}
		data.mapExits.push_back(bean);
		object = object->NextSiblingElement("object");
	}
	return true;
}

bool MapReader::readObjects(XMLElement* map, MapData& data) const
{
	XMLElement* objectgroup = map->FirstChildElement("objectgroup");

	const char* textAttr;
	while (objectgroup != nullptr)
	{
		textAttr = nullptr;
		textAttr = objectgroup->Attribute("name");
		if (textAttr == nullptr)
		{
			g_logger->logError("MapReader", "XML file could not be read, no objectgroup->name attribute found.");
			return false;
		}

		std::string name = textAttr;

		if (name.find("mapexits") != std::string::npos)
		{
			if (!readMapExits(objectgroup, data)) return false;
		}
		else
		{
			g_logger->logError("MapReader", "Objectgroup with unknown name found in level.");
			return false;
		}

		objectgroup = objectgroup->NextSiblingElement("objectgroup");
	}
	return true;
}

bool MapReader::readNPCLayer(const std::string& layer, MapData& data) const
{
	std::string layerData = layer;
	size_t pos = 0;
	std::vector<int> dynamicTileLayer;
	int id;
	std::string npcID;
	while ((pos = layerData.find(",")) != std::string::npos)
	{
		id = std::stoi(layerData.substr(0, pos));
		id = (id == 0) ? -1 : id - m_firstGidNPC;
		if (m_npcMap.find(id) == m_npcMap.end())
		{
			g_logger->logError("MapReader", "Npc ID not recognized: " + std::to_string(id));
			return false;
		}
		npcID = m_npcMap.at(id);
		data.npcs.push_back(npcID);
		layerData.erase(0, pos + 1);
	}
	id = std::stoi(layerData);
	id = (id == 0) ? -1 : id - m_firstGidNPC;
	if (m_npcMap.find(id) == m_npcMap.end())
	{
		g_logger->logError("MapReader", "Npc ID not recognized: " + std::to_string(id));
		return false;
	}
	npcID = m_npcMap.at(id);
	data.npcs.push_back(npcID);

	return true;
}

bool MapReader::readBackgroundTileLayer(const std::string& layer, MapData& data) const
{
	std::string layerData = layer;

	size_t pos = 0;
	std::vector<int> backgroundLayer;
	while ((pos = layerData.find(",")) != std::string::npos) {
		backgroundLayer.push_back(std::stoi(layerData.substr(0, pos)));
		layerData.erase(0, pos + 1);
	}
	backgroundLayer.push_back(std::stoi(layerData));

	data.backgroundLayers.push_back(backgroundLayer);
	return true;
}

bool MapReader::readCollidableLayer(const std::string& layer, MapData& data) const
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

bool MapReader::readForegroundTileLayer(const std::string& layer, MapData& data) const
{
	std::string layerData = layer;

	size_t pos = 0;
	std::vector<int> foregroundLayer;
	while ((pos = layerData.find(",")) != std::string::npos) {
		foregroundLayer.push_back(std::stoi(layerData.substr(0, pos)));
		layerData.erase(0, pos + 1);
	}
	foregroundLayer.push_back(std::stoi(layerData));

	data.foregroundLayers.push_back(foregroundLayer);
	return true;
}

bool MapReader::readLayers(XMLElement* map, MapData& data) const
{
	XMLElement* layer = map->FirstChildElement("layer");

	const char* textAttr;
	while (layer != nullptr)
	{
		textAttr = nullptr;
		textAttr = layer->Attribute("name");
		if (textAttr == nullptr)
		{
			g_logger->logError("MapReader", "XML file could not be read, no layer->name attribute found.");
			return false;
		}

		std::string name = textAttr;

		XMLElement* layerDataNode = layer->FirstChildElement("data");
		if (layerDataNode == nullptr)
		{
			g_logger->logError("MapReader", "XML file could not be read, no layer->data found.");
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
		else if (name.find("collidable") != std::string::npos)
		{
			if (!readCollidableLayer(layerData, data)) return false;
		}
		else if (name.find("npc") != std::string::npos)
		{
			if (!readNPCLayer(layerData, data)) return false;
		}
		else
		{
			g_logger->logError("MapReader", "Layer with unknown name found in map.");
			return false;
		}

		layer = layer->NextSiblingElement("layer");
	}
	return true;
}


bool MapReader::readFirstGridIDs(XMLElement* map, MapData& data) 
{
	XMLElement* tileset = map->FirstChildElement("tileset");

	m_firstGidNPC = 0;
	const char* textAttr;
	while (tileset != nullptr)
	{
		textAttr = nullptr;
		textAttr = tileset->Attribute("name");
		if (textAttr == nullptr)
		{
			g_logger->logError("MapReader", "XML file could not be read, no tileset->name attribute found.");
			return false;
		}
		std::string name = textAttr;

		int gid;
		XMLError result = tileset->QueryIntAttribute("firstgid", &gid);
		XMLCheckResult(result);

		if (name.find("npc") != std::string::npos)
		{
			m_firstGidNPC = gid;
			if (!readNpcIDs(tileset->FirstChildElement("tile"))) return false;
			break;
		}
		
		tileset = tileset->NextSiblingElement("tileset");
	}

	if (m_firstGidNPC <= 0)
	{
		g_logger->logError("MapReader", "Could not read firstgids, at least one of the required tilesets is missing.");
		return false;
	}
	return true;
}

bool MapReader::readNpcIDs(XMLElement* _tile)
{
	m_npcMap.clear();
	m_npcMap.insert({ -1, "" });
	XMLElement* tile = _tile;

	while (tile != nullptr)
	{
		int tileID;
		XMLError result = tile->QueryIntAttribute("id", &tileID);
		XMLCheckResult(result);

		XMLElement* properties = tile->FirstChildElement("properties");
		if (properties == nullptr)
		{
			g_logger->logError("MapReader", "Could not read npc tile properties, no tileset->tile->properties tag found.");
			return false;
		}
		XMLElement* _property = properties->FirstChildElement("property");
		if (_property == nullptr)
		{
			g_logger->logError("MapReader", "Could not read npc tile properties, no tileset->tile->properties->property tag found.");
			return false;
		}
		const char* textAttr = nullptr;
		textAttr = _property->Attribute("name");
		if (textAttr == nullptr)
		{
			g_logger->logError("MapReader", "XML file could not be read, no tileset->tile->properties->property name attribute found.");
			return false;
		}
		std::string name = textAttr;
		if (name.compare("id") != 0)
		{
			g_logger->logError("MapReader", "XML file could not be read, wrong tile property (not \"id\").");
			return false;
		}
		textAttr = nullptr;
		textAttr = _property->Attribute("value");
		if (textAttr == nullptr)
		{
			g_logger->logError("MapReader", "XML file could not be read, no tileset->tile->properties->property value attribute found.");
			return false;
		}

		m_npcMap.insert({ tileID, std::string(textAttr) });

		tile = tile->NextSiblingElement("tile");
	}
	
	return true;
}

bool MapReader::readMap(const char* fileName, MapData& data)
{
	XMLDocument xmlDoc;
	XMLError result = xmlDoc.LoadFile(fileName);
	XMLCheckResult(result);

	XMLElement* map = xmlDoc.FirstChildElement("map");
	if (map == nullptr)
	{
		g_logger->logError("MapReader", "XML file could not be read, no map node found.");
		return false;
	}

	if (!readMapProperties(map, data)) return false;
	if (!readFirstGridIDs(map, data)) return false;
	if (!readLayers(map, data)) return false;
	if (!readObjects(map, data)) return false;

	updateData(data);
	if (!checkData(data)) return false;
	return true;
}

bool MapReader::readMapName(XMLElement* _property, MapData& data) const
{
	// we've found the property "name"
	const char* textAttr = nullptr;
	textAttr = _property->Attribute("value");
	if (textAttr == nullptr)
	{
		g_logger->logError("MapReader", "XML file could not be read, no value attribute found (map->properties->property->name=name).");
		return false;
	}
	data.name = textAttr;
	return true;
}

bool MapReader::readTilesetPath(XMLElement* _property, MapData& data) const
{
	// we've found the property "tilesetpath"
	const char* textAttr = nullptr;
	textAttr = _property->Attribute("value");
	if (textAttr == nullptr)
	{
		g_logger->logError("MapReader", "XML file could not be read, no value attribute found (map->properties->property->name=backgroundlayer).");
		return false;
	}
	data.tileSetPath = textAttr;
	return true;
}

bool MapReader::readMapProperties(XMLElement* map, MapData& data) const
{
	// check if renderorder is correct
	const char* textAttr = nullptr;
	textAttr = map->Attribute("renderorder");
	if (textAttr == nullptr)
	{
		g_logger->logError("MapReader", "XML file could not be read, no renderorder attribute found.");
		return false;
	}
	std::string renderorder = textAttr;
	if (renderorder.compare("right-down") != 0)
	{
		g_logger->logError("MapReader", "XML file could not be read, renderorder is not \"right-down\".");
		return false;
	}

	// check if orientation is correct
	textAttr = nullptr;
	textAttr = map->Attribute("orientation");
	if (textAttr == nullptr)
	{
		g_logger->logError("MapReader", "XML file could not be read, no orientation attribute found.");
		return false;
	}
	std::string orientation = textAttr;
	if (orientation.compare("orthogonal") != 0)
	{
		g_logger->logError("MapReader", "XML file could not be read, renderorder is not \"orthogonal\".");
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
		g_logger->logError("MapReader", "XML file could not be read, no properties node found.");
		return false;
	}
	XMLElement* _property = properties->FirstChildElement("property");

	while (_property != nullptr)
	{
		textAttr = nullptr;
		textAttr = _property->Attribute("name");
		if (textAttr == nullptr)
		{
			g_logger->logError("MapReader", "XML file could not be read, no property->name attribute found.");
			return false;
		}
		std::string name = textAttr;
		if (name.compare("name") == 0)
		{
			if (!readMapName(_property, data)) return false;
		}
		else if (name.compare("tilesetpath") == 0)
		{
			if (!readTilesetPath(_property, data)) return false;
		}
		else
		{
			g_logger->logError("MapReader", "XML file could not be read, unknown name attribute found in properties (map->properties->property->name).");
			return false;
		}

		_property = _property->NextSiblingElement("property");
	}

	return true;
}

void MapReader::updateData(MapData& data) const
{
	int x = 0;
	int y = 0;

	vector<bool> xLine;

	// calculate collidable tiles
	for (std::vector<bool>::iterator it = data.collidableTiles.begin(); it != data.collidableTiles.end(); ++it) {

		xLine.push_back((*it));
		if (x + 1 >= data.mapSize.x)
		{
			x = 0;
			y++;
			data.collidableTileRects.push_back(xLine); // push back creates a copy of that vector.
			xLine.clear();
		}
		else
		{
			x++;
		}
	}

	// calculate map rect
	data.mapRect.left = 0;
	data.mapRect.top = 0;
	data.mapRect.height = static_cast<float>(data.tileSize.y * data.mapSize.y);
	data.mapRect.width = static_cast<float>(data.tileSize.x * data.mapSize.x);
}