#pragma once

#include "global.h"
#include "GameObject.h"

#include "ResourceManager.h"
#include "GUI/Slot.h"

// class for clones: used when slots are dragged around
class SlotClone final : public GameObject {
public:
	SlotClone(const Slot* original);

	GameObjectType getConfiguredType() const override;

	void render(sf::RenderTarget& renderTarget) override;
	void setPosition(const sf::Vector2f& pos) override;

	const Slot *getOriginalSlot() const;

private:
	const Slot* m_original;

	sf::RectangleShape m_backgroundRect;
	sf::RectangleShape m_iconRect;
	sf::RectangleShape m_borderRect;

	sf::IntRect m_iconTextureRect;
};