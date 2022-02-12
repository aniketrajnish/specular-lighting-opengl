#include "camera.h"

Camera::Camera(glm::vec3 position)
	: pos(position),
	worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	yaw(-90.0f),
	pitch(0.0f),
	speed(2.5f),
	sensitivity(1.0f),
	zoom(45.0f),
	cameraFront(glm::vec3(0.0f, 0.0f, -1.0f))
{
	updateCameraVectors();
}

void Camera::updatePos(CameraDirection direction, double dt) {
	float velocity = (float)dt * speed;

	switch (direction) {
	case CameraDirection::FORWARD:
		pos += cameraFront * velocity;
		break;
	case CameraDirection::BACKWARD:
		pos -= cameraFront * velocity;
		break;
	case CameraDirection::RIGHT:
		pos += cameraRight * velocity;
		break;
	case CameraDirection::LEFT:
		pos -= cameraRight * velocity;
		break;	
	}
}


glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(pos, pos + cameraFront, cameraUp);
}

float Camera::getZoom() {
	return zoom;
}
	
void Camera::updateCameraVectors() {
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);

	cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}