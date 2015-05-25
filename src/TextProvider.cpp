#include "TextProvider.h"

using namespace std;

TextProvider* g_textProvider;

void TextProvider::init()
{
	addText(Texts::New_game, "New Game", "Neues Spiel");
	addText(Texts::Load_game, "Load Game", "Spiel laden");
	addText(Texts::Start_game, "Play", "Starten");
	addText(Texts::Credits, "Credits", "Credits");
	addText(Texts::Exit, "Exit", "Beenden");

	addText(Texts::Warn_itemTooFarAway, "Item out of range", "Ich muss näher ran");

	// level item tooltips
	addText(Texts::Levelitem_tooltip_cheese, "A nice moist cheese", "Ein hübscher saftiger Käse");

	// item descriptions
	addText(Texts::Item_description_wizardhat, "A perfect fit for a mage.", "Perfekt für einen Magier.");
	addText(Texts::Item_description_staff_ice, "Feels ice cold.", "Fühlt sich eiskalt an.");
	addText(Texts::Item_description_Food_Bread, "An ordinary loaf of bread.", "Ein einfaches Brot.");
	addText(Texts::Item_description_Food_Cheese, "Smelly.", "Riecht nicht sonderlich gut.");
	addText(Texts::Item_description_Food_Water, "Fresh clear water.", "Frisches klares Wasser.");
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


