#pragma once

#include "Core/Scene/sceneobject.h"
#include "Core/Shader/shader.h"
#include "Core/Renderer/arenderer.h"
#include "Core/Texture/texture.h"

#include <algorithm>
#include <memory>

struct Vertex {
	glm::vec3 Position  = glm::vec3(0.0);
	glm::vec3 Normal    = glm::vec3(0.0);
	glm::vec2 TexCoords = glm::vec2(0.0);
	glm::vec4 Color     = glm::vec4(0.0);

	Vertex() {}
	Vertex(glm::vec3 position) : Position(position) {}
	Vertex(glm::vec3 position, glm::vec4 color) : Position(position), Color(color) {}
	Vertex(glm::vec3 position, glm::vec4 color, glm::vec2 texCoords) : Position(position), Color(color), TexCoords(texCoords) {}
	Vertex(glm::vec3 position, glm::vec4 color, glm::vec2 texCoords, glm::vec3 normal) : Position(position), Color(color), TexCoords(texCoords), Normal(normal) {}
};

struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLfloat shininess;
};

enum DrawType {
	TRIANGLEG,
	POINTG,
	LINEG,
};



struct DrawableInfo {
	//Drawing
	DrawType drawType = DrawType::TRIANGLEG;
	bool useLight = true;
	bool useCustomColor = false;
	glm::vec4 customColor = glm::vec4(1.0f);

	//Normals
	bool normalVisible = false;
	float normalSize = 1.0f;
	float normalThickness = 1.0f;
	glm::vec4 normalColor = glm::vec4(1.0f);

	//Box
	bool boxVisible = false;
	float boxThickness = 1.0f;
	glm::vec4 boxColor = glm::vec4(1.0f);

	//XRay
	bool xrayVisible = false;
	bool xrayUseLight = false;
	bool xrayCustomColor = true;
	glm::vec4 xrayColor = glm::vec4(1.0f);
	DrawType xrayDrawType = DrawType::TRIANGLEG;

	//Outline
	bool outlineVisible = false;
	float outlineThickness = 1.0f;
	glm::vec4 outlineColor = glm::vec4(1.0f);

	//Texture
	bool useDiffuseTexture = true;
	bool useAlphaTexture = true;
	bool useSpecualTexture = true;

	//Geometry
	unsigned int lineThickness = 1;
	unsigned int pointThickness = 1;
};

/**
 * @brief Base class for renderable objects
 * 
 */
class Drawable : public SceneObject
{
public:
	/**
	 * @brief Drawing options
	 * DrawableInfo are optional parameters for renderer
	 * Implementation depends on renderer 
	 */
	DrawableInfo settings;

	/**
	 * @brief Add new diffuse texture
	 * 
	 * @param texture 
	 */
	void addTexture(const Texture &texture, TextureType type);

	/**
	 * @brief Remove diffuse texture
	 * 
	 * @param texture 
	 */
	void removeTexture(Texture &texture);

	/**
	 * @brief Get the Textures
	 * 
	 * @return const std::vector<Texture*> 
	 */
	const std::vector<const Texture*> getDiffuseTextures() const;

	/**
	 * @brief Get the Alpha Texture
	 * 
	 * @return const Texture* 
	 */
	const std::vector<const Texture*> getAlphaTextures() const;

	/**
	 * @brief Get the Specular Texture
	 * 
	 * @return const Texture* 
	 */
	const std::vector<const Texture*> getSpecularTextures() const;

	/**
	 * @brief Get object size
	 * 
	 * @return Size 
	 */
	virtual Size getSize() = 0;

	/**
	 * @brief Draw object
	 * 
	 * @param drawType Mesh representation
	 */
	virtual void draw(DrawType drawType = DrawType::TRIANGLEG) = 0;

	/**
	 * @brief Set the internal mvp matrix
	 * 
	 * Function will be used by renderer
	 * 
	 * @param cView Camera view
	 * @param cProj Camera projection
	 */
	virtual void setCameraMatrices(glm::mat4 cView, glm::mat4 cProj);

	/**
	 * @brief Get calculated mvp matrix
	 * 
	 * @return glm::mat4 
	 */
	glm::mat4 getMVPMatrix();

	/**
	 * @brief Free all resources
	 * 
	 */
	void dispose();	

	/**
	 * @brief Returns wether object vertex data has been changed or not
	 * 
	 * @return true If data has been changed
	 * @return false 
	 */
	bool isModified(); 

	/**
	 * @brief Sets Modified flag to false
	 * 
	 */
	void clearModifiedFlag();

protected:
	glm::mat4 mvp = glm::mat4(1.0); //cached mvp matrix 

	//added textures
	std::vector<const Texture*> diffuseTextures;
	std::vector<const Texture*> specularTextures; 
	std::vector<const Texture*> alphaTextures;

	bool dataChanged = false; //true if drawing data has been modified
private:
};
