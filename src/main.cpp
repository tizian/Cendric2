#include "stdafx.h"

int main(int argc, char* argv[])
{
  sf::RenderWindow window(sf::VideoMode(1600, 960), "Cendric 2 :D"); 

  Level level;
  level.loadFromFile("res/level/testlevel.dric");

  InputController controller;
  controller.init();

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

	controller.update();
	// debug output for testing
	if (controller.getKeyActiveMap().at(Key::LEFT)) 
	{
		printf("main: Left Key pressed \n");
	}
	if (controller.getKeyActiveMap().at(Key::RIGHT))
	{
		printf("main: Right Key pressed \n");
	}
	if (controller.getKeyActiveMap().at(Key::JUMP))
	{
		printf("main: Jump Key pressed \n");
	}
  }

  return 0;
}