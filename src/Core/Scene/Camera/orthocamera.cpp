#include "orthocamera.h"

OrthographicCamera::OrthographicCamera()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	world_up_vector = glm::vec3(0.0f, 1.0f, 0.0f);
	front_vector = glm::vec3(0.0f, 0.0f, -1.0f);
	updateCameraVectors();
}

OrthographicCamera::OrthographicCamera(glm::vec3 position, glm::vec3 up, glm::vec3 front)
{
	this->position = position;
	this->world_up_vector = up;
	this->front_vector = front;

	
	updateCameraVectors();
}

glm::mat4 OrthographicCamera::getCameraMatrix() {
    return glm::ortho(-0.5f * Width, 0.5f * Width, -0.5f * Height, 0.5f * Height, NearZ, FarZ);
}

ViewFrustum OrthographicCamera::getViewFrustum(int splits) {
	ViewFrustum viewFrustum;
	glm::vec3 nearCenter = position - (-front_vector * NearZ);

	std::vector<glm::vec3> farSplits;
	for (int i = 0; i < splits; i++) {
		farSplits.push_back(position - (-front_vector * (FarZ / (i + 1))));
	}

	Size sizeFar;
	sizeFar.height = Height;
	sizeFar.width = Width;

	Size sizeNear;
	sizeNear.height = Height;
	sizeNear.width = Width;

	std::vector<glm::vec3> nearCorners;
	nearCorners.push_back(nearCenter + up_vector * (sizeNear.height * 0.5f) - right_vector * (sizeNear.width * 0.5f));
	nearCorners.push_back(nearCenter + up_vector * (sizeNear.height * 0.5f) + right_vector * (sizeNear.width * 0.5f));
	nearCorners.push_back(nearCenter - up_vector * (sizeNear.height * 0.5f) - right_vector * (sizeNear.width * 0.5f));
	nearCorners.push_back(nearCenter - up_vector * (sizeNear.height * 0.5f) + right_vector * (sizeNear.width * 0.5f));
	viewFrustum.splits.push_back(nearCorners);

	while (splits > 0)
	{
		GLfloat split;
		if (splits == 3) {
			split = (FarZ * 0.2);
		}
		else {
			split = (FarZ / splits);
		}
		glm::vec3 splitCenter = position - (-front_vector * split);

		Size sizeSplit;
		sizeSplit.height = Height;
		sizeSplit.width = Width;

		std::vector<glm::vec3> splitCorners;
		splitCorners.push_back(splitCenter + up_vector * (sizeSplit.height * 0.5f) - right_vector * (sizeSplit.width * 0.5f));
		splitCorners.push_back(splitCenter + up_vector * (sizeSplit.height * 0.5f) + right_vector * (sizeSplit.width * 0.5f));
		splitCorners.push_back(splitCenter - up_vector * (sizeSplit.height * 0.5f) - right_vector * (sizeSplit.width * 0.5f));
		splitCorners.push_back(splitCenter - up_vector * (sizeSplit.height * 0.5f) + right_vector * (sizeSplit.width * 0.5f));
		viewFrustum.splits.push_back(splitCorners);
		splits--;
	}

	viewFrustum.position = position;
	viewFrustum.front = front_vector;
	viewFrustum.up = up_vector;
	viewFrustum.right = right_vector;

	viewFrustum.farZ = FarZ;
	viewFrustum.nearZ = NearZ;

	return viewFrustum;
}