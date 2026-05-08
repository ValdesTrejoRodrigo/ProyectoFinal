#pragma once

#include <glew.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>

#include <glfw3.h>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);

	void keyControl(bool* keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);

	glm::vec3 getCameraPosition();
	glm::vec3 getCameraDirection();
	glm::vec3 getAvatarPosition(); // Posición del avatar
	float getAvatarRotation(); // Rotación del avatar
	float getYaw(); // Para orientar el avatar
	float getVelocidadAnimacion(); // Tiempo para animación de caminar
	bool estaCaminando(); // Indica si el avatar está caminando
	void setCameraMode(int mode); // Cambiar modo de cámara (1 = tercera persona, 2 = aérea, 3 = fija)
	void setFixedCameraTarget(glm::vec3 targetPos); // Establecer objetivo de cámara fija
	int getCameraMode(); // Obtener modo actual
	glm::mat4 calculateViewMatrix();

	~Camera();

private:
	glm::vec3 position;      // Posición de la cámara
	glm::vec3 avatarPosition; // Posición del avatar en el plano XZ
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat avatarYaw; // Para orientar el avatar
	GLfloat pitch;

	GLfloat moveSpeed;
	GLfloat turnSpeed;

	// Variables para animación de caminar
	GLfloat velocidadAnimacion;
	bool caminando;

	int cameraMode; // 1 = tercera persona con avatar, 2 = aérea, 3 = fija
	glm::vec3 aerialPosition; // Posición de la cámara aérea
	glm::vec3 fixedPosition; // Posición fija de la cámara
	glm::vec3 fixedTarget; // Objetivo que mira la cámara fija

	// Parámetros de cámara de tercera persona
	GLfloat distanciaDetrasAvatar;  // Distancia de la cámara al avatar
	GLfloat alturaSobreAvatar;   // Altura de la cámara sobre el avatar
	GLfloat alturaSobrePiso;        // Altura del avatar sobre el piso

	void update();
	void updateCameraPosition(); // Actualizar posición de cámara en 3era persona
};
