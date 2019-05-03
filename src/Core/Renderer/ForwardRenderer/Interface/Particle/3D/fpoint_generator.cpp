#include "fpoint_generator.h"

ForwardPointGenerator3D::ForwardPointGenerator3D(PointGenerator3D * pointGenerator) : ForwardDrawable(static_cast<Drawable*>(pointGenerator)) {
    setup();
}

void ForwardPointGenerator3D::update() {
    PointGenerator3D * generator = static_cast<PointGenerator3D*>(getLinkedObject());
    PointParticles3D particles = generator->getParticles();
    particleCount = particles.position.size();
    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBOpos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * particleCount, &particles.position[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (GLvoid*)0);

    glBindBuffer(GL_ARRAY_BUFFER, VBOcolor);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * particleCount, &particles.color[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (GLvoid*)0);
    
    glBindVertexArray(0);

    ForwardDrawable::update();
}

void ForwardPointGenerator3D::draw() {
    Shader shader = ResourceManager::loadShader(ShaderName::Renderer::Forward::Basic::Instancing::Point3D);

    PointGenerator3D * generator = static_cast<PointGenerator3D*>(getLinkedObject());

    shader.use();

    glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

    glBindVertexArray(VAO);
    glPointSize(2);
    glDrawArrays(GL_POINTS, 0, particleCount);
    glBindVertexArray(0);

    ForwardDrawable::draw();
}

void ForwardPointGenerator3D::drawDepthInfo() {
    ForwardDrawable::drawDepthInfo();
}

void ForwardPointGenerator3D::dispose() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBOpos);
    glDeleteBuffers(1, &VBOcolor);
    ForwardDrawable::dispose();
}

void ForwardPointGenerator3D::setup() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBOpos);
    glGenBuffers(1, &VBOcolor);
}