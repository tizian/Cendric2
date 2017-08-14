#pragma once

#include "global.h"
#include "Animation.h"

////////////////////////////////////////////////////////////
// This class was altered from the original source
// by Ironbell
// For the original source, see notice below
////////////////////////////////////////////////////////////
//
// Copyright (C) 2014 Maximilian Wagenbach (aka. Foaly) (foaly.f@web.de)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
// you must not claim that you wrote the original software.
// If you use this software in a product, an acknowledgment
// in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
// and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

class AnimatedSprite : public virtual sf::Drawable, public virtual sf::Transformable {
public:
	explicit AnimatedSprite(sf::Time frameTime = sf::seconds(0.2f), bool paused = false, bool looped = true);

	void update(sf::Time deltaTime);
	void play(Animation* animation);
	void play();
	void pause();
	void stop();

	void setAnimation(const Animation* animation);
	void setFrameTime(sf::Time time);
	void setLooped(bool looped);
	void setFlippedX(bool flipped);
	void setFlippedY(bool flipped);
	void setColor(const sf::Color& color);
	void setFrame(std::size_t newFrame, bool resetTime = true);
	void setRandomStartingFrame();
	// resets color, rotation, scale and flipped property
	void reset();

	const Animation* getAnimation() const;
	sf::FloatRect getLocalBounds() const;
	sf::FloatRect getGlobalBounds() const;
	sf::Time getFrameTime() const;
	std::size_t getCurrentFrame() const;
	bool isLooped() const;
	bool isPlaying() const;
	bool isFlippedX() const;
	bool isFlippedY() const;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	const Animation* m_animation = nullptr;
	sf::Time m_frameTime;
	sf::Time m_currentTime;
	std::size_t m_currentFrame;
	bool m_isPaused;
	bool m_isLooped;
	bool m_isFlippedX = false;
	bool m_isFlippedY = false;
	const sf::Texture* m_texture;
	sf::Vertex m_vertices[4];
};