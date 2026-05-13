#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	cicloDyN = 0.0f;  // Inicializar tiempo del ciclo
}

void DirectionalLight::UpdateCycle(GLfloat deltaTime)
{
	// Actualizar el tiempo del ciclo (120 segundos = 2 minutos)
	cicloDyN += deltaTime /130.0f;//130 para que el ciclo completo dure 2 minutos
	if (cicloDyN >= 120.0f)
	{
		cicloDyN = 0.0f;
	}

	// Calcular el ángulo del sol (0 a 360 grados en 2 minutos)
	GLfloat angle = (cicloDyN / 120.0f) * 360.0f;
	GLfloat radians = glm::radians(angle);

	// Simular el movimiento del sol (de este a oeste, pasando por arriba)
	direction.x = sin(radians);
	direction.y = -cos(radians);  // Negativo para que el sol salga arriba
	direction.z = 0.0f;

	// Normalizar la dirección
	direction = glm::normalize(direction);

	// Cambiar color e intensidad según la hora del día
	GLfloat alturaSol = direction.y;  // -1 (mediodía) a 1 (medianoche)

	if (alturaSol < -0.1f)  // Día (sol arriba)
	{
		// Color blanco brillante
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		ambientIntensity = 0.4f;
		diffuseIntensity = 0.8f;
	}
	else if (alturaSol < 0.1f)  // Atardecer/Amanecer
	{
		// Color naranja/rojizo
		GLfloat factor = (alturaSol + 0.1f) / 0.2f;  // 0 a 1
		color = glm::vec3(1.0f, 0.6f - factor * 0.3f, 0.2f);
		ambientIntensity = 0.3f - factor * 0.1f;
		diffuseIntensity = 0.5f - factor * 0.2f;
	}
	else  // Noche (sol abajo)
	{
		color = glm::vec3(0.5f, 0.5f, 0.7f);
		ambientIntensity = 0.5f;
		diffuseIntensity = 0.6f;
	}
}

glm::vec3 DirectionalLight::getDirection()
{
	return direction;
}

DirectionalLight::~DirectionalLight()
{}

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
									GLfloat aIntensity, GLfloat dIntensity,
									GLfloat xDir, GLfloat yDir, GLfloat zDir) : Light(red, green, blue, aIntensity, dIntensity)
{
	direction = glm::vec3(xDir, yDir, zDir);
	cicloDyN = 0.0f;  // Inicializar tiempo del ciclo
}

void DirectionalLight::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientcolorLocation,
	GLfloat diffuseIntensityLocation, GLfloat directionLocation)
{
	glUniform3f(ambientcolorLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}


