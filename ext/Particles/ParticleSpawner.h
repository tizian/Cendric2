#pragma once

#include <SFML/Graphics.hpp>

namespace particles {

class ParticleData;

/* Abstract base class for all generators */
class ParticleSpawner {
public:
	ParticleSpawner() {}
	virtual ~ParticleSpawner() {}

	virtual void spawn(ParticleData *data, int startId, int endId) = 0;

public:
	sf::Vector2f center{ 0.0f, 0.0f };
};


class PointSpawner : public ParticleSpawner {
public:
	PointSpawner() {}
	~PointSpawner() {}

	void spawn(ParticleData *data, int startId, int endId);
};

class BoxSpawner : public ParticleSpawner {
public:
	BoxSpawner() {}
	~BoxSpawner() {}

	void spawn(ParticleData *data, int startId, int endId);

public:
    sf::Vector2f size{ 0.0f, 0.0f };
};

class CircleSpawner : public ParticleSpawner {
public:
	CircleSpawner() {}
	~CircleSpawner() {}

	void spawn(ParticleData *data, int startId, int endId);

public:
	sf::Vector2f radius{ 0.0f, 0.0f };
};

class DiskSpawner : public ParticleSpawner {
public:
	DiskSpawner() {}
	~DiskSpawner() {}

	void spawn(ParticleData *data, int startId, int endId);

public:
	float radius{ 0.0f };
};

}