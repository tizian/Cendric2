#include "stdafx.h"

// A game object with animations
class GameObject : public Object
{
public:
	GameObject();
	~GameObject();

	// \brief loads animation from resource manager
	virtual void load(std::string tag);

	void render(sf::RenderTarget &renderTarget) const override;
	void update(sf::Time frameTime) override;

	void setCurrentAnimation(Animation &animation);
	void addAnimation(std::string tag, Animation &animation);
	Animation getAnimation(std::string tag);
	
	AnimatedSprite m_animatedSprite;

private:
	map<std::string,  Animation> m_animations;	
};