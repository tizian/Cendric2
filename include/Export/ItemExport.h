#pragma once

#include "global.h"

#include <iostream>
#include <fstream>  

class Item;

/* exports items in the wiki format */
class ItemExport final {
private:
	ItemExport() {};

public:
	static void exportItems();

private:
	static void exportItem(Item* item);

	static void savePicture(Item* item);
	static void writeDescription(Item* item, std::ofstream& out);
	static void writeAttributes(Item* item, std::ofstream& out);
	static void writeConvertible(Item* item, std::ofstream& out);
	static void writeDocument(Item* item, std::ofstream& out);
	static void writeWeapon(Item* item, std::ofstream& out);
	static void writeInfobox(Item* item, std::ofstream& out);
	static void writeCategories(Item* item, std::ofstream& out);

	static const std::string EXPORT_PATH;
};