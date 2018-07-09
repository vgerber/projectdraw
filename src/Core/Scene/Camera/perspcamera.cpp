#include "perspcamera.h"

glm::mat4 PerspectiveCamera::GetCameraMatrix()
{
	return glm::perspective(glm::radians(FOV), Width / Height, NearZ, FarZ);
}

ViewFrustum PerspectiveCamera::getViewFrustum(int splits)
{
	ViewFrustum viewFrustum;
	glm::vec3 nearCenter = position - (-front_vector * NearZ);

	std::vector<glm::vec3> farSplits;
	for (int i = 0; i < splits; i++) {
		farSplits.push_back(position - (-front_vector * (FarZ / (i+1))));
	}

	Size sizeFar;
	sizeFar.height = 2.0f * tanf(glm::radians(FOV) / 2.0f) * FarZ;
	sizeFar.width = sizeFar.height * (800.0f / 600.0f);

	Size sizeNear;
	sizeNear.height = 2.0f * tanf(glm::radians(FOV) / 2.0f) * NearZ;
	sizeNear.width = sizeNear.height * (800.0f / 600.0f);

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
		sizeSplit.height = 2.0f * tanf(glm::radians(FOV) / 2.0f) * split;
		sizeSplit.width = sizeSplit.height * (800.0f / 600.0f);

		std::vector<glm::vec3> splitCorners;
		splitCorners.push_back(splitCenter + up_vector * (sizeSplit.height * 0.55f) - right_vector * (sizeSplit.width * 0.55f));
		splitCorners.push_back(splitCenter + up_vector * (sizeSplit.height * 0.55f) + right_vector * (sizeSplit.width * 0.55f));
		splitCorners.push_back(splitCenter - up_vector * (sizeSplit.height * 0.55f) - right_vector * (sizeSplit.width * 0.55f));
		splitCorners.push_back(splitCenter - up_vector * (sizeSplit.height * 0.55f) + right_vector * (sizeSplit.width * 0.55f));
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