#include "orthocamera.h"

OrthographicCamera::OrthographicCamera(glm::vec3 position, glm::vec3 forward,  glm::vec3 up) :  Camera(position, forward, up) {

}

ViewFrustum OrthographicCamera::getViewFrustum(int splits) {
	ViewFrustum viewFrustum;
	glm::vec3 nearCenter = getPosition() - (getForward() * nearZ);

	std::vector<glm::vec3> farSplits;
	for (int i = 0; i < splits; i++) {
		farSplits.push_back(getPosition() - (-getForward() * (farZ / (i + 1))));
	}

	Size sizeFar;
	sizeFar.height = height;
	sizeFar.width  = width;

	Size sizeNear;
	sizeNear.height = height;
	sizeNear.width  = width;

	std::vector<glm::vec3> nearCorners;
	nearCorners.push_back(nearCenter + getUp() * (sizeNear.height * 0.5f) - getRight() * (sizeNear.width * 0.5f));
	nearCorners.push_back(nearCenter + getUp() * (sizeNear.height * 0.5f) + getRight() * (sizeNear.width * 0.5f));
	nearCorners.push_back(nearCenter - getUp() * (sizeNear.height * 0.5f) - getRight() * (sizeNear.width * 0.5f));
	nearCorners.push_back(nearCenter - getUp() * (sizeNear.height * 0.5f) + getRight() * (sizeNear.width * 0.5f));
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
		glm::vec3 splitCenter = getPosition() - (-getForward()* split);

		Size sizeSplit;
		sizeSplit.height = height;
		sizeSplit.width  = width;

		std::vector<glm::vec3> splitCorners;
		splitCorners.push_back(splitCenter + getUp() * (sizeSplit.height * 0.5f) - getRight() * (sizeSplit.width * 0.5f));
		splitCorners.push_back(splitCenter + getUp() * (sizeSplit.height * 0.5f) + getRight() * (sizeSplit.width * 0.5f));
		splitCorners.push_back(splitCenter - getUp() * (sizeSplit.height * 0.5f) - getRight() * (sizeSplit.width * 0.5f));
		splitCorners.push_back(splitCenter - getUp() * (sizeSplit.height * 0.5f) + getRight() * (sizeSplit.width * 0.5f));
		viewFrustum.splits.push_back(splitCorners);
		splits--;
	}

	viewFrustum.position = getPosition();
	viewFrustum.front    = getForward();
	viewFrustum.up       = getUp();
	viewFrustum.right    = getRight();

	viewFrustum.farZ  = farZ;
	viewFrustum.nearZ = nearZ;

	return viewFrustum;
}

void OrthographicCamera::updateProjectionMatrix() {
	projMatrix = glm::ortho(-0.5f * width, 0.5f * width, -0.5f * height, 0.5f * height, nearZ, farZ);
}