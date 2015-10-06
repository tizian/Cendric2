#pragma once

#include "global.h"
#include "Screen.h"
#include "Screens/ErrorScreen.h"

class ScreenManager {
public:
	ScreenManager(Screen* initialScreen);
	~ScreenManager();

	void update(const sf::Time& frameTime);
	void render(sf::RenderTarget& renderTarget);
	void setErrorScreen();

	Screen* getCurrentScreen() const;

private:
	Screen* m_currentScreen = nullptr;
	bool m_isErrorScreen;
};