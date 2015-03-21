#include "stdafx.h"

void SplashScreen::show(sf::RenderWindow & window)
{
	sf::Texture screen;
	if (!screen.loadFromFile("res/screens/screen_splash.png"))
	{
		printf("SplashScreen: Error at opening file res/screens/screen_splash.png \n");
		return;
	}

	sf::Sprite sprite(screen);

	window.clear();
	window.draw(sprite);
	window.display();

	sf::Event event;

	while (true)
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed
				|| event.type == sf::Event::MouseButtonPressed
				|| event.type == sf::Event::Closed)
			{
				return;
			}
		}
	}
}