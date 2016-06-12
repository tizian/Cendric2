#pragma once

#include <SFML/Graphics.hpp>

namespace particles {

class ParticleData;

/* Abstract base class for all particle updators */
class ParticleUpdater {
public:
	ParticleUpdater() {}
	virtual ~ParticleUpdater() {}

	virtual void update(ParticleData *data, float dt) = 0;
};


class EulerUpdater : public ParticleUpdater {
public:
	EulerUpdater() {}
	~EulerUpdater() {}

	void update(ParticleData *data, float dt);

public:
	sf::Vector2f globalAcceleration{ 0.0f, 0.0f };
};


class HorizontalCollisionUpdater : public ParticleUpdater {
public:
	HorizontalCollisionUpdater() {}
	~HorizontalCollisionUpdater() {}

	void update(ParticleData *data, float dt);

public:
	float pos{ 0.0f };
	float bounceFactor{ 0.5f };
};


class VerticalCollisionUpdater : public ParticleUpdater {
public:
	VerticalCollisionUpdater() {}
	~VerticalCollisionUpdater() {}

	void update(ParticleData *data, float dt);

public:
	float pos{ 0.0f };
	float bounceFactor{ 0.5f };
};


class AttractorUpdater : public ParticleUpdater {
public:
	AttractorUpdater() {}
	~AttractorUpdater() {}

	void update(ParticleData *data, float dt);

	size_t numAttractors() const { return m_attractors.size(); }
	void add(const sf::Vector3f &attr) { m_attractors.push_back(attr); }
	sf::Vector3f &get(int id) { return m_attractors[id]; }

protected:
	std::vector<sf::Vector3f> m_attractors;	// .xy is position, .z is force
};


class SizeUpdater : public ParticleUpdater {
public:
	SizeUpdater() {}
	~SizeUpdater() {}

	void update(ParticleData *data, float dt);
};


class RotationUpdater : public ParticleUpdater {
public:
	RotationUpdater() {}
	~RotationUpdater() {}

	void update(ParticleData *data, float dt);
};


class ColorUpdater : public ParticleUpdater {
public:
	ColorUpdater() {}
	~ColorUpdater() {}

	void update(ParticleData *data, float dt);
};


class TimeUpdater : public ParticleUpdater {
public:
	TimeUpdater() {}
	~TimeUpdater() {}

	void update(ParticleData *data, float dt);
};


class AnimationUpdater : public ParticleUpdater {
public:
	AnimationUpdater() {}
	~AnimationUpdater() {}

	void update(ParticleData *data, float dt);

public:
	std::vector<sf::IntRect> frames;
	float frameTime;
	bool looped{ false };
};

}