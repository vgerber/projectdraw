#pragma once
#include "GL/glew.h"

#include "Core/Scene/Camera/camera.h"


enum RenderMode {
	POINTR = 0,
	LINER = 1,
	FILLR = 2
};

struct Viewport {
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
};

class AbstractRenderer {
public:
    AbstractRenderer(int width, int height, Camera &camera);
    
    /**
     * @brief Resize internals to new size
     * 
     * @param width 
     * @param height 
     */
    virtual void resize(int width, int height);

    /**
     * @brief Set rendering viewport
     * 
     * @param viewport 
     */
    virtual void setViewport(Viewport viewport);

    /**
     * @brief 
     * 
     */
    virtual void clearScreen() = 0;

    /**
     * @brief Set the Render Mode for rendering process
     * 
     * @param rmode 
     */
    virtual void setRenderMode(RenderMode rmode);

    /**
     * @brief Starts rendering
     * 
     */
    virtual void render() = 0;

    /**
     * @brief Add an @ref SceneObject to rendering process
     * 
     * @param sceneObject 
     */
	virtual void addSceneObject(SceneObject &sceneObject) = 0;    

    /**
     * @brief Remove @ref SceneObject from rendering process
     * 
     * @param sceneObject 
     */
	virtual void removeSceneObject(SceneObject &sceneObject) = 0;

    /**
     * @brief Get rendered scene
     * 
     * @return GLuint 
     */
    virtual GLuint getTexture() = 0;

    /**
     * @brief Free allocated memory
     * 
     */
    virtual void dispose() = 0;

    /**
     * @brief Get the rendering width in pixel
     * 
     * @return int 
     */
    int getWidth();

    /**
     * @brief Get the rendering height in pixel
     * 
     * @return int 
     */
    int getHeight();

    /**
     * @brief Get the unique RendererID
     * 
     * @return int 
     */
    int getRendererId();

    /**
     * @brief Set the @ref Camera for scene rendering
     * 
     * @param camera 
     */
    void setCamera(Camera &camera);

    /**
     * @brief Get the renderer @ref Camera 
     * 
     * @return Camera* 
     */
    Camera * getCamera();

private:
    static int globalRendererId;
    int rendererId;
    int width = 0, height = 0;
protected:
    Viewport viewport;
    RenderMode renderMode;
    Camera *camera;
	bool invalidShaders = false;    
};