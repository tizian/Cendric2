#pragma once

#include "global.h"
#include "Screen.h"
#include "ErrorScreen.h"

class ScreenManager
{
public:
	ScreenManager();
	ScreenManager(Screen *initialScreen);
	~ScreenManager();

	void update(sf::Time frameTime);
	void render(sf::RenderTarget &renderTarget);
	void setErrorScreen();

	Screen *getCurrentScreen();

private:
	Screen *m_currentScreen = nullptr;
	bool m_isErrorScreen;
};