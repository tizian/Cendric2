#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

namespace particles
{
    class ParticleData
    {
    public:
        explicit ParticleData(int maxCount);
        ~ParticleData() {}

        ParticleData(const ParticleData &) = delete;
        ParticleData &operator=(const ParticleData &) = delete;

        void kill(int id);
        void wake(int id);
        void swapData(int id1, int id2);

    public:
        std::unique_ptr<sf::Vector2f[]> pos;        // Current position
        std::unique_ptr<sf::Vector2f[]> vel;        // Current velocity
        std::unique_ptr<sf::Vector2f[]> acc;        // Current acceleration
        std::unique_ptr<sf::Vector3f[]> time;       // x: remaining time to live,   y: time to live,    z: interpolation value in [0, 1] of lifetime
        std::unique_ptr<sf::Vector3f[]> size;       // x: current size,     y: start size,      z: end size
        std::unique_ptr<sf::Color[]> col;           // Current color
        std::unique_ptr<sf::Color[]> startCol;      // Start color
        std::unique_ptr<sf::Color[]> endCol;        // End color
        std::unique_ptr<bool[]>  alive;

        int count{ 0 };
        int countAlive{ 0 };
    };
}
