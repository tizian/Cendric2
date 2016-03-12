#pragma once

#include "Particles/ParticleData.h"

#include <SFML/Graphics.hpp>

namespace particles
{
	/* Abstract base class for all generators */
	class ParticleGenerator
	{
	public:
		ParticleGenerator() {}
		virtual ~ParticleGenerator() {}

		virtual void generate(ParticleData *data, int startId, int endId) = 0;
	};


	class PointPositionGenerator : public ParticleGenerator
	{
	public:
		PointPositionGenerator() {}
		~PointPositionGenerator() {}

		void generate(ParticleData *data, int startId, int endId);

	public:
		sf::Vector2f center{ 0.0f, 0.0f };
	};


	class BoxPositionGenerator : public ParticleGenerator
	{
	public:
		BoxPositionGenerator() {}
		~BoxPositionGenerator() {}

		void generate(ParticleData *data, int startId, int endId);

	public:
		sf::Vector2f center{ 0.0f, 0.0f };
	    sf::Vector2f size{ 0.0f, 0.0f };
	};


	class CirclePositionGenerator : public ParticleGenerator
	{
	public:
		CirclePositionGenerator() {}
		~CirclePositionGenerator() {}

		void generate(ParticleData *data, int startId, int endId);

	public:
		sf::Vector2f center{ 0.0f, 0.0f };
		sf::Vector2f radius{ 0.0f, 0.0f };
	};


	class DiskPositionGenerator : public ParticleGenerator
	{
	public:
		DiskPositionGenerator() {}
		~DiskPositionGenerator() {}

		void generate(ParticleData *data, int startId, int endId);

	public:
		sf::Vector2f center{ 0.0f, 0.0f };
		float radius{ 0.0f };
	};


	class SizeGenerator : public ParticleGenerator
	{
	public:
		SizeGenerator() {}
		~SizeGenerator() {}

		void generate(ParticleData *data, int startId, int endId);

	public:
		float minStartSize{ 1.0f };
		float maxStartSize{ 1.0f };
		float minEndSize{ 1.0f };
		float maxEndSize{ 1.0f };
	};


	class ConstantSizeGenerator : public ParticleGenerator
	{
	public:
		ConstantSizeGenerator() {}
		~ConstantSizeGenerator() {}

		void generate(ParticleData *data, int startId, int endId);

	public:
		float size{ 1.0f };
	};


	class ColorGenerator : public ParticleGenerator
	{
	public:
		ColorGenerator() {}
		~ColorGenerator() {}

		void generate(ParticleData *data, int startId, int endId);

	public:
		sf::Color minStartCol{ sf::Color::Black };
		sf::Color maxStartCol{ sf::Color::Black };
		sf::Color minEndCol{ sf::Color::Black };
		sf::Color maxEndCol{ sf::Color::Black };
	};


	class ConstantColorGenerator : public ParticleGenerator
	{
	public:
		ConstantColorGenerator() {}
		~ConstantColorGenerator() {}

		void generate(ParticleData *data, int startId, int endId);

	public:
		sf::Color color{ sf::Color::Black };
	};


	class VelocityGenerator : public ParticleGenerator
	{
	public:
		VelocityGenerator() {}
		~VelocityGenerator() {}

		void generate(ParticleData *data, int startId, int endId);

	public:
		sf::Vector2f minStartVel{ 0.0f, 0.0f };
		sf::Vector2f maxStartVel{ 0.0f, 0.0f };
	};


	class AngledVelocityGenerator : public ParticleGenerator
	{
	public:
		AngledVelocityGenerator() {}
		~AngledVelocityGenerator() {}

		void generate(ParticleData *data, int startId, int endId);

	public:
		float minAngle{ 0.0f };
		float maxAngle{ 0.0f };
		float minStartVel{ 0.0f };
		float maxStartVel{ 0.0f };
	};


	class TimeGenerator : public ParticleGenerator
	{
	public:
		TimeGenerator() {}
		~TimeGenerator() {}

		void generate(ParticleData *data, int startId, int endId);

	public:
		float minTime{ 0.0f };
		float maxTime{ 0.0f };
	};

	class TexCoordsGenerator : public ParticleGenerator
	{
	public:
		TexCoordsGenerator() {}
		~TexCoordsGenerator() {}

		void generate(ParticleData *data, int startId, int endId);

	public:
		sf::IntRect texCoords{ 0, 0, 1, 1 };
	};


	class TexCoordsRandomGenerator : public ParticleGenerator
	{
	public:
		TexCoordsRandomGenerator() {}
		~TexCoordsRandomGenerator() {}

		void generate(ParticleData *data, int startId, int endId);

	public:
		std::vector<sf::IntRect> texCoords;
	};
}
