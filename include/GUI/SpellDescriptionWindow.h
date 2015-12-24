#pragma once

#include "global.h"
#include "GUI/Window.h"
#include "GUI/BitmapText.h"
#include "TextProvider.h"
#include "Enums/EnumNames.h"
#include "Structs/SpellData.h"
#include "GUI/GUIConstants.h"
#include "Structs/AttributeData.h"
#include "Structs/SpellModifier.h"

class SpellDescriptionWindow : public Window {
public:
	SpellDescriptionWindow();

	// sets the spell ID and modifiers for the window and reloads everything.
	// the attributes should be the attributes of the core with the equipped items
	void reload(SpellID id, const std::vector<SpellModifier>& modifiers, const AttributeData* bean);
	void render(sf::RenderTarget& renderTarget) override;
	void setPosition(const sf::Vector2f& position) override;

	void show();
	void hide();

	static const float WIDTH;

private:
	bool m_isVisible = false;

	BitmapText m_titleText;
	BitmapText m_descriptionText;
	BitmapText m_statsText;
};