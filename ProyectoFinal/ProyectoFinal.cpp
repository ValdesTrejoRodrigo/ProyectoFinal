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
Model Mario64Cuerpo;
Model Mario64Cabeza;
Model Mario64BrazoDer;
Model Mario64BrazoIzq;
Model Mario64AntebrazoDer;
Model Mario64AntebrazoIzq;
Model Mario64PiernaDer;
Model Mario64MusloDer;
Model Mario64PieDer;
Model Mario64PiernaIzq;
Model Mario64MusloIzq;
Model Mario64PieIzq;

//elementos del entorno
Model excalibur;
Model piedra;//piedra donde esta incrustada excalibur
Model roca; // roca detras de la espada en la piedra
Model robotLampara; //sentado en la roca

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
/*
Model Locomotora;
Model Vagon;
Model RuedaLocomotora;
*/
// Variables para animación del dirigible
float dirigibleTime = 0.0f;
glm::vec3 posicionDirigible(130.0f, 90.0f, -130.0f); // Posición central del recorrido
float rotYDirigible = 180.0f;
float inclinacionDirigible = 0.0f;
float rotColaDirigible = 0.0f;

//motocicleta
Model CuerpoMoto;
Model LlantaDelanteraMoto;
Model LlantaTraseraMoto;

// Variables para animación de la motocicleta
float motoTime = 0.0f;
// Definimos el ancla o centro de la animación
glm::vec3 centroRecorridoMoto(-20.0f, -1.0f, -185.0f);
glm::vec3 motoPosition; // La función se encargará de darle valores a esto
float motoRotationY = 0.0f;
float anguloInclinacionMoto = 0.0f;
float rotLlantasMoto = 0.0f;

//estrcuturas
Model Molino;
Model AspaMolino;


Model Iglesia;
Model EngranajesIglesia;

//EDIFICIOS
Model EdificioGrande;
Model Edificio2;
Model Edificio3;
Model Edificio4;

Model fabrica;

Model RocasFlotantes;
Model Brazo1CasaMobileSteampunk;
Model Brazo2CasaMobileSteampunk;
Model CasaMobileSteampunk;

// Variables para la Casa Mobile Steampunk
bool animarCasa = false;
bool presionoQ = false;
float progresoCasa = 0.0f;
int direccionCasa = 1;
float velocidadCasa = 0.05f;
int faseCasa = 0; 
// Variables temporales para aplicar en los modelos
float angBrazo1, angBrazo2, angCasa, t;

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

std::vector<glm::vec3> posicionArboles2 = {
	//arboles iglesia
	glm::vec3(116.0f, 0.0f, 66.0f),
	glm::vec3(116.0f, 0.0f, 76.0f),
	glm::vec3(116.0f, 0.0f, 86.0f),
	glm::vec3(96.0f, 0.0f, 66.0f),
	glm::vec3(96.0f, 0.0f, 76.0f),
	glm::vec3(96.0f, 0.0f, 86.0f),

};


std::vector<ObjetoEscena> bancoPositions = {
	{glm::vec3(-110.0f, -1.5f, 157.0f), 0.0f},
	{glm::vec3(-110.0f, -1.5f, 113.0f), 180.0f},
};

//edicios iguales al molino pero con diferentes posiciones y rotaciones para dar variedad al escenario

std::vector<ObjetoEscena> posicionesMolinos = {
	{glm::vec3(-205.0f, -2.0f, -240.0f), 180.0f}, // molino original
	{glm::vec3(-220.0f, -2.0f, -220.0f), 180.0f}, // Molino 2
	{glm::vec3(-190.0f, -2.0f, -220.0f), 180.0f},  // Molino 3 
	{glm::vec3(-235.0f, -2.0f, -250.0f), 180.0f},   // Molino 4
	{glm::vec3(-250.0f,  -2.0f, -240.0f), 180.0f}  // Molino 5
};

