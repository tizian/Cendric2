#pragma once

#include "global.h"
#include "GUI/Window.h"
#include "GUI/BitmapText.h"
#include "Enums/EnumNames.h"

#include "Structs/SpellModifier.h"

class ModifierDescriptionWindow : public Window
{
public:
	ModifierDescriptionWindow(const SpellModifier& modifier);
	~ModifierDescriptionWindow();

	void render(sf::RenderTarget& renderTarget) override;
	void setPosition(const sf::Vector2f& position) override;

private:
	BitmapText* m_descriptionText = nullptr;
};