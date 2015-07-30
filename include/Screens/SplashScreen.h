#pragma once

#include "global.h"
#include "ResourceManager.h"
#include "Screen.h"
#include "MenuScreen.h"
#include "Misc/FireBasket.h"

#include "GUI/SpellSlot.h"
#include "Structs/SpellBean.h"

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