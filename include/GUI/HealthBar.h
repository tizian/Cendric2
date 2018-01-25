#pragma once

#include "global.h"
#include "Structs/AttributeData.h"
#include "GUI/BitmapText.h"
#include "World/GameObject.h"

class TooltipWindowComponent;

enum class HealthBarStyle {
	MainCharacter,
	Enemy,
	Boss
};

// The health bar, as displayed in a level. It takes its data from an attribute struct
class HealthBar final : public GameObject {
public:
	HealthBar(const AttributeData* attributes, HealthBarStyle style);
	~HealthBar();

	void render(sf::RenderTarget& target) override;
	void update(const sf::Time& frameTime) override;
	void setPosition(const sf::Vector2f& pos) override;

	const AttributeData* getAttributes() const;
	void setAttributes(const AttributeData* attributes);
	void setName(const std::string& name);
	void setVisible(bool visible);

	GameObjectType getConfiguredType() const override { return _Interface; }

private:
	const AttributeData* m_attributes;
	TooltipWindowComponent* m_tooltipComponent;

	int m_currentHP;
	int m_maxOverlayHP;
	int m_overlayHP;

	sf::Time m_highlightTime;
	sf::Time m_waitTime;
	sf::Time m_shrinkTime;

	bool m_isVisible = true;

	sf::RectangleShape m_border;
	sf::RectangleShape m_bar;
	sf::RectangleShape m_hitOverlay;

	BitmapText m_name;

	float m_borderOffsetX;
	float m_borderOffsetY;

	const sf::Texture* m_borderTexture;
	const sf::Texture* m_barTexture;
	const sf::Texture* m_hitOverlayTexture;
	const sf::Texture* m_hitOverlayHighlightTexture;

	const float BAR_HEIGHT = 15.f;

	const float HIGHLIGHT_TIME = 0.05f;
	const float WAIT_TIME = 0.8f;
	const float SHRINK_TIME = 1.f;
};