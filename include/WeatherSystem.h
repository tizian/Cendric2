#pragma once

#include "global.h"

#include "Structs/WeatherData.h"
#include "Particles/ParticleSystem.h"

class WeatherSystem {
public:
	~WeatherSystem();

	void load(const WeatherData* data, bool isLevel);
	void update(const sf::Vector2f& center, const sf::Time& frameTime);
	void render(sf::RenderTarget& renderTarget);

private:
	particles::ParticleSystem* m_ps = nullptr;
	sf::Vector2f* m_center = nullptr;
};