#pragma once

#include "global.h"
#include "ResourceManager.h"

class BackgroundLayer
{
public:
	BackgroundLayer();
	~BackgroundLayer();

	void load(std::string& filename, float distance);
	void render(sf::RenderTarget &target, sf::RenderStates states) const;
	void dispose();
	
	float getDistance() const;

private:
	// distance to camera. -1 means infinity.
	float m_distance;
	std::string m_fileName;
	sf::Sprite m_sprite;
};