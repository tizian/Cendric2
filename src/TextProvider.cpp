#include "TextProvider.h"

using namespace std;

TextProvider* g_textProvider;

void TextProvider::init()
{
	addText(Texts::New_game, "New Game", "Neues Spiel");
	addText(Texts::Load_game, "Load Game", "Spiel laden");
	addText(Texts::Save_game, "Save Game", "Spiel speichern");
	addText(Texts::Options, "Options", "Einstellungen");
	addText(Texts::Continue_game, "Resume", "Spiel fortsetzen");
	addText(Texts::Credits, "Credits", "Mitwirkende");
	addText(Texts::Exit, "Exit", "Beenden");
	addText(Texts::Start_game, "Play", "Spiel starten");
	addText(Texts::Back, "Back", "Zurück");
	addText(Texts::Yes, "Yes", "Ja");
	addText(Texts::No, "No", "Nein");
	addText(Texts::Question_loadGame, "Load game?\nAll unsaved progress will\nbe lost.", "Spiel laden?\nUngespeicherter Fortschritt\ngeht verloren.");
	addText(Texts::Question_startNewGame, "Start new game?\nAll unsaved progress will\nbe lost.", "Neues Spiel starten?\nUngespeicherter Fortschritt\ngeht verloren.");

	// Inventory & Attributes
	addText(Texts::Inventory, "Inventory", "Inventar");
	addText(Texts::CharacterInfo, "Character Info", "Charakter Info");
	addText(Texts::Gold, "Gold", "Gold");
	addText(Texts::Attribute_health, "Health", "Lebensenergie");
	addText(Texts::Attribute_mana, "Mana", "Mana");
	addText(Texts::Attribute_healthRegeneration, "Health regeneration", "Lebensregeneration");
	addText(Texts::Attribute_manaRegeneration, "Mana regeneration", "Manaregeneration");
	addText(Texts::Attribute_damageFire, "Fire damage", "Feuerschaden");
	addText(Texts::Attribute_damageIce, "Ice damage", "Eisschaden");
	addText(Texts::Attribute_damagePhysical, "Physical damage", "Physischer Schaden");
	addText(Texts::Attribute_resistanceFire, "Fire resistance", "Feuerresistenz");
	addText(Texts::Attribute_resistanceIce, "Ice resistance", "Eisresistenz");
	addText(Texts::Attribute_resistancePhysical, "Armour", "Rüstung");
	
	addText(Texts::Warn_itemTooFarAway, "Out of range", "Ich muss näher ran");

	// level item tooltips
	addText(Texts::Levelitem_tooltip_cheese, "A nice moist cheese", "Ein hübscher saftiger Käse");

	// item descriptions
	addText(Texts::Item_description_wizardhat, "A perfect fit for a mage.", "Perfekt für einen Magier.");
	addText(Texts::Item_description_staff_ice, "Feels ice cold.", "Fühlt sich eiskalt an.");
	addText(Texts::Item_description_weapon_rustysword, "Doesn't seem to have been used lately.", "Wurde wohl schon länger nicht mehr benutzt.");
	addText(Texts::Item_description_Food_Bread, "An ordinary loaf of bread.", "Ein einfaches Brot.");
	addText(Texts::Item_description_Food_Cheese, "Smelly.", "Riecht nicht sonderlich gut.");
	addText(Texts::Item_description_Food_Water, "Fresh clear water.", "Frisches klares Wasser.");
	
	// item  names
	addText(Texts::Item_name_food_bread, "Bread", "Brot");
	addText(Texts::Item_name_food_cheese, "Cheese", "Käse");
	addText(Texts::Item_name_food_water, "Water", "Wasser");
	addText(Texts::Item_name_staff_ice, "Ice staff", "Eisstab");
	addText(Texts::Item_name_weapon_rustysword, "Rusty sword", "Rostiges Schwert");
	addText(Texts::Item_name_wizardhat_blue, "Blue wizardhat", "Blauer Spitzhut");
	addText(Texts::Item_name_wizardhat_grey, "Grey wizardhat", "Grauer Spitzhut");
}

const std::string& TextProvider::getText(Texts key)
{
	switch (m_language)
	{
	case Language::Lang_DE:
		return m_germanTexts[key];
	case Language::Lang_EN:
		return m_englishTexts[key];
	default:
		// unexpected
		return m_englishTexts[key];;
	}
}

void TextProvider::setLanguage(Language lang)
{
	m_language = lang;
}

void TextProvider::addText(Texts key, const std::string& english, const std::string& german)
{
	m_englishTexts.insert({ key, english });
	m_germanTexts.insert({ key, german });
}


