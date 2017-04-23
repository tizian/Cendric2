#pragma once

#include "global.h"
#include "GameObject.h"
#include "TextProvider.h"
#include "ResourceManager.h"

#include "GUI/BitmapText.h"
#include "GUI/SlicedSprite.h"

class LevelMovableGameObject;

// a floating speech bubble for talking mobs in levels
class SpeechBubble final : public GameObject {
public:
	SpeechBubble(LevelMovableGameObject* owner);
	
	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;

	void setText(const std::string& text);
	void setPosition(const sf::Vector2f& position) override;
	void setFloatingHeight(float height);

	GameObjectType getConfiguredType() const override;

	void show();
	void hide();

private:
	LevelMovableGameObject* m_owner = nullptr;
	bool m_isVisible = true;
	// The height of the speech bubble above its owner, default is 30.f
	float m_floatingHeight = 30.f;
	BitmapText m_text;

	SlicedSprite m_mainLayer;
	SlicedSprite m_ornamentLayer;

	sf::Sprite m_pointer;

	// The minimal size, when no text is set
	static const float MIN_SIZE;
	// The maximal length of the text in x.
	static const float MAX_LENGTH;
	// The margin outside of the speech bubble text.
	static const float TEXT_MARGIN;
};