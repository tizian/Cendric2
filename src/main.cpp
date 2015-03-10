#include "stdafx.h"

int main(int argc, char* argv[])
{
  sf::RenderWindow window(sf::VideoMode(1024, 512), "Cendric 2 :D"); 

  Level level;
  level.loadFromFile("res/level/testlevel.dric");

  while (window.isOpen()) {

    sf::Event Event;

    while (window.pollEvent(Event)) 
	{
      if (Event.type == sf::Event::Closed)
		  window.close();
    }

	window.clear();
	window.draw(level.getTilemap());
	window.display();
  }

  system("PAUSE");
  return 0;
}