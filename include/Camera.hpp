#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "stdafx.h"

class Camera {
public:
	Camera();
	Camera(int w, int h);

	void UpdateView();

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetFrustrumMatrix();

	void UpdateProjection(int width, int height);

	// Keyboard
	void MoveCamera(SDL_Scancode scancode);
	// Mouse
	void HandleMouse(SDL_Event e);
	// Mouse Delta
	void MouseMoved(glm::vec2 mouseDelta);

	// Updatable by 
	float MouseSensitivity = 0.1f;
	float CameraSpeed = 1.0f;

	void UpdatePosition(glm::vec3 position);
	void UpdateEulerLookDirection(float roll, float pitch, float yaw);
	void UpdateLookDirection(glm::vec3 lookDirection);

	glm::vec3 Position = {};
	float Roll, Pitch, Yaw;
	glm::vec3 LookDirection = {};

private:

	glm::mat4 viewMatrix = {};
	glm::mat4 projMatrix = {};
	
};

#endif