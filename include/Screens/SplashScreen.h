#pragma once

#include "global.h"
#include "ResourceManager.h"
#include "Screen.h"
#include "MenuScreen.h"
#include "Particles/ParticleSystem.h"

class SplashScreen : public Screen {
public:
	SplashScreen();
	~SplashScreen();

	void execUpdate(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) override;

	void execOnEnter(const Screen* previousScreen) override;
	void execOnExit(const Screen* nextScreen) override;

public:
	static void loadFireParticles(particles::TextureParticleSystem* ps, const sf::Vector2f& center);

private:
	sf::Sprite m_screenSpriteBackground;
	sf::Sprite m_screenSpriteForeground;
	BitmapText m_versionText;
	
	particles::TextureParticleSystem* m_ps_left = nullptr;
	particles::TextureParticleSystem* m_ps_right = nullptr;
};