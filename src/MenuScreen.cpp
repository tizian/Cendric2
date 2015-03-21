#include "stdafx.h"

MenuScreen::MenuResult MenuScreen::show(sf::RenderWindow& window)
{
	//Load menu image from file
	sf::Texture screen;
	if (!screen.loadFromFile("res/screens/screen_menu.png"))
	{
		printf("SplashScreen: Error at opening file res/screens/screen_menu.png \n");
		return MenuResult::Nothing;
	}

	sf::Sprite sprite(screen);

	//Setup clickable regions

	//Play menu item coordinates
	MenuItem playButton;
	playButton.rect.left = 600;
	playButton.rect.top = 300;
	playButton.rect.width = 400;
	playButton.rect.height = 150;
	playButton.action = MenuResult::Play;

	//Exit menu item coordinates
	MenuItem exitButton;
	exitButton.rect.left = 600;
	exitButton.rect.top = 470;
	exitButton.rect.width = 400;
	exitButton.rect.height = 150;
	exitButton.action = MenuResult::Exit;

	m_menuItems.push_back(playButton);
	m_menuItems.push_back(exitButton);

	window.clear();
	window.draw(sprite);
	window.display();

	return getMenuResponse(window);
}

MenuScreen::MenuResult MenuScreen::handleClick(sf::Vector2f pos)
{
	std::list<MenuItem>::iterator it;
	printf("MenuScreen: Mouse click at %f,%f \n", pos.x, pos.y);

	for (it = m_menuItems.begin(); it != m_menuItems.end(); it++)
	{
		sf::Rect<float> menuItemRect = (*it).rect;
		if (menuItemRect.contains(pos))
		{
			return (*it).action;
		}
	}

	return MenuResult::Nothing;
}

MenuScreen::MenuResult  MenuScreen::getMenuResponse(sf::RenderWindow& window)
{
	sf::Event event;

	while (true)
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::MouseButtonPressed)
			{
				// get the current mouse position in the window
				sf::Vector2i pixelPos = sf::Mouse::getPosition(window);

				// convert it to world coordinates
				sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
				return handleClick(worldPos);
			}
			if (event.type == sf::Event::Closed)
			{
				return MenuResult::Exit;
			}
		}
	}
}