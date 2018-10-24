#include "mouse.h"

/*
Point getRelativeMousePosition(Window &window) {
	Point camPos = window.getCursorPosition();
	Size windowSize = window.getSize();
	camPos.position.x = (2.f * camPos.position.x) / windowSize.width - 1.0f;
	camPos.position.y = 1.0f - (2.f * camPos.position.y) / windowSize.height;
	camPos.position.z = 1.0f;
	return camPos;
}

std::pair<Point, Point> getCameraMousePosition(Window &window, Camera &camera) {
	Point relPos = getRelativeMousePosition(window);
	glm::vec3 camPos = relPos.position;
	glm::vec4 rayEye = glm::inverse(camera.getCameraMatrix()) * glm::vec4(camPos.x, camPos.y, -1.0f, 1.0f);
	glm::vec4 rayWorld = glm::inverse(camera.getViewMatrix()) * glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

	return std::pair<Point, Point>(Point{ relPos }, Point{ glm::normalize(glm::vec3(rayWorld)) });
}
*/