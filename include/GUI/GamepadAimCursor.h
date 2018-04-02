#pragma once

#include "global.h"
#include "World/GameObject.h"

class LevelMainCharacter;

class GamepadAimCursor : public GameObject {
public:
	GamepadAimCursor(LevelMainCharacter* character);

	void update(const sf::Time& frameTime) override;
	void renderAfterForeground(sf::RenderTarget& target) override;
	
	void setVisible(bool visible);

	const sf::Vector2f& getCurrentPosition() const;

	GameObjectType getConfiguredType() const override { return GameObjectType::_Interface; }

private:
	LevelMainCharacter* m_mainChar;
	sf::Sprite m_cursorSprite;

	bool m_isVisible;
	bool m_isActive;

	sf::Vector2f m_currentPosition;
	sf::Vector2f m_currentAimOffset;
	static const float AIM_DISTANCE;
};
