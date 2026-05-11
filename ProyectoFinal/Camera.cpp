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

	// Variables para animación
	velocidadAnimacion = 0.0f;
	caminando = false;

	// Modo de cámara
	cameraMode = 1; // Iniciar en modo tercera persona
	aerialPosition = glm::vec3(0.0f, 90.0f, 0.0f); // Posición inicial aérea (50 unidades arriba)
	fixedPosition = glm::vec3(106.0f, 2.5f, 56.0f); // Posición fija de la cámara   para la espada en la piedra
	fixedTarget = glm::vec3(106.0f, 1.0f, 76.0f); 
	fixedPosition2 = glm::vec3(-55.0f, 8.0f, 136.0f); // Cámara fija 2 (tecla 4)  iglesia
	fixedTarget2 = glm::vec3(-146.0f, 7.0f, 136.0f);
	fixedPosition3 = glm::vec3(0.0f, 50.0f, 140.0f); // Cámara fija 3 (tecla 5)	  casa en las rocas flotantes
	fixedTarget3 = glm::vec3(0.0f, 50.0f, 220.0f);


	// Parámetros de cámara de tercera persona

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

	if (cameraMode == 1) // Modo tercera persona con avatar
	{
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
		
		// Actualizar tiempo de animación
		velocidadAnimacion += deltaTime/8; 
		caminando = true;
	}
	else
	{
		caminando = false;
	}

	// Mantener el avatar en el plano XZ
	avatarPosition.y = 0.0f;

	// Actualizar la posición de la cámara basada en el avatar
	updateCameraPosition();
}
	else if (cameraMode == 2) // Modo cámara aérea
	{
		// Movimiento de cámara aérea en el plano XZ (vista desde arriba)
		glm::vec3 forwardXZ = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
		glm::vec3 rightXZ = glm::normalize(glm::vec3(right.x, 0.0f, right.z));

		if (keys[GLFW_KEY_W])
		{
			aerialPosition += forwardXZ * velocity * 2.0f; // Velocidad ajustada
		}

		if (keys[GLFW_KEY_S])
		{
			aerialPosition -= forwardXZ * velocity * 2.0f;
		}

		if (keys[GLFW_KEY_A])
		{
			aerialPosition -= rightXZ * velocity * 2.0f;
		}

		if (keys[GLFW_KEY_D])
		{
			aerialPosition += rightXZ * velocity * 2.0f;
		}

		// Mantener altura constante en modo aéreo
		aerialPosition.y = 25.0f; // Altura fija de 25 unidades

		// Actualizar posición de la cámara
		position = aerialPosition;

		// No hay animación de caminar en modo aéreo
		caminando = false;
		}
		else if (cameraMode == 3) // Modo cámara fija 1
		{
			position = fixedPosition;
			caminando = false;
		}
		else if (cameraMode == 4) // Modo cámara fija 2
		{
			position = fixedPosition2;
			caminando = false;
		}
		else if (cameraMode == 5) // Modo cámara fija 3
		{
			position = fixedPosition3;
			caminando = false;
		}
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (cameraMode == 1) // Modo tercera persona
	{
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
	else if (cameraMode == 2) // Modo aéreo
	{
		// Limitar el pitch para cámara aérea
		if (pitch > 89.0f)
		{
			pitch = 89.0f;
		}

		if (pitch < -89.0f)
		{
			pitch = -89.0f;
		}

		update(); // Actualizar vectores front, right, up
	}
	else if (cameraMode == 2) // Modo aéreo
	{
		// Limitar el pitch para cámara aérea
		if (pitch > 89.0f)
		{
			pitch = 89.0f;
		}

		if (pitch < -89.0f)
		{
			pitch = -89.0f;
		}

		update(); // Actualizar vectores front, right, up
	}
	else if (cameraMode == 3 || cameraMode == 4 || cameraMode == 5) // Cámaras fijas
	{
		// Las cámaras fijas no responden al mouse
		// No hacer nada
	}


}

glm::mat4 Camera::calculateViewMatrix()
{
	if (cameraMode == 1) // Tercera persona
	{
		// La cámara siempre mira hacia el avatar
		glm::vec3 lookAtTarget = glm::vec3(avatarPosition.x, alturaSobrePiso + 2.0f, avatarPosition.z);
		return glm::lookAt(position, lookAtTarget, worldUp);
	}
	else if (cameraMode == 2) // Cámara aérea
	{
		// La cámara mira hacia donde apunta el frente
		return glm::lookAt(position, position + front, worldUp);
	}
	else if (cameraMode == 3) // Cámara fija 1
	{
		return glm::lookAt(fixedPosition, fixedTarget, worldUp);
	}
	else if (cameraMode == 4) // Cámara fija 2
	{
		return glm::lookAt(fixedPosition2, fixedTarget2, worldUp);
	}
	else // Cámara fija 3 (modo 5)
	{
		return glm::lookAt(fixedPosition3, fixedTarget3, worldUp);
	}
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

float Camera::getVelocidadAnimacion()
{
	return velocidadAnimacion;
}

bool Camera::estaCaminando()
{
	return caminando;
}

int Camera::getCameraMode()
{
	return cameraMode;
}

void Camera::setFixedCameraTarget(glm::vec3 targetPos)
{
	fixedTarget = targetPos;
}

void Camera::setCameraMode(int mode)
{
	cameraMode = mode;

	if (mode == 2) // Al cambiar a modo aéreo
	{
		// Posicionar cámara aérea sobre la posición actual del avatar
		aerialPosition = glm::vec3(avatarPosition.x, 15.0f, avatarPosition.z);
		position = aerialPosition;
	}
	else if (mode == 1) // Al volver a modo tercera persona
	{
		updateCameraPosition();
	}
	else if (mode == 3) // Al cambiar a modo fijo 1
	{
		position = fixedPosition;
	}
	else if (mode == 4) // Al cambiar a modo fijo 2
	{
		position = fixedPosition2;
	}
	else if (mode == 5) // Al cambiar a modo fijo 3
	{
		position = fixedPosition3;
	}
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
