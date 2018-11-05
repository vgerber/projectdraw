//#define GLEW_STATIC
#include <GL/glew.h>

#include <Core/common.h>
#include <Core/Scene/Texture/texscene.h>
#include <Recorder/glrecorder.h>

#include <stdio.h>

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#define WIDTH  800
#define HEIGHT  600

int main() {

	sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "My Window");
	initCore();

	//startRecording("tmp.mpg", AV_CODEC_ID_MPEG1VIDEO, 25);

	GLRecorder recorder("gl_tmp.mpg", WIDTH, HEIGHT);

	TextureScene scene(WIDTH, HEIGHT);

	Texture testTex("C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/examples/texture_demo/assets/dennis.png");
	scene.setTexture(&testTex);

	while (window.isOpen()) {

		sf::Event e;
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window.close();
			}
		}
		scene.draw(1.0f);
		window.display();

		//readGLPixels(&rgb, &pixels);
		//encodeFrame(rgb);
		//nframes++;
		recorder.captureFrame();
	}
	//endRecording();
	recorder.finish();

	return 0;
}