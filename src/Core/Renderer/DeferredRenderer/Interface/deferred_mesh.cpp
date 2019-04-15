#include "deferred_mesh.h"

DeferredMesh::DeferredMesh(Mesh * mesh) : DeferredDrawable(static_cast<Drawable*>(mesh)) { 
    setup(); 
}

void DeferredMesh::update() {
    DeferredDrawable::update();

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

void DeferredMesh::draw() {
    Mesh * mesh = static_cast<Mesh*>(getLinkedObject());
    DrawableInfo settings = mesh->settings;

    Shader shaderBasic = ResourceManager::loadShader(ShaderName::Renderer::Deferred::Mesh::Basic);

	if (settings.xrayVisible) {
		//glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(getModelMatrix()));
		//glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "mvp"), 1, GL_FALSE, glm::value_ptr(cameraProj * cameraView * getModelMatrix()));
		glUniform1f(glGetUniformLocation(shaderBasic.getId(), "useLight"), settings.xrayUseLight);
		glUniform1i(glGetUniformLocation(shaderBasic.getId(), "enableCustomColor"), settings.xrayCustomColor);
		glm::vec4 color = settings.xrayColor;
		glUniform4f(glGetUniformLocation(shaderBasic.getId(), "customColor"), color.r, color.g, color.b, color.a);

		glEnable(GL_DEPTH_TEST);
		//set all occupied pixels to 1
		glStencilFunc(GL_GREATER, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_REPLACE, GL_KEEP);
		glStencilMask(0xFF);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE);

		drawRaw(shaderBasic);

		//draw to all pixels with stencil equals 1 and reset it to zero
		glStencilFunc(GL_EQUAL, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilMask(0xFF);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		glDisable(GL_DEPTH_TEST);
		drawRaw(shaderBasic);

		glEnable(GL_DEPTH_TEST);
	}


	if (settings.outlineVisible) {
		glUniform1f(glGetUniformLocation(shaderBasic.getId(), "useLight"), 0.0f);
		glUniform1i(glGetUniformLocation(shaderBasic.getId(), "enableCustomColor"), 1);
		glm::vec4 color = settings.outlineColor;
		glUniform4f(glGetUniformLocation(shaderBasic.getId(), "customColor"), color.r, color.g, color.b, color.a);

		//set all occupied pixels to 2
		glStencilFunc(GL_GREATER, 2, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0xFF);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE);

        drawRaw(shaderBasic);


		Size outlineSize;
		Size size = mesh->getSize();
		float thickness = settings.outlineThickness;
		outlineSize.width = (size.width + thickness) / size.width;
		outlineSize.height = (size.height + thickness) / size.height;
		outlineSize.depth = (size.depth + thickness) / size.depth;

		glm::vec3 oldScale = mesh->getScale();
		glm::vec3 oldPosition = mesh->getPosition();

		mesh->scale(outlineSize.width, outlineSize.height, outlineSize.depth);

		
		//setPosition(getPosition() + glm::vec3(-0.5f * thickness));

		glStencilFunc(GL_EQUAL, 0, 0xFF);
		glStencilOp(GL_ZERO, GL_KEEP, GL_REPLACE);
		glStencilMask(0xFF);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_TRUE);

		drawRaw(shaderBasic);

		mesh->translate(oldPosition);
		mesh->scale(oldScale);
	}

	glUniform1f(glGetUniformLocation(shaderBasic.getId(), "useLight"), settings.useLight);
	glUniform1i(glGetUniformLocation(shaderBasic.getId(), "enableCustomColor"), settings.useCustomColor);
	glm::vec4 color = settings.customColor;
	glUniform4f(glGetUniformLocation(shaderBasic.getId(), "customColor"), color.r, color.g, color.b, color.a);

	glStencilFunc(GL_EQUAL, 0, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0xFF);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);

    drawRaw(shaderBasic);

    if(mesh->settings.normalVisible) {
        Shader shaderNormal = ResourceManager::loadShader(ShaderName::Renderer::Deferred::Debug::Normal);
        glUniformMatrix4fv(glGetUniformLocation(shaderNormal.getId(), "model"), 1, GL_FALSE, glm::value_ptr(mesh->getWorldTransform().getMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(shaderNormal.getId(), "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
            
        glBindVertexArray(this->VAO);
        glPointSize(mesh->settings.pointThickness);
        glDrawElements(GL_POINTS, mesh->getIndices().size(), GL_UNSIGNED_INT, 0);
    }

    DeferredDrawable::draw();
}

void DeferredMesh::drawRaw(Shader shader) {
    Mesh * mesh = static_cast<Mesh*>(getLinkedObject());

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

    DeferredDrawable::drawRaw(shader);
}

void DeferredMesh::dispose() {
    DeferredDrawable::dispose();

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void DeferredMesh::setup() {
    glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
}