#pragma once

#include "global.h"
#include "LevelMovableGameObject.h"
#include "Level.h"
#include "InputController.h"
#include "SpellManager.h"
#include "Screen.h"
#include "CharacterCore.h"

// Cendric in a level
class LevelMainCharacter : public LevelMovableGameObject
{
public:
	LevelMainCharacter(Level* level);
	~LevelMainCharacter();

	void load() override;
	void onHit(Spell* spell);
	void render(sf::RenderTarget& target) override;
	void update(const sf::Time& frameTime) override;

	void setCharacterCore(CharacterCore* core);
	
	float getConfiguredMaxVelocityY() const override;
	float getConfiguredMaxVelocityX() const override;
	GameObjectType getConfiguredType() const override;
	sf::Color getConfiguredDebugColor() const override;

protected:
	float getConfiguredDampingGroundPersS() const;
	// handle input and calculate the next position
	void handleInput() override;

private:
	// TODO load this map from configuration
	std::map<Key, SpellID> m_keyMap;
	CharacterCore* m_core;
	AttributeBean* m_attributes;

	// TODO [tiz] this is a part of the interface. Refactor / Move.
	void updateInterfaceBars();
	sf::RectangleShape m_hpBarOutline;
	sf::RectangleShape m_hpBar;
	sf::RectangleShape m_manaBarOutline;
	sf::RectangleShape m_manaBar;
	const float BAR_HEIGHT = 10.f;
	const float BAR_WIDTH = 200.f;
	const float BAR_OUTLINE_THICKNESS = 2.f;
	const float BAR_LEFT = 10.f;
	const float BAR_TOP = 10.f;
};