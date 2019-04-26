#include <stdio.h>

#include <GL/glew.h>

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include <Core/common.h>
//#include <Core/Scene/Texture/texscene.h>
//include <CudaRender/cudatexture.h>
#include <Recorder/glrecorder.h>

#include "cuda/gol.h"

#define WIDTH 1024
#define HEIGHT 1024

int main() {
    
    sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "CudaDemo");
    initCore();

    TextureScene scene(WIDTH, HEIGHT);

	GLRecorder recorder("hcopy.mpg", WIDTH, HEIGHT, 1, 25, 25);
	
	testprint();

	int dim = 1024;
	
	CudaTexture * cu_texture = new CudaTexture(dim+2, dim+2);
	
	int * world = nullptr;
	int * d_world;
	int * d_tmpWorld;



	cu_texture->map();
	initGOL(&world, &d_world, &d_tmpWorld, dim, cu_texture->getCudaTexture());	
	cu_texture->unmap();

	cudaDeviceSynchronize();

	scene.setTexture(cu_texture);

	int step = 0;

	int spause = 100;
	int sdelay = 0;
	bool sleep_pause = false;
	bool sleep_delay = false;
	bool texture_dirty = true;
	sf::Clock clock;
	sf::Time deltaTime = clock.getElapsedTime();
	sf::Time deltaDelayTime = clock.getElapsedTime();


    while(window.isOpen()) {		
        sf::Event e;
        while(window.pollEvent(e)) {
            if(e.type == sf::Event::Closed) {
                window.close();
            }
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
				sdelay += 10;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
				if (sdelay >= 10) {
					sdelay -= 10;
				}
			}
        }
		scene.draw(1.0f);
		window.display();
		recorder.captureFrame();
		if (!sleep_pause && step % 8 == 0 && texture_dirty) {
			sleep_pause = true;
			texture_dirty = false;
			deltaTime = clock.getElapsedTime();
		}	


		if (!sleep_pause && !sleep_delay) {
			cu_texture->map();
			stepGOL(world, d_world, d_tmpWorld, dim, cu_texture->getCudaTexture());
			cu_texture->unmap();
			step++;
			texture_dirty = true;
			sleep_delay = true;
			deltaDelayTime = clock.getElapsedTime();
		}
		else {
			if (sleep_pause) {
				if ((clock.getElapsedTime() - deltaTime).asMilliseconds() >= spause) {
					sleep_pause = false;
				}
			}
			if (sleep_delay) {
				if ((clock.getElapsedTime() - deltaDelayTime).asMilliseconds() >= sdelay) {
					sleep_delay = false;
				}
			}
		}
		       
    }
	recorder.finish();

    return 0;    
}