#include "fpoint_generator.h"

ForwardPointGenerator2D::ForwardPointGenerator2D(PointGenerator2D * pointGenerator) : ForwardDrawable(static_cast<Drawable*>(pointGenerator)) {
    setup();
}

void ForwardPointGenerator2D::update() {
    PointGenerator2D * generator = static_cast<PointGenerator2D*>(getLinkedObject());
    PointParticles2D particles = generator->getParticles();
    particleCount = particles.position.size();
    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBOpos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * particleCount, &particles.position[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (GLvoid*)0);

    glBindBuffer(GL_ARRAY_BUFFER, VBOcolor);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * particleCount, &particles.color[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (GLvoid*)0);
    
    glBindVertexArray(0);

    ForwardDrawable::update();
}

void ForwardPointGenerator2D::draw() {
    Shader shader = ResourceManager::loadShader(ShaderName::Renderer::Forward::Basic::Instancing::Point2D);

    PointGenerator2D * generator = static_cast<PointGenerator2D*>(getLinkedObject());

    shader.use();

    glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

    glBindVertexArray(VAO);
    glPointSize(2);
    //glDrawArraysInstanced(GL_POINTS, 0, particleCount, 1);
    glDrawArrays(GL_POINTS, 0, particleCount);
    glBindVertexArray(0);

    ForwardDrawable::draw();
}

void ForwardPointGenerator2D::drawDepthInfo() {
    ForwardDrawable::drawDepthInfo();
}

void ForwardPointGenerator2D::dispose() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBOpos);
    glDeleteBuffers(1, &VBOcolor);
    ForwardDrawable::dispose();
}

void ForwardPointGenerator2D::setup() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBOpos);
    glGenBuffers(1, &VBOcolor);
}