
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<gl/glew.h>
GLfloat sensitivity_mouse = 0.05;
GLfloat sensitivity_scroll = 1;
GLfloat MOUSE_ZOOM = 180.0;

class camera {
public:
	glm::vec3 Position;
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	GLfloat yaw = -90.0f;	// Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
	GLfloat pitch = 0.0f;

	float zoom = 45.0;
	camera(glm::vec3);
	glm::mat4 GetViewMatrix();
	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset);
	void ProcessKeyboard(int direction, GLfloat deltatime);
	void handleMouseScroll(GLfloat yoffset);
};

camera::camera(glm::vec3 p) {
	this->Position = p;
}

glm::mat4 camera::GetViewMatrix() {
	glm::mat4 view;
	view = glm::lookAt(Position, Position + cameraFront, cameraUp);
	return view;
}

void camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset) {
	xoffset *= sensitivity_mouse;
	yoffset *= sensitivity_mouse;

	this->yaw += xoffset;
	this->pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (this->pitch > 89.0f)
		this->pitch = 89.0f;
	if (this->pitch < -89.0f)
		this->pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	this->cameraFront = glm::normalize(front);
}

void camera::ProcessKeyboard(int direction, GLfloat deltatime) {
	GLfloat cameraSpeed = 5.0f * deltatime;
	if (direction == 1)
		this->Position += cameraSpeed * cameraFront;
	if (direction == 2)
		this->Position -= cameraSpeed * cameraFront;
	if (direction == 3)
		this->Position -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (direction == 4)
		this->Position += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}


void camera::handleMouseScroll(GLfloat yoffset)
{
	if (this->zoom >= 1.0f && this->zoom <= MOUSE_ZOOM)
		this->zoom -= sensitivity_scroll * yoffset;
	if (this->zoom <= 1.0f)
		this->zoom = 1.0f;
	if (this->zoom >= 45.0f)
		this->zoom = 45.0f;
}