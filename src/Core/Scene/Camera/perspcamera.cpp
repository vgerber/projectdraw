#include "perspcamera.h"

PerspectiveCamera::PerspectiveCamera() {
	updateProjectionMatrix();
	updateViewMatrix();
}

void PerspectiveCamera::setFieldOfView(float fov) {
	this->fov = fov;
}

float PerspectiveCamera::getFieldOfView() {
	return fov;
}

ViewFrustum PerspectiveCamera::getViewFrustum(int splits)
{
	// Also re-calculate the Right and Up vector
	glm::vec3 worldPos = getWorldTransform().getTranslation();
	//transform direction vecotrs to world
	glm::mat4 worldRotation = getWorldTransform().getRotation().getRotationMatrix();
	glm::vec3 worldForward = worldRotation * glm::vec4(getBaseForward(), 0.0);
	glm::vec3 worldUp = worldRotation * glm::vec4(getBaseUp(), 0.0);
	glm::vec3 worldRight = worldRotation * glm::vec4(getBaseRight(), 0.0);

	float aspect = width / height;
	ViewFrustum viewFrustum;
	glm::vec3 nearCenter = worldPos - (-worldForward * nearZ);

	std::vector<glm::vec3> farSplits;
	for (int i = 0; i < splits; i++) {
		farSplits.push_back(worldPos - (-worldForward * (farZ / (i+1))));
	}

	Size sizeFar;
	sizeFar.height = 2.0f * tanf(glm::radians(fov) / 2.0f) * farZ;
	sizeFar.width  = sizeFar.height * aspect;

	Size sizeNear;
	sizeNear.height = 2.0f * tanf(glm::radians(fov) / 2.0f) * nearZ;
	sizeNear.width  = sizeNear.height * aspect;

	float rectSize = 0.6f;

	std::vector<glm::vec3> nearCorners;
	nearCorners.push_back(nearCenter + worldUp * (sizeNear.height * rectSize) - worldRight * (sizeNear.width * rectSize));
	nearCorners.push_back(nearCenter + worldUp * (sizeNear.height * rectSize) + worldRight * (sizeNear.width * rectSize));
	nearCorners.push_back(nearCenter - worldUp * (sizeNear.height * rectSize) - worldRight * (sizeNear.width * rectSize));
	nearCorners.push_back(nearCenter - worldUp * (sizeNear.height * rectSize) + worldRight * (sizeNear.width * rectSize));
	viewFrustum.splits.push_back(nearCorners);

	while (splits > 0)
	{
		float split;
		if (splits == 3) {
			split = (farZ * 0.2f);
		}
		else {
			split = (farZ / splits);
		}
		glm::vec3 splitCenter = worldPos - (-worldForward * split);

		Size sizeSplit;
		sizeSplit.height = 2.0f * tanf(glm::radians(fov) / 2.0f) * split;
		sizeSplit.width  = sizeSplit.height * aspect;

		std::vector<glm::vec3> splitCorners;
		splitCorners.push_back(splitCenter + worldUp * (sizeSplit.height * rectSize) - worldRight * (sizeSplit.width * rectSize));
		splitCorners.push_back(splitCenter + worldUp * (sizeSplit.height * rectSize) + worldRight * (sizeSplit.width * rectSize));
		splitCorners.push_back(splitCenter - worldUp * (sizeSplit.height * rectSize) - worldRight * (sizeSplit.width * rectSize));
		splitCorners.push_back(splitCenter - worldUp * (sizeSplit.height * rectSize) + worldRight * (sizeSplit.width * rectSize));
		viewFrustum.splits.push_back(splitCorners);
		splits--;
	}

	viewFrustum.position = worldPos;
	viewFrustum.front    = worldForward;
	viewFrustum.up		 = worldUp;
	viewFrustum.right	 = worldRight;

	viewFrustum.farZ  = farZ;
	viewFrustum.nearZ = nearZ;

	return viewFrustum;
}

void PerspectiveCamera::updateProjectionMatrix() {
	if(height != 0.0f) {
		projMatrix = glm::perspective(glm::radians(fov), width / height, nearZ, farZ);
	}
	else {
		printf("[Warning] Can not construct projection matrix\n");
		projMatrix = glm::mat4(0.0);
	}
}