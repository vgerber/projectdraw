#include <stdio.h>

#include <Core/common.h>

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include "Core/Texture/Filter/Antialias/smaa.h"

const int WIDTH = 1000;
const int HEIGHT = 1000;

const int samples = 4;

int main() {


    sf::ContextSettings ctxSetting;
    ctxSetting.majorVersion = 3;
    ctxSetting.minorVersion = 3;
    ctxSetting.depthBits = 24;
    ctxSetting.stencilBits = 8;
    ctxSetting.antialiasingLevel = 0;

    sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "Experimental", sf::Style::Default , ctxSetting);
    initCore();
    glViewport(0, 0, WIDTH, HEIGHT);

    // Setup OpenGL options
    //glEnable(GL_MULTISAMPLE); // Enabled by default on some drivers, but not all so always enable to make sure
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

    // Setup and compile our shaders
    Shader shader = ResourceManager::loadShader(ShaderName::Experimental::OpenglTest::Basic);
	Shader shaderScreen = ResourceManager::loadShader(ShaderName::Experimental::OpenglTest::BasicPostProcessing);
	Shader shaderTexture = ResourceManager::loadShader(ShaderName::Experimental::OpenglTest::Texture);

    #pragma region "object_initialization"
    // Set the object data (buffers, vertex attributes)
    GLfloat cubeVertices[] = {
        // Positions       
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
         0.5f,  -0.4f, 0.0f, 1.0f, 0.0f,
         0.0f,  0.8f, 0.0f, 1.0f, 1.0f
    };

	GLfloat screenRectangle[] = {
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

    GLfloat textureRectangle[] = {
		-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,

		-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
		 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,  1.0f, 1.0f
	};
    // Setup cube VAO
    GLuint cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
    glBindVertexArray(0);

    GLuint textureVAO, textureVBO;
    glGenVertexArrays(1, &textureVAO);
    glGenBuffers(1, &textureVBO);
    glBindVertexArray(textureVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textureRectangle), &textureRectangle, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
    glBindVertexArray(0);

	GLuint screenRectVAO, screenRectVBO;
	glGenVertexArrays(1, &screenRectVAO);
	glGenBuffers(1, &screenRectVBO);
	glBindVertexArray(screenRectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, screenRectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenRectangle), &screenRectangle, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2* sizeof(float)));
	glBindVertexArray(0);
    #pragma endregion
	

	unsigned int offlineFBO;
	glGenFramebuffers(1, &offlineFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, offlineFBO);
	// create a color attachment texture
	unsigned int offlineTexture;
	glGenTextures(1, &offlineTexture);
	glBindTexture(GL_TEXTURE_2D, offlineTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, offlineTexture, 0);

	unsigned int offlineRBO;
	glGenRenderbuffers(1, &offlineRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, offlineRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, offlineRBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


    bool msaa = true;

	SMAA smaaFilter;
	smaaFilter.setup();
	smaaFilter.resize(WIDTH, HEIGHT);

    Font font(ResourceManager::GetPath("/Fonts/VeraMono.ttf").c_str(), 400);
	Text textSceneName(font);
    textSceneName.setText("Experimental 3D");
    

    int textureCounter = 0;
    // Game loop
    while(window.isOpen())
    { 
        GLuint texture = textSceneName.activateChar(textureCounter);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set transformation matrices		
        shader.use();
		// Clear buffers

		glBindFramebuffer(GL_FRAMEBUFFER, offlineFBO);
		glClearColor(0.0f, 1.0f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);


		/*
		{
			shaderScreen.use();
			glUniform1i(glGetUniformLocation(shaderScreen.getId(), "samples"), samples);
			glBindVertexArray(screenRectVAO);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, multisampleTexture);
			glBindTexture(GL_TEXTURE_2D, offlineTexture);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		*/

		if(msaa)
		{
			smaaFilter.clear();
			smaaFilter.apply(offlineTexture);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderTexture.use();
		glUniform1i(glGetUniformLocation(shaderTexture.getId(), "tex"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, offlineTexture);
		glBindVertexArray(screenRectVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

        // Swap the buffers
        window.display();
        sf::Event e;
        while(window.pollEvent(e)) {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
            if(e.type == sf::Event::KeyPressed) {
                if(e.key.code == sf::Keyboard::Q) {
                    msaa = !msaa;
                    printf("Switch %d\n", msaa);
                    if (msaa) {
                        glDisable(GL_MULTISAMPLE);
                    }
                    else {
                        glEnable(GL_MULTISAMPLE);
                    }
                }
                if(e.key.code == sf::Keyboard::W) {
                    textureCounter++;
                    textureCounter = textureCounter % textSceneName.getText().size();
                }
            }
        }
    }
    return 0;
}
