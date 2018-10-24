#include <stdio.h>
#include <SFML/Window.hpp>

#include <Core/common.h>
#include <Core/Scene/Texture/texscene.h>

int main() {
    
    sf::Window window(sf::VideoMode(800, 600), "My Window");
    initCore();

    TextureScene scene(800, 600);

    while(window.isOpen()) {

        sf::Event e;
        while(window.pollEvent(e)) {
            if(e.type == sf::Event::Closed) {
                window.close();
            }
        }

        scene.draw(1.0f);

        window.display();
    }

    return 0;    
}