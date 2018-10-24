#include "perspcamera.h"

glm::mat4 PerspectiveCamera::getCameraMatrix()
{
	return glm::perspective(glm::radians(FOV), Width / Height, NearZ, FarZ);
}

ViewFrustum PerspectiveCamera::getViewFrustum(int splits)
{
	float aspect = Width / Height;
	ViewFrustum viewFrustum;
	glm::vec3 nearCenter = position - (-front_vector * NearZ);

	std::vector<glm::vec3> farSplits;
	for (int i = 0; i < splits; i++) {
		farSplits.push_back(position - (-front_vector * (FarZ / (i+1))));
	}

	Size sizeFar;
	sizeFar.height = 2.0f * tanf(glm::radians(FOV) / 2.0f) * FarZ;
	sizeFar.width = sizeFar.height * aspect;

	Size sizeNear;
	sizeNear.height = 2.0f * tanf(glm::radians(FOV) / 2.0f) * NearZ;
	sizeNear.width = sizeNear.height * aspect;

	float rectSize = 0.6f;

	std::vector<glm::vec3> nearCorners;
	nearCorners.push_back(nearCenter + up_vector * (sizeNear.height * rectSize) - right_vector * (sizeNear.width * rectSize));
	nearCorners.push_back(nearCenter + up_vector * (sizeNear.height * rectSize) + right_vector * (sizeNear.width * rectSize));
	nearCorners.push_back(nearCenter - up_vector * (sizeNear.height * rectSize) - right_vector * (sizeNear.width * rectSize));
	nearCorners.push_back(nearCenter - up_vector * (sizeNear.height * rectSize) + right_vector * (sizeNear.width * rectSize));
	viewFrustum.splits.push_back(nearCorners);

	while (splits > 0)
	{
		float split;
		if (splits == 3) {
			split = (FarZ * 0.2);
		}
		else {
			split = (FarZ / splits);
		}
		glm::vec3 splitCenter = position - (-front_vector * split);

		Size sizeSplit;
		sizeSplit.height = 2.0f * tanf(glm::radians(FOV) / 2.0f) * split;
		sizeSplit.width = sizeSplit.height * aspect;

		std::vector<glm::vec3> splitCorners;
		splitCorners.push_back(splitCenter + up_vector * (sizeSplit.height * rectSize) - right_vector * (sizeSplit.width * rectSize));
		splitCorners.push_back(splitCenter + up_vector * (sizeSplit.height * rectSize) + right_vector * (sizeSplit.width * rectSize));
		splitCorners.push_back(splitCenter - up_vector * (sizeSplit.height * rectSize) - right_vector * (sizeSplit.width * rectSize));
		splitCorners.push_back(splitCenter - up_vector * (sizeSplit.height * rectSize) + right_vector * (sizeSplit.width * rectSize));
		viewFrustum.splits.push_back(splitCorners);
		splits--;
	}

	viewFrustum.position = position;
	viewFrustum.front    = front_vector;
	viewFrustum.up		 = up_vector;
	viewFrustum.right	 = right_vector;

	viewFrustum.farZ = FarZ;
	viewFrustum.nearZ = NearZ;

	return viewFrustum;
}