#include "stdafx.h"

void SplashScreen::show(sf::RenderWindow & renderWindow)
{
	sf::Texture screen;
	if (!screen.loadFromFile("res/screens/screen_splash.png"))
	{
		printf("SplashScreen: Error at opening file res/screens/screen_splash.png \n");
		return;
	}

	sf::Sprite sprite(screen);

	renderWindow.draw(sprite);
	renderWindow.display();

	sf::Event event;

	while (true)
	{
		while (renderWindow.pollEvent(event))
		{
			if (event.type == sf::Event::EventType::KeyPressed
				|| event.type == sf::Event::EventType::MouseButtonPressed
				|| event.type == sf::Event::EventType::Closed)
			{
				return;
			}
		}
	}
}