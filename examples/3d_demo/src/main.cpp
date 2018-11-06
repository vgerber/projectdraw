#include <Core/common.h>

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#define WIDTH 800
#define HEIGHT 800

int main() {

    sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "3D Demo");
    initCore();

    Scene scene(WIDTH, HEIGHT);

    while(window.isOpen()) {
        while(window.isOpen()) {

        sf::Event e;
        while(window.pollEvent(e)) {
            if(e.type == sf::Event::Closed) {
                window.close();
            }
        }
		scene.tick(1.0f);
		window.display();        
    }
    }

}