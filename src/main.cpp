#include <iostream>
#include <SFML/Graphics.hpp>

#include "TileMap.h"

using namespace std;

int main(int argc, char* argv[])
{
  sf::RenderWindow window(sf::VideoMode(1024, 512), "Cendric 2 :D");
  
  const int level[] =
  {
	  0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	  0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
	  1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
	  0, 1, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
	  0, 1, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 0, 0,
	  0, 0, 1, 0, 3, 0, 2, 2, 0, 0, 1, 1, 1, 1, 2, 0,
	  2, 0, 1, 0, 3, 0, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1,
	  0, 0, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1,
  };

  TileMap map;
  if (!map.load("res/tilemap_test.png", sf::Vector2i(64, 64), level, 16, 8)) {
	  cout << "Problem with texture loading" << endl;
	  return -1;
  }
	  

  while (window.isOpen()) {

    sf::Event Event;

    while (window.pollEvent(Event)) {
      if (Event.type == sf::Event::Closed)
		  window.close();
    }

	window.clear();
	window.draw(map);
	window.display();
  }

  return 0;
}