//Proyecto Final de Computacion Grafica e Interaccion Humano Computadora
//iNTEGRANTES:
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animación
float angulovaria = 0.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;

// Variables para control de farolas
int numFarolasActivas = 0; // Número de farolas actualmente encendidas
bool farolasEncendidas = false; // Estado de las farolas

//personajes
//Avtara ligado a la camara
Model Cuerpo;
Model BraDer;
Model BraIzq;
Model PiernaDer;
Model PiernaIzq;

//NPC
Model Sherlock;
Model Waver;
Model Reines;


//elementos del entorno
Model excalibur;
Model piedra;

//Decoraciones
Model Farola;
Model Arbol1; //low poly
Model Arbol2; //abeto?
Model Bancos;
Model BotesBasura;


// Variables para animación de Excalibur
bool excaliburSacada = false;
float excaliburTiempoAnimado = 0.0f;
float excaliburY = 1.0f; // Altura inicial
float rotExcalibur = 0.0f; // Rotación en Y
float animDuration = 8.0f;// Duración total de la animación: 8 segundos
float progreso = 0.0f;
float giroEspada = 0.0f;

//vehiculos
Model Dirigible;
Model AspaDirigible;
Model ColaDirigible;

Model Locomotora;
Model Vagon;
Model RuedaLocomotora;

// Variables para animación del dirigible
float dirigibleTime = 0.0f;
glm::vec3 posicionDirigible(130.0f, 70.0f, -130.0f); // Posición central del recorrido
float rotYDirigible = 180.0f;
float inclinacionDirigible = 0.0f;
float rotColaDirigible = 0.0f;



//estrcuturas
Model Molino;
Model AspaMolino;


Model Iglesia;
Model EngranajesIglesia;


Skybox skyboxDia;
Skybox skyboxNoche;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";



//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};


	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

}
struct ObjetoEscena {
	glm::vec3 posicion;
	float rotacionY; // Ángulo en grados
};

//arreglos de posiciones para las decoraciones (farolas, los arboles...)
std::vector<glm::vec3> posicionFarolas = {
	glm::vec3(-80.0f, 2.5f, 156.0f),
	glm::vec3(-80.0f, 2.5f, 112.0f),
	glm::vec3(10.0f, 0.0f, -10.0f)
};

std::vector<glm::vec3> posicionArboles = {
	//arboles iglesia
	glm::vec3(-90.0f, 4.0f, 166.0f),
	glm::vec3(-110.0f, 4.0f, 166.0f),
	glm::vec3(-130.0f, 4.0f, 166.0f),
	glm::vec3(-150.0f, 4.0f, 166.0f),
	glm::vec3(-90.0f, 4.0f, 105.0f),
	glm::vec3(-110.0f, 4.0f, 105.0f),
	glm::vec3(-130.0f, 4.05f, 105.0f),
	glm::vec3(-150.0f, 4.0f, 105.0f),

};


std::vector<ObjetoEscena> bancoPositions = {
	{glm::vec3(-110.0f, -1.5f, 157.0f), 0.0f},
	{glm::vec3(-110.0f, -1.5f, 113.0f), 180.0f},
};

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

