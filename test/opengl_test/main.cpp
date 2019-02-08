#include <stdio.h>

#include <Core/common.h>

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include "Shaders/Experimental/OpenglTest/SMAA/AreaTex.h"
#include "Shaders/Experimental/OpenglTest/SMAA/SearchTex.h"

const int WIDTH = 1800;
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

	Shader shaderSMAAEdgeLuma = ResourceManager::loadShader(ShaderName::Experimental::OpenglTest::SMAA::EdgeLuma);
	Shader shaderSMAABlendingWeight = ResourceManager::loadShader(ShaderName::Experimental::OpenglTest::SMAA::BlendingWeight);
	Shader shaderSMAABlending = ResourceManager::loadShader(ShaderName::Experimental::OpenglTest::SMAA::Blending);

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

	GLenum modes[] = { GL_COLOR_ATTACHMENT0 };

	unsigned int smaaFBO;
	glGenFramebuffers(1, &smaaFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, smaaFBO);
	// create a color attachment texture
	unsigned int smaaTexture;
	glGenTextures(1, &smaaTexture);
	glBindTexture(GL_TEXTURE_2D, smaaTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, smaaTexture, 0);
	glDrawBuffers(1, modes);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

    unsigned int smaaEdgeTexture;
	glGenTextures(1, &smaaEdgeTexture);
	glBindTexture(GL_TEXTURE_2D, smaaEdgeTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    unsigned int smaaBlendTexture;
	glGenTextures(1, &smaaBlendTexture);
	glBindTexture(GL_TEXTURE_2D, smaaBlendTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	unsigned int smaaAreaTexture;
	glGenTextures(1, &smaaAreaTexture);
	glBindTexture(GL_TEXTURE_2D, smaaAreaTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG8, AREATEX_WIDTH, AREATEX_HEIGHT, 0, GL_RG, GL_UNSIGNED_BYTE, areaTexBytes);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	unsigned int smaaSearchTexture;
	glGenTextures(1, &smaaSearchTexture);
	glBindTexture(GL_TEXTURE_2D, smaaSearchTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, SEARCHTEX_WIDTH, SEARCHTEX_HEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, searchTexBytes);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
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

		//smaa
		{
			glBindFramebuffer(GL_FRAMEBUFFER, smaaFBO);
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glDisable(GL_BLEND);
			
			//smaa edge detection
			{
				glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
				glClear(GL_COLOR_BUFFER_BIT);
				shaderSMAAEdgeLuma.use();
				//glUniform2f(glGetUniformLocation(shaderSMAAEdgeLuma.getId(), "screenSize"), 1.0f / WIDTH, 1.0f / HEIGHT);
				glUniform1i(glGetUniformLocation(shaderSMAAEdgeLuma.getId(), "albedoTexture"), 0);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, offlineTexture);
				glBindVertexArray(screenRectVAO);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				glCopyTextureSubImage2D(smaaEdgeTexture, 0, 0, 0, 0, 0, WIDTH, HEIGHT);
			}

			//smaa weight calculation
			{
				glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
				glClear(GL_COLOR_BUFFER_BIT);
				shaderSMAABlendingWeight.use();
				glUniform1i(glGetUniformLocation(shaderSMAABlendingWeight.getId(), "edgeTexture"), 0);
				glUniform1i(glGetUniformLocation(shaderSMAABlendingWeight.getId(), "areaTexture"), 1);
				glUniform1i(glGetUniformLocation(shaderSMAABlendingWeight.getId(), "searchTexture"), 2);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, smaaEdgeTexture);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, smaaAreaTexture);
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, smaaSearchTexture);
				glBindVertexArray(screenRectVAO);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				glCopyTextureSubImage2D(smaaBlendTexture, 0, 0, 0, 0, 0, WIDTH, HEIGHT);
			}
			//smaa blending
			{	
				glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
				glClear(GL_COLOR_BUFFER_BIT);
				shaderSMAABlending.use();
				glUniform1i(glGetUniformLocation(shaderSMAABlending.getId(), "albedoTexture"), 0);
				glUniform1i(glGetUniformLocation(shaderSMAABlending.getId(), "blendTexture"), 1);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, offlineTexture);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, smaaBlendTexture);
				glBindVertexArray(screenRectVAO);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}



			glBindFramebuffer(GL_READ_FRAMEBUFFER, smaaFBO);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			glBlitFramebuffer(0, 0, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);
		}


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
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			shaderTexture.use();
			glUniform1i(glGetUniformLocation(shaderTexture.getId(), "tex"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, offlineTexture);
			glBindVertexArray(screenRectVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}		
		

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
