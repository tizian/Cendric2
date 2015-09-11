#pragma once

#include "global.h"
#include "GameObject.h"

#include "GUI/BitmapText.h"
#include "ResourceManager.h"
#include "GUI/InventoryDescriptionWindow.h"

class CharacterCore;

class ModifierSlot : public GameObject
{
public:
	// constructor for filled slots.
	ModifierSlot(const SpellModifier& modifier);
	// constructor for empty slots.
	ModifierSlot();

	void select();
	void deselect();
	// activate restores the colors of this slot
	void activate();
	// deactivate sets the colors of this slot all grey.
	void deactivate();

	void highlight(bool highlight);

	void render(sf::RenderTarget& renderTarget) override;
	void onLeftJustPressed() override;

	void setPosition(const sf::Vector2f& pos) override;

	// asks if it is clicked and sets the member bool to false again (!)
	bool isClicked();
	
	GameObjectType getConfiguredType() const override;
	const SpellModifier& getModifier() const;

	static const float SIDE_LENGTH;
	static const float MARGIN;

private:
	bool m_isClicked = false;
	bool m_isSelected = false;

	SpellModifier m_spellModifier;

	sf::RectangleShape m_inside;
	sf::RectangleShape m_outside;
};