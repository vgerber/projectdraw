// #define GLEW_STATIC
#include <GL/glew.h>

#include <Core/Scene/Scenes/imgscene.h>
#include <Core/common.h>
#include <Recorder/glrecorder.h>

#include <stdio.h>

#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

#define WIDTH 800
#define HEIGHT 600

int main() {

  sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "My Window");
  initCore();

  // startRecording("tmp.mpg", AV_CODEC_ID_MPEG1VIDEO, 25);

  GLRecorder recorder("gl_tmp.mpg", WIDTH, HEIGHT);

  ImageScene scene(WIDTH, HEIGHT);

  Texture testTex("/home/vincent/Development/Cpp/projectdraw/examples/"
                  "texture_demo/assets/Dennis.png");
  scene.addTexture(testTex);

  while (window.isOpen()) {

    sf::Event e;
    while (window.pollEvent(e)) {
      if (e.type == sf::Event::Closed) {
        window.close();
      }
    }
    scene.draw(1.0f);
    window.display();

    // readGLPixels(&rgb, &pixels);
    // encodeFrame(rgb);
    // nframes++;
    recorder.captureFrame();
  }
  // endRecording();
  recorder.finish();

  return 0;
}