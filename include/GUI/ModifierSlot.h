#pragma once

#include "global.h"
#include "GameObject.h"

#include "GUI/BitmapText.h"
#include "ResourceManager.h"
#include "GUI/ModifierDescriptionWindow.h"

class CharacterCore;

class ModifierSlot : public GameObject
{
public:
	// constructor for filled slots.
	ModifierSlot(const SpellModifier& modifier);
	// constructor for empty slots.
	ModifierSlot();
	~ModifierSlot();

	void select();
	void deselect();
	// activate restores the colors of this slot
	void activate();
	// deactivate sets the colors of this slot all grey.
	void deactivate();

	void highlight(bool highlight);

	void render(sf::RenderTarget& renderTarget) override;
	void renderAfterForeground(sf::RenderTarget& renderTarget) override;
	void onLeftJustPressed() override;
	void onMouseOver() override;
	void setNr(int nr);

	void setPosition(const sf::Vector2f& pos) override;

	// asks if it is clicked and sets the member bool to false again (!)
	bool isClicked();
	
	GameObjectType getConfiguredType() const override;
	const SpellModifier& getModifier() const;
	int getNr() const;

	static const float SIDE_LENGTH;
	static const float MARGIN;

private:
	bool m_isClicked = false;
	bool m_isSelected = false;
	int m_nr = -1;

	void initWindow();
	ModifierDescriptionWindow* m_descriptionWindow = nullptr;
	bool m_showDescriptionWindow = false;

	SpellModifier m_spellModifier;

	sf::RectangleShape m_inside;
	sf::RectangleShape m_outside;
};