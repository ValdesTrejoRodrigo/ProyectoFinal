#include "Camera.h"

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	// La posición inicial representa dónde está el AVATAR en el plano XZ
	avatarPosition = startPosition;
	avatarPosition.y = 0.0f; // El avatar siempre está en el plano XZ (y=0)
	
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	// Parámetros de cámara de tercera persona
	distanciaDetrasAvatar = 8.0f;  // Distancia detrás del avatar
	alturaSobreAvatar = 4.0f;   // Altura de la cámara sobre el avatar
	alturaSobrePiso = 0.35f;        // Altura del avatar sobre el suelo

	update();
	updateCameraPosition();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;

	// Crear vectores de movimiento en el plano XZ
	glm::vec3 forwardXZ = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
	glm::vec3 rightXZ = glm::normalize(glm::vec3(right.x, 0.0f, right.z));

	// Variable para detectar si hay movimiento
	bool isMoving = false;
	glm::vec3 moveDirection(0.0f);

	// Mover el AVATAR en el plano XZ y calcular dirección de movimiento
	if (keys[GLFW_KEY_W])
	{
		avatarPosition += forwardXZ * velocity;
		moveDirection += forwardXZ;
		isMoving = true;
	}

	if (keys[GLFW_KEY_S])
	{
		avatarPosition -= forwardXZ * velocity;
		moveDirection -= forwardXZ;
		isMoving = true;
	}

	if (keys[GLFW_KEY_A])
	{
		avatarPosition -= rightXZ * velocity;
		moveDirection -= rightXZ;
		isMoving = true;
	}

	if (keys[GLFW_KEY_D])
	{
		avatarPosition += rightXZ * velocity;
		moveDirection += rightXZ;
		isMoving = true;
	}


	// Si hay movimiento, calcular la rotación del avatar hacia la dirección de movimiento
	if (isMoving)
	{
		moveDirection = glm::normalize(moveDirection);
		// Calcular el ángulo de rotación basado en la dirección de movimiento
		// atan2 nos da el ángulo en radianes, lo convertimos a grados
		avatarYaw = glm::degrees(atan2(moveDirection.x, moveDirection.z)) +90.0f;
	}

	// Mantener el avatar en el plano XZ
	avatarPosition.y = 0.0f;

	// Actualizar la posición de la cámara basada en el avatar
	updateCameraPosition();
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	// Limitar el pitch para cámara de tercera persona
	if (pitch > 45.0f)
	{
		pitch = 45.0f;
	}

	if (pitch < -45.0f)
	{
		pitch = -45.0f;
	}

	update();
	updateCameraPosition();
}

glm::mat4 Camera::calculateViewMatrix()
{
	// La cámara siempre mira hacia el avatar
	glm::vec3 lookAtTarget = glm::vec3(avatarPosition.x, alturaSobrePiso + 2.0f, avatarPosition.z);
	return glm::lookAt(position, lookAtTarget, worldUp);
}

glm::vec3 Camera::getCameraPosition()
{
	return position;
}


glm::vec3 Camera::getCameraDirection()
{
	return glm::normalize(front);
}

glm::vec3 Camera::getAvatarPosition()
{
	// Retornar la posición del avatar en el plano XZ con su altura
	return glm::vec3(avatarPosition.x, alturaSobrePiso, avatarPosition.z);
}


float Camera::getYaw()
{
	return yaw;
}

float Camera::getAvatarRotation()
{
	return avatarYaw;
}

void Camera::updateCameraPosition()
{
	// Calcular la posición de la cámara detrás y arriba del avatar
	// Basado en yaw y pitch
	
	// Dirección horizontal (solo en XZ)
	glm::vec3 horizontalDirection;
	horizontalDirection.x = cos(glm::radians(yaw));
	horizontalDirection.y = 0.0f;
	horizontalDirection.z = sin(glm::radians(yaw));
	horizontalDirection = glm::normalize(horizontalDirection);

	// Posición de la cámara detrás del avatar
	position = avatarPosition - horizontalDirection * distanciaDetrasAvatar;
	
	// Ajustar altura de la cámara
	position.y = alturaSobrePiso + alturaSobreAvatar;
	
	// Ajustar altura adicional basada en el pitch
	GLfloat pitchOffset = distanciaDetrasAvatar * tan(glm::radians(pitch));
	position.y += pitchOffset;
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}


Camera::~Camera()
{
}
