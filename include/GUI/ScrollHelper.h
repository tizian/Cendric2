#pragma once

#include "global.h"

class ScrollBar;

class ScrollHelper {
public:
	ScrollHelper(const sf::FloatRect& boundingBox);
	void render(sf::RenderTarget& target);

public:
	sf::RenderTexture texture;
	float lastOffset;
	float nextOffset;

private:
	sf::Sprite m_sprite;
	const sf::IntRect m_boundingBox;
};