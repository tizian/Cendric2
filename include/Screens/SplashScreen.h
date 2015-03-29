#include "stdafx.h"

class SplashScreen : public Screen
{
public:
	SplashScreen();

	Screen* update(sf::Time frameTime) override;
	void render(sf::RenderTarget &renderTarget) override; 

	virtual void onEnter(Screen *previousScreen);
	virtual void onExit(Screen *nextScreen);

private:	
	sf::Sprite m_screenSprite;
};