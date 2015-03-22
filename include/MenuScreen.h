#include "stdafx.h"

class MenuScreen
{

public:
	enum class MenuResult 
	{ 
		Nothing, 
		Exit, 
		Play 
	};

	struct MenuItem
	{
	public:
		sf::FloatRect rect;
		MenuResult action;
	};

	MenuResult show(sf::RenderWindow& window);

private:
	MenuResult getMenuResponse(sf::RenderWindow& window);
	MenuResult handleClick(sf::Vector2f pos);
	std::list<MenuItem> m_menuItems;
};