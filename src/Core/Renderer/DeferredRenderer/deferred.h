#pragma once


#include "Core/core.h"
#include "Core/Scene/Camera/camera.h"
#include "Core/Scene/drawable.h"
#include "Core/Scene/Light/lights.h"
#include "Core/Scene/Particle/particlegenerator.h"
#include "Core/Scene/Animation/animatable.h"
#include "Core/Scene/Text/text.h"
#include "Core/Texture/Filter/Antialias/smaa.h"

#include "Core/Renderer/arenderer.h"
#include "Core/Renderer/DeferredRenderer/light_util.h"

#include "Core/Renderer/DeferredRenderer/Interface/deferred_sceneobject.h"
#include "Core/Renderer/DeferredRenderer/Interface/deferred_text.h"
#include "Core/Renderer/DeferredRenderer/Interface/deferred_mesh.h"

struct SortPointLights {
	glm::vec3 cameraPosition;

	SortPointLights(glm::vec3 cameraPosition) { this->cameraPosition = cameraPosition; }

	inline bool operator() (PointLight * p1, PointLight * p2) {
		float p1Value = 0.0;
		float p2Value = 0.0;

		if(p1->shadow && !p2->shadow) {
			return true;
		}
		else if(p1->shadow && p2->shadow) {
			p1Value = glm::distance2(cameraPosition, p1->getPosition());
			p2Value = glm::distance2(cameraPosition, p2->getPosition());
			if(p1Value < p2Value) {
				p1Value = p1->intensity;
				p2Value = p2->intensity;
				if (p1Value > p2Value) {
					return true;
				}
			}
		}
		return false;
	}
};

struct SortSpotLights {
	glm::vec3 cameraPosition;

	SortSpotLights(glm::vec3 cameraPosition) { this->cameraPosition = cameraPosition; }

	inline bool operator() (SpotLight * s1, SpotLight * s2) {
		float s1Value = 0.0;
		float s2Value = 0.0;

		if(s1->shadow && !s2->shadow) {
			return true;
		}
		else if(s1->shadow && s2->shadow) {
			s1Value = glm::distance2(cameraPosition, s1->getPosition());
			s2Value = glm::distance2(cameraPosition, s2->getPosition());
			if(s1Value < s2Value) {
				s1Value = s1->intensity;
				s2Value = s2->intensity;
				if (s1Value > s2Value) {
					return true;
				}
			}
		}
		return false;
	}
};

struct SortDrawable {
	glm::vec3 cameraPosition;

	SortDrawable(glm::vec3 cameraPosition) { this->cameraPosition = cameraPosition; }

	inline bool operator() (Drawable *d1, Drawable *d2) {
		if((d1->settings.xrayVisible || d2->settings.xrayVisible)) {
			if (!d1->settings.xrayVisible && d2->settings.xrayVisible) {
				return true;
			}
			if(d1->settings.xrayUseLight && !d2->settings.xrayVisible) {
				return false;
			}
		}
		else {
			if (glm::distance2(d1->getPosition(), cameraPosition) < glm::distance2(d2->getPosition(), cameraPosition)) {
				return true;
			}
		}
		return false;
	}
};

class DeferredRenderer : public AbstractRenderer {
public:	

    DeferredRenderer(int width, int height, Camera &camera);   



    virtual void resize(int width, int height) override;

    virtual void clearScreen() override;

	virtual void render();

	virtual void addSceneObject(SceneObject &sceneObject);
	virtual void removeSceneObject(SceneObject &sceneObject);

	virtual GLuint getTexture() override;

	virtual void dispose() override;

protected:

	static const int PointLightShadows = 10;
	static const int SpotLightShadows = 10;

	static const int ShaderPointLightShadowSize = 1;
	static const int ShaderPointLightSize = 100;
	static const int ShaderSpotLightShadowSize = 10;
	static const int ShaderSpotLightSize = 100;

	static const int AASamples = 4;

	//light shaders
	Shader shaderDLight;
	Shader shaderDLightShadow;
	Shader shaderPLight;
	Shader shaderPLightShadow;
	Shader shaderSLight; 
	Shader shaderSLightShadow; 
	Shader shaderTexture;
	
	//object drawing shader
	Shader shaderBasic;
	Shader shaderInstancing;
	Shader shaderFont;
	Shader shaderLight;
	

	//util shader
	Shader shaderNormals;

	//post
	Shader shaderFXAA;
	Shader shaderHDR;
	Shader shaderBloomBlur;
	Shader shaderBloomMerge;


	GLuint gBufferFBO = 0;
	GLuint rboGDepth = 0;
	GLuint gBufferPosition = 0;
    GLuint gBufferNormal = 0;
    GLuint gBufferAlbedo = 0;
    GLuint gBufferOption1 = 0;
    GLuint gBufferGlow = 0;
    GLuint gDepthStencil = 0;

	GLuint bloomFBO = 0;
	

	GLuint uboMatrices = 0;

	GLuint screenRectFBO = 0;
	GLuint screenRectVBO = 0;
	GLuint screenRectVAO = 0;
	GLuint screenRectTexture = 0;

	   
	GLuint tmpRenderTexture = 0;


	static const int bloomSample = 4;
	GLuint bloomTextures[bloomSample];
	GLuint tmpbloomTextures[bloomSample];


	std::vector<DeferredSceneObject*> sceneObjects;

	//lights
	DirectionalLight* directionalLight = nullptr;
	std::vector<PointLight*> pointLights;
	std::vector<SpotLight*> spotLights;

	std::vector<Animatable*> animatables;


	std::vector<Text*> textObjects;

	//Filter
	SMAA smaaFilter;


	virtual void renderObjects();

	///Merge colorbuffers and apply light and shadow effects
	virtual void renderLight();

	virtual void applyAntialias();
	virtual void applyBloom();
	virtual void applyHDR();

	void setup();

	DeferredSceneObject * generateDeferredObject(SceneObject * sceneObject);

	void updateSceneObjectTree(SceneObject * sceneObject);

};