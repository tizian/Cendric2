#include "stdafx.h"

// an abstract game object with a hitbox, a statetime, an animation, a position
class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void load(std::string filename);
	virtual void draw(sf::RenderWindow & window);

	virtual void setPosition(float x, float y);

private:
	sf::Sprite  m_sprite;
	sf::Texture m_texture;
	std::string m_filename;
	bool m_isLoaded;
};