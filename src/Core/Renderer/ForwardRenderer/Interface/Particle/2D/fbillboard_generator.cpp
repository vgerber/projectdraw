#include "fbillboard_generator.h"

ForwardBillboardGenerator2D::ForwardBillboardGenerator2D(BillboardGenerator2D * billboardGenerator) : ForwardDrawable(static_cast<Drawable*>(billboardGenerator)) {
    setup();
}

void ForwardBillboardGenerator2D::update() {
    BillboardGenerator2D * generator = static_cast<BillboardGenerator2D*>(getLinkedObject());
    PointParticles2D particles = generator->getParticles();
    particleCount = particles.position.size();
    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBOpos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * particleCount, &particles.position[0], GL_STREAM_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (GLvoid*)0);
    
    glVertexAttribDivisor(0, 1);

    glBindVertexArray(0);

    ForwardDrawable::update();
}

void ForwardBillboardGenerator2D::draw() {
    Shader shader = ResourceManager::loadShader(ShaderName::Renderer::Forward::Basic::Instancing::Billboard2D);

    BillboardGenerator2D * generator = static_cast<BillboardGenerator2D*>(getLinkedObject());
    shader.use();

    glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "vp"),   1, GL_FALSE, glm::value_ptr(vp));
    glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(model));

    glUniform3f(glGetUniformLocation(shader.getId(), "right"), cameraRightWS.x, cameraRightWS.y, cameraRightWS.z);
    glUniform3f(glGetUniformLocation(shader.getId(), "up"), cameraUpWS.x, cameraUpWS.y, cameraUpWS.z);

    glUniform1i(glGetUniformLocation(shader.getId(), "billboard"), 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, generator->getDiffuseTextures()[0]->getGLTexture());

    glBindVertexArray(VAO);
    glDrawArraysInstanced(GL_POINTS, 0, 4, particleCount);
    glBindVertexArray(0);
    
    ForwardDrawable::draw();
}

void ForwardBillboardGenerator2D::drawDepthInfo() {
    ForwardDrawable::drawDepthInfo();
}

void ForwardBillboardGenerator2D::dispose() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBOpos);
    ForwardDrawable::dispose();
}

void ForwardBillboardGenerator2D::setCamera(const Camera &camera) {
    //to update own mvp matrix setCamera from base class must be called first
    BillboardGenerator2D * generator = static_cast<BillboardGenerator2D*>(getLinkedObject());

    

    view = camera.getViewMatrix();
    vp = camera.getProjectionMatrix() * view;
    model = generator->getWorldTransform().getMatrix();

    glm::mat4 camRotationWS = camera.getWorldTransform().getRotation().getRotationMatrix();
    cameraRightWS = camRotationWS * glm::vec4(camera.getBaseRight(), 1.0);
    cameraUpWS = camRotationWS * glm::vec4(camera.getBaseUp(), 1.0);
    //mvp = proj * view * model;    

    for(auto child : children) {
        static_cast<ForwardSceneObject*>(child)->setCamera(camera);
    }
}

void ForwardBillboardGenerator2D::setup() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBOpos);

}