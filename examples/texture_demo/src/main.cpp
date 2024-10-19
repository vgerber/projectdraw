// #define GLEW_STATIC
#include <GL/glew.h>

#include <Core/Scene/Scenes/imgscene.h>
#include <Core/common.h>

#include <stdio.h>

#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

const char *image = "/home/vincent/Development/Cpp/projectdraw/examples/"
                    "texture_demo/assets/seehofer_traurig.png";

int main() {
  sf::ContextSettings settings;
  settings.majorVersion = 3;
  settings.minorVersion = 3;
  sf::Window window(sf::VideoMode(800, 900), "My Window", sf::Style::Default,
                    settings);
  initCore();
  printf("OpenGL version (%s)\n", glGetString(GL_VERSION));

  ImageScene scene(800, 900);

  Texture testTex(image);
  scene.addTexture(testTex);

  while (window.isOpen()) {

    sf::Event e;
    while (window.pollEvent(e)) {
      if (e.type == sf::Event::Closed) {
        window.close();
      }
      if (e.type == sf::Event::Resized) {
        sf::Vector2u size = window.getSize();
        scene.resize(size.x, size.y);
      }
    }
    scene.draw(1.0f);
    window.display();
  }

  return 0;
}
