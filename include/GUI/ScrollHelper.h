#pragma once

#include "global.h"

class ScrollBar;

class ScrollHelper final {
public:
	ScrollHelper(const sf::FloatRect& boundingBox);
	void render(sf::RenderTarget& target);
	void setPosition(const sf::Vector2f& position);

public:
	sf::RenderTexture texture;
	float lastOffset;
	float nextOffset;

private:
	sf::Sprite m_sprite;
	sf::FloatRect m_boundingBox;
};