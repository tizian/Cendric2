#pragma once

#include "global.h"
#include "ResourceManager.h"
#include "Screen.h"
#include "MenuScreen.h"
#include "Misc/FireBasket.h"

// TODO: Remove test includes and everything that belongs to it.
#include "GUI/SpellSlot.h"
#include "Structs/SpellBean.h"
#include "Particles/ParticleSystem.h"

class SplashScreen : public Screen
{
public:
	SplashScreen();

	Screen* update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) override; 

	void execOnEnter(const Screen *previousScreen) override;
	void execOnExit(const Screen *nextScreen) override;

private:	
	sf::Sprite m_screenSprite;
};