// Función de animación compleja del barco volador
void animacionDirigible(float deltaTime, glm::vec3& posicionBase, float& rotacionY, float& timeAccum)
{
	// Actualizar tiempo
	timeAccum += deltaTime/16;

	// Movimiento en forma de 8 (lemniscata) en el plano XZ
	float tamañoRecorrido = 30.0f; // Tamaño de la trayectoria
	float velocidad = 0.2f;  // Velocidad de recorrido

	float t = timeAccum * velocidad;

	// Ecuación paramétrica de la lemniscata (figura de 8)
	float denominador = 1.0f + sin(t) * sin(t);
	float offsetX = tamañoRecorrido * cos(t) / denominador;
	float offsetZ = tamañoRecorrido * sin(t) * cos(t) / denominador;

	// Guardar la posición base
	static glm::vec3 posicionCentral = posicionBase; // Se guarda solo la primera vez

	// Aplicar offset a la posición central
	posicionBase.x = posicionCentral.x + offsetX;
	posicionBase.z = posicionCentral.z + offsetZ;

	// Movimiento vertical ondulante (sube y baja suavemente) desde la altura base
	posicionBase.y = posicionCentral.y + sin(timeAccum * 0.5f) * 2.0f;

	// Calcular rotación para que apunte hacia la dirección de movimiento
	float dx = -tamañoRecorrido * sin(t) / denominador;
	float dz = tamañoRecorrido * (cos(t) * cos(t) - sin(t) * sin(t)) / denominador;

	rotacionY = glm::degrees(atan2(dx, dz));
}


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	

	//avatar ligado a la camara
	Cuerpo = Model();
	Cuerpo.LoadModel("Models/PersonajeGray/GrayCuerpo.obj");
	BraDer = Model();
	BraDer.LoadModel("Models/PersonajeGray/BrazoDerGray.obj");
	BraIzq = Model();
	BraIzq.LoadModel("Models/PersonajeGray/BrazoIzqGray.obj");
	PiernaDer = Model();
	PiernaDer.LoadModel("Models/PersonajeGray/PiernaDerGray.obj");
	PiernaIzq = Model();
	PiernaIzq.LoadModel("Models/PersonajeGray/PiernaIzqGray.obj");

	//NPC
	Sherlock = Model();
	Sherlock.LoadModel("Models/NPC/SherlockFGO.obj");
	Waver = Model();
	Waver.LoadModel("Models/NPC/WaverFGO.obj");
	Reines = Model();
	Reines.LoadModel("Models/NPC/ReinesFGO.obj");

	//elementos del entorno

	//modelo de la espada en la piedra
	excalibur = Model();
	excalibur.LoadModel("Models/excalibur.obj");
	piedra = Model();
	piedra.LoadModel("Models/piedra.obj");

	//decoraciones
	Farola = Model();
	Farola.LoadModel("Models/Farola.obj");
	Arbol1 = Model();
	Arbol1.LoadModel("Models/ArbolLowPoly.obj");
	Arbol2 = Model();
	Arbol2.LoadModel("Models/Abeto.obj");
	Bancos = Model();
	Bancos.LoadModel("Models/BancosSentar.obj");
	BotesBasura = Model();
	BotesBasura.LoadModel("Models/BotesBasura.obj");


	//vehiculos
	Dirigible = Model();
	Dirigible.LoadModel("Models/DirigibleSteampunk.obj");
	AspaDirigible = Model();
	AspaDirigible.LoadModel("Models/AspaDirigible.obj");
	ColaDirigible = Model();
	ColaDirigible.LoadModel("Models/ColaDirigible.obj");


	//estructuras
	Molino = Model();
	Molino.LoadModel("Models/Molino.obj");
	AspaMolino = Model();
	AspaMolino.LoadModel("Models/AspaMolino.obj");

	Iglesia = Model();
	Iglesia.LoadModel("Models/Iglesia.obj");
	EngranajesIglesia = Model();
	EngranajesIglesia.LoadModel("Models/EngranajesIglesia.obj");


	
	std::vector<std::string> skyboxFacesDia;
	skyboxFacesDia.push_back("Textures/Skybox/Monte_right.jpeg");
	skyboxFacesDia.push_back("Textures/Skybox/Monte_left.jpeg");
	skyboxFacesDia.push_back("Textures/Skybox/Monte_down.jpeg");
	skyboxFacesDia.push_back("Textures/Skybox/Monte_up.jpeg");
	skyboxFacesDia.push_back("Textures/Skybox/Monte_back.jpeg");
	skyboxFacesDia.push_back("Textures/Skybox/Monte_front.jpeg");
	skyboxDia = Skybox(skyboxFacesDia);

	std::vector<std::string> skyboxFacesNoche;
	skyboxFacesNoche.push_back("Textures/Skybox/MonteN_right.jpeg");
	skyboxFacesNoche.push_back("Textures/Skybox/MonteN_left.jpeg");
	skyboxFacesNoche.push_back("Textures/Skybox/MonteN_down.jpeg");
	skyboxFacesNoche.push_back("Textures/Skybox/MonteN_up.jpeg");
	skyboxFacesNoche.push_back("Textures/Skybox/MonteN_back.jpeg");
	skyboxFacesNoche.push_back("Textures/Skybox/MonteN_front.jpeg");
	skyboxNoche = Skybox(skyboxFacesNoche);
	

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, -1.0f, 0.0f);

	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
