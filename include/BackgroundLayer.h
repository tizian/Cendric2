#pragma once

#include "global.h"
#include "ResourceManager.h"

class BackgroundLayer
{
public:
	BackgroundLayer();
	~BackgroundLayer();

	void load(std::string& filename, float distance);
	void dispose();
	void render(sf::RenderTarget &target, sf::RenderStates states);
	float getDistance();

private:
	// distance to camera. -1 means infinity.
	float m_distance;
	std::string m_fileName;
	sf::Sprite m_sprite;
};