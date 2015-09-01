#pragma once

#include <thread> 

#include "global.h"
#include "ResourceManager.h"
#include "Screens/MapScreen.h"
#include "Screens/LevelScreen.h"

#include "Enums/MapID.h"
#include "Enums/LevelID.h"

class LoadingScreen : public Screen
{
public:
	LoadingScreen(CharacterCore* core);

	Screen* update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget &renderTarget) override;

	virtual void execOnEnter(const Screen *previousScreen) override;
	virtual void execOnExit(const Screen *nextScreen) override;

	void setLoaded();

private:
	bool m_isLoaded = false;
	BitmapText* m_title = nullptr;
	std::thread m_thread;

	LevelScreen* m_levelToLoad = nullptr;
	MapScreen* m_mapToLoad = nullptr;

	// fancy particles
	std::unique_ptr<particles::TextureParticleSystem> m_ps;
	particles::PointPositionGenerator* m_posGen;
	particles::ColorGenerator* m_colorGen;
	float m_phi = M_PI;
	const float VELOCITY = 300.f;
	const float RADIUS = 200.f;
	const sf::Vector2f CENTER = sf::Vector2f(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f + 100.f);
	sf::Vector2f m_pos;
};