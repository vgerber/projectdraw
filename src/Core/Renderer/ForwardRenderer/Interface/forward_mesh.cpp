#include "forward_mesh.h"

ForwardMesh::ForwardMesh(Mesh * mesh) : ForwardDrawable(static_cast<Drawable*>(mesh)) { 
    setup(); 
}

void ForwardMesh::update() {
    ForwardDrawable::update();

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    const Mesh * mesh = static_cast<Mesh*>(getLinkedObject());

    glBufferData(GL_ARRAY_BUFFER, mesh->getVertices().size() * sizeof(Vertex), &mesh->getVertices()[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndices().size() * sizeof(GLuint), &mesh->getIndices()[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Color));
    glBindVertexArray(0);

}

void ForwardMesh::draw() {
    Mesh * mesh = static_cast<Mesh*>(getLinkedObject());
    Shader shader = ResourceManager::loadShader(ShaderName::Renderer::Forward::Basic::Mesh);
    shader.use();
    glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(mesh->getWorldTransform().getMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
    
    glUniform1i(glGetUniformLocation(shader.getId(), "alphaTexture"), 0);
	glUniform1i(glGetUniformLocation(shader.getId(), "diffuseTexture"), 1);
	glUniform1i(glGetUniformLocation(shader.getId(), "specularTexture"), 2);


    bool useAlphaTexture    = mesh->settings.useAlphaTexture   && mesh->getAlphaTextures().size() > 0;
    bool useSpecularTexture = mesh->settings.useSpecualTexture && mesh->getSpecularTextures().size() > 0;
    bool useDiffuseTexture  = mesh->settings.useDiffuseTexture && mesh->getDiffuseTextures().size() > 0;
    glUniform1i(glGetUniformLocation(shader.getId(), "enableAlphaTexture"),    useAlphaTexture);
    glUniform1i(glGetUniformLocation(shader.getId(), "enableDiffuseTexture"),  useDiffuseTexture);
    glUniform1i(glGetUniformLocation(shader.getId(), "enableSpecularTexture"), useSpecularTexture);

    if(useAlphaTexture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mesh->getAlphaTextures()[0]->getGLTexture());
    }
    if(useDiffuseTexture) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, mesh->getDiffuseTextures()[0]->getGLTexture());
    }
    if(useSpecularTexture) {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, mesh->getSpecularTextures()[0]->getGLTexture());
    }

    
    if (mesh->getVertices().size() > 0) {
        // Draw mesh
        glBindVertexArray(this->VAO);
        DrawableInfo settings = mesh->settings;
        if (settings.drawType == DrawType::LINEG) {
            glLineWidth(settings.lineThickness);
            glDrawElements(GL_LINE_STRIP, mesh->getIndices().size(), GL_UNSIGNED_INT, 0);
        }
        else if (settings.drawType == DrawType::POINTG) {
            glPointSize(settings.pointThickness);
            glDrawElements(GL_POINTS, mesh->getIndices().size(), GL_UNSIGNED_INT, 0);
        }
        else {
            glDrawElements(GL_TRIANGLES, mesh->getIndices().size(), GL_UNSIGNED_INT, 0);
        }
    }

    ForwardDrawable::draw();
}

void ForwardMesh::drawDepthInfo() {
    Mesh * mesh = static_cast<Mesh*>(getLinkedObject());

    Shader shader = ResourceManager::loadShader(ShaderName::Renderer::Forward::DepthInfo::Mesh);
    shader.use();
    
    glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(mesh->getWorldTransform().getMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

    if (mesh->getVertices().size() > 0) {
        // Draw mesh
        glBindVertexArray(this->VAO);
        DrawableInfo settings = mesh->settings;
        if (settings.drawType == DrawType::LINEG) {
            glLineWidth(settings.lineThickness);
            glDrawElements(GL_LINES, mesh->getIndices().size(), GL_UNSIGNED_INT, 0);
        }
        else if (settings.drawType == DrawType::POINTG) {
            glPointSize(settings.pointThickness);
            glDrawElements(GL_POINTS, mesh->getIndices().size(), GL_UNSIGNED_INT, 0);
        }
        else {
            glDrawElements(GL_TRIANGLES, mesh->getIndices().size(), GL_UNSIGNED_INT, 0);
        }
    }

    ForwardDrawable::drawDepthInfo();
}

void ForwardMesh::dispose() {
    ForwardDrawable::dispose();

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void ForwardMesh::setup() {
    glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
}