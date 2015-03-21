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
		sf::Rect<int> rect;
		MenuResult action;
	};

	MenuResult show(sf::RenderWindow& window);

private:
	MenuResult getMenuResponse(sf::RenderWindow& window);
	MenuResult handleClick(sf::Vector2i& pos);
	std::list<MenuItem> m_menuItems;
};