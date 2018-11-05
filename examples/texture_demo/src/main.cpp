//#define GLEW_STATIC
#include <GL/glew.h>

#include <Core/common.h>
#include <Core/Scene/Texture/texscene.h>

#include <stdio.h>



#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>



int main() {
    
    sf::Window window(sf::VideoMode(800, 900), "My Window");
    initCore();

	
    TextureScene scene(800, 900);

	Texture testTex("C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/examples/texture_demo/assets/dennis.png");
	scene.setTexture(testTex);

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
