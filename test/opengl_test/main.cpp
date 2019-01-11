#include <stdio.h>

#include <Core/common.h>

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

const int WIDTH = 1000;
const int HEIGHT = 1000;


int main() {

    sf::ContextSettings ctxSetting;
    ctxSetting.majorVersion = 3;
    ctxSetting.minorVersion = 3;
    ctxSetting.depthBits = 24;
    ctxSetting.stencilBits = 8;
    ctxSetting.antialiasingLevel = 4;

    sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "Experimental", sf::Style::Default , ctxSetting);
    initCore();
    glViewport(0, 0, WIDTH, HEIGHT);

    // Setup OpenGL options
    glEnable(GL_MULTISAMPLE); // Enabled by default on some drivers, but not all so always enable to make sure
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader shader = ResourceManager::loadShader(ShaderName::Experimental::OpenglTest::Basic);

    #pragma region "object_initialization"
    // Set the object data (buffers, vertex attributes)
    GLfloat cubeVertices[] = {
        // Positions       
        -0.5f, -0.5f, 0.0f, 
         0.5f, -0.2f, 0.0f, 
         0.5f,  0.8f, 0.0f, 
    };
    // Setup cube VAO
    GLuint cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);
    #pragma endregion

    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    const int samples = 4;
    unsigned int multisampleTexture;
    glGenTextures(1, &multisampleTexture);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, WIDTH, HEIGHT, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, multisampleTexture, 0);
    
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    bool msaa = false;
    int maxSamples = 0;
    glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
    printf("Max samples %d\n",maxSamples);

    // Game loop
    while(window.isOpen())
    {    
        
        // Clear buffers
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set transformation matrices		
        shader.use();
        
        if(msaa) {
            glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            glBlitFramebuffer(0, 0, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        }
        else {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // Swap the buffers
        window.display();
        sf::Event e;
        while(window.pollEvent(e)) {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
            if(e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Q) {
                msaa = !msaa;
                printf("Switch %d\n", msaa);
            }
        }
    }
    return 0;
}