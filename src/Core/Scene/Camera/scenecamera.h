#pragma once
#include "camera.h"
#include "Core/Mesh/Primitives/primitives.h"
#include "Core/Renderer/DeferredRenderer/deferred.h"
#include "Core/Renderer/ForwardRenderer/Basic/forward.h"
#include "Core/Renderer/arenderer.h"

/**
 * @brief config for scene cameras
 * 
 */
struct SceneCameraConfig {
	bool debugVisible = true;
	bool dLightVisible = true;
	bool sLightVisible = true;
	bool pLightVisible = true;
	bool ParticleVisible = true;
	bool Active = true;
};

/**
 * @brief wrapper for cameras in scenes
 * 
 */
class SceneCamera {
public:
	SceneCameraConfig config; //configuration

	Camera * camera; //camera object

	AbstractRenderer * renderer = nullptr; //camera renderer


	/**
	 * @brief Construct a new Scene Camera object
	 * 
	 * @param camera target camera
	 * @param frame see @ref resizeFrame
	 * @param sceneWidth 
	 * @param sceneHeight 
	 */
	SceneCamera(Camera &camera, Size frame, int sceneWidth, int sceneHeight);

	/**
	 * @brief Get the ViewFrustum as geometry
	 * 
	 * @param splits num of subregions in viewfrustums
	 * @return Geometry 
	 */
	Geometry getDebugViewFrustum(int splits = 0);

	/**
	 * @brief clear internal buffers
	 * 
	 */
	void clear();

	/**
	 * @brief Get the rendered texture from renderer
	 * 
	 * @return GLuint 
	 */
	GLuint getTexture();

	/**
	 * @brief free resources
	 * 
	 */
	void dispose();

	/**
	 * @brief resize internal objects
	 * 
	 * @param sceneWidth 
	 * @param sceneHeight 
	 */
	void resize(int sceneWidth, int sceneHeight);

	/**
	 * @brief 
	 * 
	 * Size(x, y, z, width, height, depth)
	 * offset x,y,z: 			  -1.0 - 1.0
	 * size width, height, depht:  0.0 - 2.0
	 * 
	 * @param frame 
	 */
	void resizeFrame(Size frame);

	/**
	 * @brief Get the size if the frame
	 * 
	 * see @ref resizeFrame
	 * 
	 * @return Size 
	 */
	Size getFrame();

	/**
	 * @brief draw the frame texture to the target
	 * 
	 * @param target target framebuffer
	 */
	void drawFrame(GLuint target);

private:
	Size frame;
	int width, height;
	GLuint camVAO, camVBO;

	/**
	 * @brief initialise bfuffers with given size
	 * 
	 * @param sceneWidth 
	 * @param sceneHeight 
	 */
	void setup(int sceneWidth, int sceneHeight);
	

};