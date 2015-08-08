#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

namespace particles
{
    class ParticleData
    {
    public:
        explicit ParticleData(size_t maxCount) { generate(maxCount); }
        ~ParticleData() {}

        ParticleData(const ParticleData &) = delete;
        ParticleData &operator=(const ParticleData &) = delete;

        void generate(size_t maxSize);
        void kill(size_t id);
        void wake(size_t id);
        void swapData(size_t a, size_t b);

    public:
        std::unique_ptr<sf::Vector2f[]> pos;
		std::unique_ptr<sf::Vector3f[]> size;		// x: current size,		y: start size,		z: end size
        std::unique_ptr<sf::Color[]> col;
        std::unique_ptr<sf::Color[]> startCol;
        std::unique_ptr<sf::Color[]> endCol;
        std::unique_ptr<sf::Vector2f[]> vel;
        std::unique_ptr<sf::Vector2f[]> acc;
        std::unique_ptr<sf::Vector3f[]> time;       // x: remaining time to live,   y: time to live,    z: interpolation value in [0, 1] of lifetime
        std::unique_ptr<bool[]>  alive;

        size_t count{ 0 };
        size_t countAlive{ 0 };
    };
}