// Farola 1
	pointLights[pointLightCount] = PointLight(1.0f, 0.6f, 0.2f,  // Color naranja cálido
		0.5f, 2.5f,  // Intensidad ambiental y difusa AUMENTADAS
		-80.0f, 7.0f, 156.0f,  // Posición
		1.0f, 0.022f, 0.0019f);  // Atenuación REDUCIDA (mayor alcance)

	// Farola 2
	pointLights[pointLightCount + 1] = PointLight(1.0f, 0.6f, 0.2f,
		0.5f, 2.5f,
		-80.0f, 7.0f, 112.0f,
		1.0f, 0.022f, 0.0019f);

	// Farola 3
	pointLights[pointLightCount + 2] = PointLight(1.0f, 0.6f, 0.2f,
		0.5f, 2.5f,
		10.0f, 3.0f, -10.0f,
		1.0f, 0.022f, 0.0019f);

	unsigned int baseLightCount = pointLightCount;
	numFarolasActivas = 4; // Número total de farolas


	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;
	//luz sobre la espada
	spotLights[1] = SpotLight(1.0f, 1.0f, 0.0f,
		0.0f, 2.0f,
		36.0f, 10.0f, 36.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0031f, 0.0031f,
		20.0f);
	spotLightCount++;


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	

	glm::vec3 lowerLight(0.0f,0.0f,0.0f);

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelaux2(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		angulovaria += 0.5f*deltaTime;
		// Actualizar ciclo día/noche
		mainLight.UpdateCycle(deltaTime);

		GLfloat sunHeight = mainLight.getDirection().y; // Obtener altura del sol

		if (sunHeight > 0.0f && !farolasEncendidas) // Es de noche y farolas apagadas
		{
			// Encender farolas
			farolasEncendidas = true;
			pointLightCount = baseLightCount + numFarolasActivas;
		}
		else if (sunHeight <= 0.0f && farolasEncendidas) // Es de día y farolas encendidas
		{
			// Apagar farolas
			farolasEncendidas = false;
			pointLightCount = baseLightCount;
		}


		// Animar el dirigible
		animacionDirigible(deltaTime, posicionDirigible, rotYDirigible, dirigibleTime);


		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Cambiar modo de cámara con teclas numéricas
		if (mainWindow.getsKeys()[GLFW_KEY_1])
		{
			camera.setCameraMode(1); // Modo tercera persona
		}
		if (mainWindow.getsKeys()[GLFW_KEY_2])
		{
			camera.setCameraMode(2); // Modo aéreo
		}
		if (mainWindow.getsKeys()[GLFW_KEY_3])
		{
			camera.setCameraMode(3); // Modo cámara fija
		}

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		if (sunHeight > 0.0f) // Noche
		{
			skyboxNoche.DrawSkybox(camera.calculateViewMatrix(), projection);
		}
		else // Día 
		{
			skyboxDia.DrawSkybox(camera.calculateViewMatrix(), projection);
		}

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation(); // para la textura con movimiento

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		//Reinicializando variables cada ciclo de reloj
		model= glm::mat4(1.0);
		modelaux= glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		toffset = glm::vec2(0.0f, 0.0f);
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();


		if (camera.getCameraMode() == 1)
		{
			// ============ AVATAR LIGADO A LA CAMARA ============
			// Se obtiene la posicion del avatar basada en la camara
			glm::vec3 avatarPos = camera.getAvatarPosition();
			float avatarRotation = camera.getAvatarRotation(); // Rotacion basada en direccion de movimiento

			// Obtener tiempo de animacion
			float animTime = camera.getVelocidadAnimacion();
			bool estaCaminando = camera.estaCaminando();

			// Calcular angulos de animacion de brazos y piernas
			float rotExtremidadesGray = 0.0f;
			if (estaCaminando)
			{
				rotExtremidadesGray = sin(animTime) * 30.0f; // Oscilacion de 30 grados
			}

			//Personaje Gray FateGrandOrder
			// Cuerpo Avatar
			model = glm::mat4(1.0);
			model = glm::translate(model, avatarPos);
			model = glm::rotate(model, glm::radians(avatarRotation), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotar segun direccion de movimiento
			modelaux = model;
			model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
			Cuerpo.RenderModel();

			//Brazo derecho (oscila opuesto al brazo izquierdo)
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, 0.75f, 0.105f));
			if (estaCaminando)
			{
				model = glm::rotate(model, glm::radians(rotExtremidadesGray), glm::vec3(0.0f, 0.0f, 1.0f));
			}
			model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
			BraDer.RenderModel();

			//Brazo izquierdo (oscila opuesto al brazo derecho)
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, 0.75f, -0.15f));
			if (estaCaminando)
			{
				model = glm::rotate(model, glm::radians(-rotExtremidadesGray), glm::vec3(1.0f, 0.0f, 1.0f));
			}
			model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
			BraIzq.RenderModel();

			//PiernaDer (oscila opuesto a la pierna izquierda)
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, -0.19f, 0.17f));
			if (estaCaminando)
			{
				model = glm::rotate(model, glm::radians(-rotExtremidadesGray), glm::vec3(0.0f, 0.0f, 1.0f));
			}
			model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
			PiernaDer.RenderModel();

			//PiernaIzq (oscila opuesto a la pierna derecha)
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, -0.19f, -0.22f));
			if (estaCaminando)
			{
				model = glm::rotate(model, glm::radians(rotExtremidadesGray), glm::vec3(0.0f, 0.0f, 1.0f));
			}
			model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
			PiernaIzq.RenderModel();

		}//Fin del if de avatar ligado a la camara


		//NPC
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, 1.0f, 20.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Sherlock.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(25.0f, 1.1f, 25.0f));		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Waver.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.0f, 0.5f, 30.0f));		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reines.RenderModel();

		// Activar/desactivar animación de Excalibur con tecla E
		if (mainWindow.getsKeys()[GLFW_KEY_E])
		{
			if (!excaliburSacada)
			{
				excaliburSacada = true;
				excaliburTiempoAnimado = 0.0f; // Reiniciar animación
			}
		}

		// Actualizar animación de Excalibur si está activa
		if (excaliburSacada)
		{
			excaliburTiempoAnimado += deltaTime/16;

			if (excaliburTiempoAnimado < 2.0f) // Fase 1: Subir (2 segundos)
			{
				// Subir suavemente desde 1.0 hasta 8.0
				progreso = excaliburTiempoAnimado / 2.0f; // 0 a 1
				excaliburY = 1.0f + (progreso * 7.0f); // De 1.0 a 8.0
				rotExcalibur = 0.0f;
			}
			else if (excaliburTiempoAnimado < 6.0f) // Fase 2: Girar en el aire (4 segundos)
			{
				// Mantener altura y girar
				excaliburY = 6.0f;
				giroEspada = excaliburTiempoAnimado - 2.0f; // Tiempo desde que empezó a girar
				rotExcalibur = giroEspada * 180.0f; // 2 vueltas completas en 4 segundos (720 grados)
			}
			else if (excaliburTiempoAnimado < animDuration) // Fase 3: Bajar (2 segundos)
			{
				// Bajar suavemente desde 8.0 hasta 1.0
				float tiempoDescenso = excaliburTiempoAnimado - 6.0f; // 0 a 2
				progreso = tiempoDescenso / 2.0f; // 0 a 1
				excaliburY = 6.0f - (progreso * 7.0f); // De 8.0 a 1.0
				rotExcalibur = 720.0f; // Mantener rotación final
			}
			else // Animación completada
			{
				// Reiniciar animación
				excaliburSacada = false;
				excaliburTiempoAnimado = 0.0f;
				excaliburY = 1.0f;
				rotExcalibur = 0.0f;
			}
		}



		//espada en la piedra punto de interes 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(36.0f, excaliburY, 36.0f)); // Usar altura animada
		model = glm::rotate(model, glm::radians(rotExcalibur), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotar en Y
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		excalibur.RenderModel();

		//piedra en la que esta clavada (sin cambios)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(35.0f, -1.53f, 35.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		piedra.RenderModel();


		//modelo de barco volador
		model = glm::mat4(1.0);
		model = glm::translate(model, posicionDirigible);//para cambiar la posición del dirigible en su recorrido
		model = glm::rotate(model, glm::radians(rotYDirigible+180), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación animada

		// Añadir inclinación en los giros
		inclinacionDirigible = sin(dirigibleTime * 0.6f) * 15.0f;
		model = glm::rotate(model, glm::radians(inclinacionDirigible), glm::vec3(0.0f, 0.0f, 1.0f));

		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Dirigible.RenderModel();

		// Aspa del dirigible DERECHA (con rotación continua)
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.5f, -0.2f, 1.25f));
		model = glm::rotate(model, dirigibleTime * 10.0f, glm::vec3(0.0f, 0.0f, 1.0f)); // Gira rápido
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		AspaDirigible.RenderModel();

		// Aspa del dirigible IZQUIERDA (con rotación continua)
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.5f, -0.2f, 1.25f));
		model = glm::rotate(model, -dirigibleTime * 10.0f, glm::vec3(0.0f, 0.0f, 1.0f)); // Gira rápido en sentido opuesto
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		AspaDirigible.RenderModel();

		// Cola del dirigible (movimiento como pez)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 3.0f));
		// Movimiento de cola tipo pez (oscilación lateral)
		rotColaDirigible = sin(dirigibleTime * 3.0f) * 20.0f; // Oscila ±20 grados
		model = glm::rotate(model, glm::radians(rotColaDirigible), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		ColaDirigible.RenderModel();
/*
		//modelo de locomotora
		model = glm::mat4(1.0);
		model = glm::translate(model, trenPosition);
		model = glm::rotate(model, glm::radians(trenRotationY+90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Locomotora.RenderModel();
/*
		/*
		model = modelaux;
		model = glm::translate(model, trenPosition);
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Vagon.RenderModel();
		*/
		
		
		// Farolas
		for (const auto& pos : posicionFarolas) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, pos);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

			// Usamos el material brillante para todas las farolas
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			Farola.RenderModel();
		}

		//arboleslowPoly
		for (const auto& pos : posicionArboles) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, pos);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
			Arbol1.RenderModel();
		}
		//arbolesAbeto

		//Bancos
		for (const auto& banco : bancoPositions) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, banco.posicion);
			model = glm::rotate(model, glm::radians(banco.rotacionY), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
			Bancos.RenderModel();
		}

		//botes de basura



		//modelo de molinos
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-106.0f, -2.0f, -106.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Molino.RenderModel();

		modelaux = model;
		model = glm::translate(model, glm::vec3(-4.6f, 12.0f, 0.0f));
		model = glm::rotate(model, glm::radians(angulovaria), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		AspaMolino.RenderModel();

		//Ilgesia
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-146.0f, 9.0f, 136.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Iglesia.RenderModel();

		modelaux = model;
		model = glm::translate(model, glm::vec3(3.0f, 8.2f, 2.7f));
		model = glm::rotate(model, glm::radians(angulovaria), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		EngranajesIglesia.RenderModel();

		/*
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_BLEND);
		*/
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
