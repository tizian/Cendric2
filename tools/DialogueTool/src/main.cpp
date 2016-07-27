#include "imgui/imgui.h"
#include "imgui-sfml/imgui-sfml.h"
 
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
 
int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "ImGui test");
    window.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(window);
 
    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
 
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
 
        ImGui::SFML::Update(deltaClock.restart());
 
        ImGui::Begin("Sample window"); // begin window
        ImGui::End(); // end window
 
        window.clear();
        ImGui::Render();
        window.display();
    }
 
    ImGui::SFML::Shutdown();
}