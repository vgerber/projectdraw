//#define GLEW_STATIC
#include <GL/glew.h>

#include <Core/common.h>
#include <Core/Scene/Scenes/texscene.h>

#include <stdio.h>



#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#ifdef _WIN32
    const char * image = "C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/examples/texture_demo/assets/dennis.png";
#endif
#ifdef linux
    const char * image = "/home/vincent/Development/UNI/cv1_class/assets/seehofer_traurig.png";
#endif

int main() {
    sf::ContextSettings settings;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    sf::Window window(sf::VideoMode(800, 900), "My Window", sf::Style::Default, settings);
    initCore();
    printf("OpenGL version (%s)\n", glGetString(GL_VERSION));
	
    TextureScene scene(800, 900);
    
	Texture testTex(image);
	scene.addTexture(testTex);

    while(window.isOpen()) {

        sf::Event e;
        while(window.pollEvent(e)) {
            if(e.type == sf::Event::Closed) {
                window.close();
            }
            if(e.type == sf::Event::Resized) {
                sf::Vector2u size = window.getSize();
                scene.resize(size.x, size.y);
            }
        }
		scene.draw(1.0f);
		window.display();        
    }
	
    return 0;    
}