std::vector<ObjetoEscena> posicionesFabricas = {
	{glm::vec3(260.0f, 12.0f, -270.0f), 180.0f}, 
	{glm::vec3(260.0f, 12.0f, -190.0f), 180.0f}, 
	{glm::vec3(195.0f, 12.0f, -270.0f), 180.0f}, 
	{glm::vec3(195.0f, 12.0f, -190.0f), 180.0f},   
	{glm::vec3(130.0f, 12.0f, -270.0f), 180.0f},
	{glm::vec3(130.0f, 12.0f, -190.0f), 180.0f},
	{glm::vec3(65.0f, 12.0f, -270.0f), 180.0f},
	{glm::vec3(65.0f, 12.0f, -190.0f), 180.0f},
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
	float tamañoRecorrido = 50.0f; // Tamaño de la trayectoria
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

// Añadimos 'centroPista' a los parámetros
void animacionMoto(float deltaTime, glm::vec3& position, glm::vec3 centroPista, float& rotationY, float& anguloInclinacion, float& wheelRotation, float& timeAccum)
{
	// Actualizar tiempo
	timeAccum += deltaTime / 16;

	// Dimensiones del cuadrado
	float longitudLado = 60.0f;
	float velocidadMoto = 8.0f;

	// Perímetro total del cuadrado
	float perimetro = 4.0f * longitudLado;
	float distanciaMoto = fmod(timeAccum * velocidadMoto, perimetro);

	wheelRotation = distanciaMoto * 10.0f;
	bool hacerCaballito = false;

	if (distanciaMoto < longitudLado)  // Lado 1 
	{
		// SUMAMOS EL CENTRO AQUÍ
		position.x = centroPista.x + (-longitudLado / 2 + distanciaMoto);
		position.z = centroPista.z + (-longitudLado / 2);
		rotationY = 90.0f;

		if (distanciaMoto > longitudLado * 0.4f && distanciaMoto < longitudLado * 0.6f)
			hacerCaballito = true;
	}
	else if (distanciaMoto < 2 * longitudLado)  // Lado 2 
	{
		position.x = centroPista.x + (longitudLado / 2);
		position.z = centroPista.z + (-longitudLado / 2 + (distanciaMoto - longitudLado));
		rotationY = 0.0f;
	}
	else if (distanciaMoto < 3 * longitudLado)  // Lado 3 
	{
		position.x = centroPista.x + (longitudLado / 2 - (distanciaMoto - 2 * longitudLado));
		position.z = centroPista.z + (longitudLado / 2);
		rotationY = 270.0f;

		if (distanciaMoto > (2 * longitudLado + longitudLado * 0.4f) && distanciaMoto < (2 * longitudLado + longitudLado * 0.6f))
			hacerCaballito = true;
	}
	else  // Lado 4 
	{
		position.x = centroPista.x + (-longitudLado / 2);
		position.z = centroPista.z + (longitudLado / 2 - (distanciaMoto - 3 * longitudLado));
		rotationY = 180.0f;
	}

	// Altura base sobre el suelo (tomando en cuenta la altura del centro de la pista)
	position.y = centroPista.y;

	// Animación del caballito
	if (hacerCaballito)
	{
		float caballitoProgress = sin((distanciaMoto - floor(distanciaMoto / longitudLado) * longitudLado) * 10.0f);
		anguloInclinacion = -25.0f * abs(caballitoProgress);
		// Sumamos el levantamiento a la altura base
		position.y = centroPista.y + (abs(caballitoProgress) * 0.5f);
	}
	else
	{
		anguloInclinacion = anguloInclinacion * 0.9f;
		if (abs(anguloInclinacion) < 0.1f) anguloInclinacion = 0.0f;
	}
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
	Mario64Cuerpo = Model();
	Mario64Cuerpo.LoadModel("Models/NPC/PersonajeMario64/Mario64Cuerpo.obj");
	Mario64Cabeza = Model();
	Mario64Cabeza.LoadModel("Models/NPC/PersonajeMario64/Mario64Cabeza.obj");
	Mario64BrazoDer = Model();
	Mario64BrazoDer.LoadModel("Models/NPC/PersonajeMario64/Mario64BrazoDer.obj");
	Mario64BrazoIzq = Model();
	Mario64BrazoIzq.LoadModel("Models/NPC/PersonajeMario64/Mario64BrazoIzq.obj");
	Mario64AntebrazoDer = Model();
	Mario64AntebrazoDer.LoadModel("Models/NPC/PersonajeMario64/Mario64AntebrazoDer.obj");
	Mario64AntebrazoIzq = Model();
	Mario64AntebrazoIzq.LoadModel("Models/NPC/PersonajeMario64/Mario64AntebrazoIzq.obj");
	Mario64PiernaDer = Model();
	Mario64PiernaDer.LoadModel("Models/NPC/PersonajeMario64/Mario64PiernaDer.obj");
	Mario64MusloDer = Model();
	Mario64MusloDer.LoadModel("Models/NPC/PersonajeMario64/Mario64MusloDer.obj");
	Mario64PieDer = Model();
	Mario64PieDer.LoadModel("Models/NPC/PersonajeMario64/Mario64PieDer.obj");
	Mario64PiernaIzq = Model();
	Mario64PiernaIzq.LoadModel("Models/NPC/PersonajeMario64/Mario64PiernaIzq.obj");
	Mario64MusloIzq = Model();
	Mario64MusloIzq.LoadModel("Models/NPC/PersonajeMario64/Mario64MusloIzq.obj");
	Mario64PieIzq = Model();
	Mario64PieIzq.LoadModel("Models/NPC/PersonajeMario64/Mario64PieIzq.obj");


	//elementos del entorno

	//modelo de la espada en la piedra
	excalibur = Model();
	excalibur.LoadModel("Models/excalibur.obj");
	piedra = Model();
	piedra.LoadModel("Models/piedra.obj");
	roca = Model();
	roca.LoadModel("Models/Roca.obj");
	robotLampara = Model();
	robotLampara.LoadModel("Models/RobotLampara.obj");

	//EDIFICIOS
	EdificioGrande = Model();
	EdificioGrande.LoadModel("Models/EdificioGrande.obj");
	Edificio2 = Model();
	Edificio2.LoadModel("Models/Edificio2.obj");
	Edificio3 = Model();
	Edificio3.LoadModel("Models/Edificio3.obj");
	Edificio4 = Model();
	Edificio4.LoadModel("Models/Edificio4.obj");

	RocasFlotantes = Model();
	RocasFlotantes.LoadModel("Models/RocasFlotantes.obj");
	Brazo1CasaMobileSteampunk = Model();
	Brazo1CasaMobileSteampunk.LoadModel("Models/Brazo1CasaMobileSteampunk.obj");
	Brazo2CasaMobileSteampunk = Model();
	Brazo2CasaMobileSteampunk.LoadModel("Models/Brazo2CasaMobileSteampunk.obj");	
	CasaMobileSteampunk = Model();
	CasaMobileSteampunk.LoadModel("Models/CasaMobileSteampunk.obj");

	fabrica = Model();
	fabrica.LoadModel("Models/Fabrica.obj");

	Molino = Model();
	Molino.LoadModel("Models/Molino.obj");
	AspaMolino = Model();
	AspaMolino.LoadModel("Models/AspaMolino.obj");

	Iglesia = Model();
	Iglesia.LoadModel("Models/Iglesia.obj");
	EngranajesIglesia = Model();
	EngranajesIglesia.LoadModel("Models/EngranajesIglesia.obj");

	//decoraciones
	Farola = Model();
	Farola.LoadModel("Models/Farola.obj");
	Arbol1 = Model();
	Arbol1.LoadModel("Models/ArbolLowPoly.obj");
	Arbol2 = Model();
	Arbol2.LoadModel("Models/Abeto.obj");
	Bancos = Model();
	Bancos.LoadModel("Models/BancosSentar.obj");
	/*
	BotesBasura = Model();
	BotesBasura.LoadModel("Models/BotesBasura.obj");
	*/

	//vehiculos
	Dirigible = Model();
	Dirigible.LoadModel("Models/DirigibleSteampunk.obj");
	AspaDirigible = Model();
	AspaDirigible.LoadModel("Models/AspaDirigible.obj");
	ColaDirigible = Model();
	ColaDirigible.LoadModel("Models/ColaDirigible.obj");

	CuerpoMoto = Model();
	CuerpoMoto.LoadModel("Models/CuerpoMoto.obj");
	LlantaDelanteraMoto = Model();
	LlantaDelanteraMoto.LoadModel("Models/LlantaDelanteraMoto.obj");
	LlantaTraseraMoto = Model();
	LlantaTraseraMoto.LoadModel("Models/LlantaTraseraMoto.obj");


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
		106.0f, 10.0f, 76.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0031f, 0.0031f,
		20.0f);
	spotLightCount++;


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);


	glm::vec3 lowerLight(0.0f, 0.0f, 0.0f);

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelaux2(1.0);
	glm::mat4 modelaux3(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		angulovaria += 0.5f * deltaTime;
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

		// Actualizar animación de la motocicleta
		animacionMoto(deltaTime, motoPosition, centroRecorridoMoto, motoRotationY, anguloInclinacionMoto, rotLlantasMoto, motoTime);

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
			camera.setCameraMode(3); // Modo cámara fija 1
		}
		if (mainWindow.getsKeys()[GLFW_KEY_4])
		{
			camera.setCameraMode(4); // Modo cámara fija 2
		}
		if (mainWindow.getsKeys()[GLFW_KEY_5])
		{
			camera.setCameraMode(5); // Modo cámara fija 3
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
		model = glm::mat4(1.0);
		modelaux = glm::mat4(1.0);
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

		//Mario (NPC)
		//El cuerpo es el modelo padre del resto de las partes del cuerpo.
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Mario64Cuerpo.RenderModel();
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Mario64Cabeza.RenderModel();
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.45f, 2.0f, 0.0f));
		modelaux3 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Mario64BrazoIzq.RenderModel();
		model = modelaux3;
		model = glm::translate(model, glm::vec3(0.42f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Mario64AntebrazoIzq.RenderModel();
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-0.45f, 2.0f, 0.0f));
		modelaux3 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Mario64BrazoDer.RenderModel();
		model = modelaux3;
		model = glm::translate(model, glm::vec3(-0.42f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Mario64AntebrazoDer.RenderModel();
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.28f, 1.3f, 0.0f));
		modelaux3 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Mario64PiernaIzq.RenderModel();
		model = modelaux3;
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
		modelaux3 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Mario64MusloIzq.RenderModel();
		model = modelaux3;
		model = glm::translate(model, glm::vec3(0.0f, -0.55f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Mario64PieIzq.RenderModel();

		model = modelaux2;
		model = glm::translate(model, glm::vec3(-0.28f, 1.3f, 0.0f));
		modelaux3 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Mario64PiernaDer.RenderModel();
		model = modelaux3;
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
		modelaux3 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Mario64MusloDer.RenderModel();
		model = modelaux3;
		model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Mario64PieDer.RenderModel();


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
		model = glm::translate(model, glm::vec3(106.0f, excaliburY, 76.0f)); // Usar altura animada
		model = glm::rotate(model, glm::radians(rotExcalibur), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotar en Y
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		excalibur.RenderModel();

		//piedra en la que esta clavada (sin cambios)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(106.0f, -1.53f, 76.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		piedra.RenderModel();
		//roca detras de la piedra para dar sensación de profundidad al punto de interés
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(106.0f, 0.0f, 83.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		roca.RenderModel();
		//robot sentado en la roca
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(106.0f, 5.5f, 82.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		robotLampara.RenderModel();

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

		//modelo de motocicleta
		model = glm::mat4(1.0f);
		model = glm::translate(model, motoPosition); // Usar posición animada (que ya tiene el offset)
		model = glm::rotate(model, glm::radians(motoRotationY), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(anguloInclinacionMoto), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		CuerpoMoto.RenderModel();

		// Llanta delantera izquierda con rotación de avance
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.675f, -0.55f, -2.6f));
		model = glm::rotate(model, glm::radians(rotLlantasMoto), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotación de la llanta
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		LlantaDelanteraMoto.RenderModel();

		// Llanta delantera derecha con rotación de avance
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.675f, -0.55f, -2.6f));
		model = glm::rotate(model, glm::radians(rotLlantasMoto), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotación de la llanta
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		LlantaDelanteraMoto.RenderModel();

		// Llanta trasera izquierda con rotación de avance
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -0.6f, 1.1f));
		model = glm::rotate(model, glm::radians(rotLlantasMoto), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotación de la llanta
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		LlantaTraseraMoto.RenderModel();

		// Llanta trasera derecha con rotación de avance
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -0.6f, 2.55f));
		model = glm::rotate(model, glm::radians(rotLlantasMoto), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotación de la llanta
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		LlantaTraseraMoto.RenderModel();
		//botes de basura

		//ESTRUCTURAS

		//Edificio grande
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, 5.0f, -100.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		EdificioGrande.RenderModel();

		//Edificio 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.0f, 7.0f, -100.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Edificio2.RenderModel();

		//Edificio 3
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, 16.0f, -185.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Edificio3.RenderModel();

		//Edificio 4
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(176.0f, -2.0f, 50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Edificio4.RenderModel();

		//fabricas
		for (const auto& molino : posicionesFabricas) {
			// 1. Renderizar la base del Molino
			model = glm::mat4(1.0f);
			model = glm::translate(model, molino.posicion);
			model = glm::rotate(model, glm::radians(molino.rotacionY), glm::vec3(0.0f, 1.0f, 0.0f));
			modelaux = model;
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
			fabrica.RenderModel();
		}

		//animacion por teclado para el movimiento de los brazo y la casa
		if (mainWindow.getsKeys()[GLFW_KEY_Q])
		{
			if (!presionoQ) // Solo cambia el estado una vez por pulsación
			{
				animarCasa = !animarCasa;
				presionoQ = true;
			}
		}
		else
		{
			presionoQ = false;
		}
		if (animarCasa)
		{
			progresoCasa += velocidadCasa * (deltaTime / 2) * direccionCasa;

			// FASE 0: Del centro a la derecha
			if (direccionCasa == 1 && progresoCasa >= 1.0f)
			{
				progresoCasa = 1.0f;
				direccionCasa = -1; // Cambiamos dirección
				faseCasa = 1;       // Pasamos a la siguiente fase
			}
			// FASE 1: De la derecha a la izquierda (cruzando el centro)
			else if (direccionCasa == -1 && progresoCasa <= -1.0f)
			{
				progresoCasa = -1.0f;
				direccionCasa = 1; // Cambiamos dirección para volver
				faseCasa = 2;      // Pasamos a la fase final
			}
			// FASE 2: De la izquierda de regreso al centro EXACTO
			else if (faseCasa == 2 && direccionCasa == 1 && progresoCasa >= 0.0f)
			{
				progresoCasa = 0.0f; // La fijamos exactamente en el centro (posición inicial)
				animarCasa = false;
				faseCasa = 0;        // Lista para la próxima vez que presiones Q
			}
		
		}

		if (progresoCasa < 0.0f) // Lado Izquierdo (de -1 a 0)
		{
			//donde esta, a donde va y regresa a donde esta
			t = progresoCasa + 1.0f; // Normalizamos de 0 a 1
			angBrazo1 = 0.0f + (45.0f - 0.0f) * t;
			angBrazo2 = -90.0f + (-45.0f - (-90.0f)) * t;
			angCasa = 90.0f + (0.0f - 90.0f) * t;
		}
		else // Lado Derecho (de 0 a 1)
		{
			//       inicial, derecha , regreso al centro
			t = progresoCasa; // Ya está de 0 a 1
			angBrazo1 = 45.0f + (135.0f - 45.0f) * t;
			angBrazo2 = -45.0f + (-90.0f - (-45.0f)) * t; // Asumí 0.0f para extremo derecho
			angCasa = 0.0f + (-45.0f - 0.0f) * t;
		}

		//Casa mobile Steampunk
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 40.0f, 220.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		RocasFlotantes.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.0f, 6.0f, -4.0f));
		model = glm::rotate(model, glm::radians(angBrazo1), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Brazo1CasaMobileSteampunk.RenderModel();

		model = modelaux2;
		model = glm::translate(model, glm::vec3(11.0f, -1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(angBrazo2), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux3 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Brazo2CasaMobileSteampunk.RenderModel();

		model = modelaux3;
		model = glm::translate(model, glm::vec3(-6.0f, 7.5f, -1.0f));
		model = glm::rotate(model, glm::radians(angCasa), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		CasaMobileSteampunk.RenderModel();

		//molinos, renderizados en un loop para aplicar animación a las aspas de cada molino
		for (const auto& molino : posicionesMolinos) {
			// 1. Renderizar la base del Molino
			model = glm::mat4(1.0f);
			model = glm::translate(model, molino.posicion);
			model = glm::rotate(model, glm::radians(molino.rotacionY), glm::vec3(0.0f, 1.0f, 0.0f));
			modelaux = model;
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
			Molino.RenderModel();

			// 2. Renderizar las Aspas animadas
			model = modelaux;
			model = glm::translate(model, glm::vec3(-4.6f, 12.0f, 0.0f));
			model = glm::rotate(model, glm::radians(angulovaria), glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
			AspaMolino.RenderModel();
		}

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
		
		//Bancos
		for (const auto& banco : bancoPositions) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, banco.posicion);
			model = glm::rotate(model, glm::radians(banco.rotacionY), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
			Bancos.RenderModel();
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//arbolesAbeto
		for (const auto& pos : posicionArboles2) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, pos);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
			Arbol2.RenderModel();
		}





		
		
		glDisable(GL_BLEND);
		
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
