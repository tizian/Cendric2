#pragma once

#include "global.h"
#include "GameObject.h"

#include "ResourceManager.h"
#include "GUI/ModifierSlot.h"

// class for the modifier slot clone that spawns when a modifier slot is dragged around.
class ModifierSlotClone : public GameObject
{
public:
	ModifierSlotClone(const ModifierSlot* original);

	void render(sf::RenderTarget& renderTarget) override;

	void setPosition(const sf::Vector2f& pos) override;

	GameObjectType getConfiguredType() const override;
	const SpellModifier& getModifier() const;

private:
	SpellModifier m_original;

	sf::RectangleShape m_inside;
	sf::RectangleShape m_outside;
};