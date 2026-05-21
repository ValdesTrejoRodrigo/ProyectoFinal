//Proyecto Final de Computacion Grafica e Interaccion Humano Computadora
//iNTEGRANTES:
#define STB_IMAGE_IMPLEMENTATION
//para audio
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

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

//para iluminaci�n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//para audio
ma_engine engine;
ma_sound music; //soundtrack de fondo

//variables para animaci�n
float angulovaria = 0.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;
Texture plain;
Texture pisoTexture;
Texture grassTexture;
Texture waterTexture;

// Variables para control de farolas
int numFarolasActivas = 0; // N�mero de farolas actualmente encendidas
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

Model Estrella;
//Variables para animaci�n de estrella
glm::vec3 posicionEstrella = glm::vec3(0.0f, 0.0f, 0.0f);
float rotacionEstrellaY = 0.0f;
float timeAccumEstrella = 0.0f;

Model Flag;
Model Flagpole;

//Decoraciones
Model Farola;
Model Arbol1; //low poly
Model Arbol2; //abeto?
Model Bancos;

//Reloj
Model Reloj_M;
Model EngranajeGrande_M;
Model EngranajeMediano_M;
Model EngranajePequeno_M;
Model EngranajeEnano_M;
Model ManecillaHora_M;
Model ManecillaMinuto_M;
// Variables para animaci�n del reloj
float posXreloj = -225.0f, posYreloj = 15.0f, posZreloj = 50.0f;
float rotEngranajeGrande = 0.0f;
float rotEngranajeMediano = 0.0f;
float rotEngranajePequeno = 0.0f;
float rotEngranajeEnano = 0.0f;
float rotManecillaHora = 0.0f;
float rotManecillaMinuto = 0.0f;

// Variables para animaci�n de Excalibur
bool excaliburSacada = false;
float excaliburTiempoAnimado = 0.0f;
float excaliburY = 1.0f; // Altura inicial
float rotExcalibur = 0.0f; // Rotaci�n en Y
float animDuration = 8.0f;// Duraci�n total de la animaci�n: 8 segundos
float progreso = 0.0f;
float giroEspada = 0.0f;

//vehiculos
Model Dirigible;
Model AspaDirigible;
Model ColaDirigible;

// Variables para animaci�n del dirigible
float dirigibleTime = 0.0f;
glm::vec3 posicionDirigible(130.0f, 90.0f, -130.0f); // Posici�n central del recorrido
float rotYDirigible = 180.0f;
float inclinacionDirigible = 0.0f;
float rotColaDirigible = 0.0f;

Model Locomotora;
Model Vagon;
/*
Model RuedaGLocomotora;
Model RuedaPLocomotora;
*/

// Variables para animaci�n por keyframes del tren
float movTrenX = 240.0f;  // Movimiento en X
float rotRuedasTren = 0.0f;  // Rotaci�n de las ruedas
float ciclo, ciclo2 = 0.0f; // Variables para controlar el ciclo de animaci�n
#define MAX_FRAMES 100
int i_max_steps = 90;  // Pasos de interpolaci�n entre keyframes
int i_curr_steps = 7;

typedef struct _frameTren
{
	float movTrenX;      // Posici�n X del tren
	float movTrenXInc;   // Incremento en X
} FRAME_TREN;

FRAME_TREN KeyFrameTren[MAX_FRAMES];
int FrameIndexTren = 0;
bool playTren = false;
int playIndexTren = 0;
int	reinicioFrame,guardoFrame = 0;

typedef struct _frameReloj
{
	//Variables para GUARDAR Key Frames del reloj
	float rotEngranajeGrande;
	float rotEngranajeMediano;
	float rotEngranajePequeno;
	float rotEngranajeEnano;
	float rotManecillaHora;
	float rotManecillaMinuto;
	float incRotEngranajeGrande = 0.0f;
	float incRotEngranajeMediano = 0.0f;
	float incRotEngranajePequeno = 0.0f;
	float incRotEngranajeEnano = 0.0f;

} FRAME_RELOJ;

FRAME_RELOJ KeyFrameReloj[MAX_FRAMES];
int FrameIndexReloj = 30;			//introducir datos
bool playReloj = false;
int playIndexReloj = 0;

//motocicleta
Model CuerpoMoto;
Model LlantaDelanteraMoto;
Model LlantaTraseraMoto;

// Variables para animaci�n de la motocicleta
float motoTime = 0.0f;
// Definimos el ancla o centro de la animaci�n
glm::vec3 centroRecorridoMoto(-20.0f, -0.5f, -185.0f);
glm::vec3 motoPosition; // La funci�n se encargar� de darle valores a esto
float motoRotationY = 180.0f;
float anguloInclinacionMoto = 0.0f;
float rotLlantasMoto = 0.0f;

//estrcuturas
Model Molino;
Model AspaMolino;
Model Castillo;

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

Model TunelTren;
Model ViasTren;

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
bool luzEspadaActiva = true;   // toggle con tecla F
bool luzEspadaKeyPressed = false;

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";



//c�lculo del promedio de las normales para sombreado de Phong
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
		0, 2, 1,
		1, 2, 3
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
		-1.0f, 0.0f, -1.0f,		 0.0f, 0.0f,	 0.0f, -1.0f, 0.0f,
		 1.0f, 0.0f, -1.0f,		 1.0f, 0.0f,	 0.0f, -1.0f, 0.0f,
		-1.0f, 0.0f,  1.0f,		 0.0f, 1.0f,	 0.0f, -1.0f, 0.0f,
		 1.0f, 0.0f,  1.0f,		 1.0f, 1.0f,	 0.0f, -1.0f, 0.0f,
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
	obj1->CreateMesh(vertices, indices, 32, 6);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 6);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

}
struct ObjetoEscena {
	glm::vec3 posicion;
	float rotacionY; // �ngulo en grados
};

//arreglos de posiciones para las decoraciones (farolas, los arboles...)
std::vector<glm::vec3> posicionFarolas = {
	glm::vec3(-80.0f, 2.5f, 156.0f),
	glm::vec3(-80.0f, 2.5f, 112.0f),
	glm::vec3(10.0f, 2.5f, -120.0f),
	glm::vec3(145.0f, 2.5f, 30.0f)
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
	{glm::vec3(-110.0f, -1.0f, 157.0f), 0.0f},
	{glm::vec3(-110.0f, -1.0f, 113.0f), 180.0f},
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

std::vector<glm::vec3> posicionesVias = {
	glm::vec3(270.0f, 0.0f, -15.0f),
	glm::vec3(210.0f, 0.0f, -15.0f),
	glm::vec3(150.0f, 0.0f, -15.0f),
	glm::vec3(90.0f, 0.0f, -15.0f),
	glm::vec3(30.0f, 0.0f, -15.0f),
	glm::vec3(-30.0f, 0.0f, -15.0f),
	glm::vec3(-90.0f, 0.0f, -15.0f),
	glm::vec3(-150.0f, 0.0f, -15.0f),
	glm::vec3(-210.0f, 0.0f, -15.0f),
	glm::vec3(-270.0f, 0.0f, -15.0f),
};
void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

// Funci�n de animaci�n compleja del barco volador
void animacionDirigible(float deltaTime, glm::vec3& posicionBase, float& rotacionY,
	float& inclinacion, float& timeAccum) {
	timeAccum += deltaTime / 16;

	float tamanoRecorrido = 50.0f;
	float velocidad = 0.2f;
	float t = timeAccum * velocidad;

	// Guardar posici�n central SOLO LA PRIMERA VEZ
	static bool primeraVez = true;
	static glm::vec3 posicionCentral;
	if (primeraVez) {
		posicionCentral = posicionBase;
		primeraVez = false;
	}

	// Lemniscata (figura de 8)
	float denominador = 1.0f + sin(t) * sin(t);
	float offsetX = tamanoRecorrido * cos(t) / denominador;
	float offsetZ = tamanoRecorrido * sin(t) * cos(t) / denominador;

	// Aplicar offsets a la posici�n central
	posicionBase.x = posicionCentral.x + offsetX;
	posicionBase.z = posicionCentral.z + offsetZ;

	// Altura que depende de la posici�n en la curva
	float distanciaDelCentro = sqrt(offsetX * offsetX + offsetZ * offsetZ);
	posicionBase.y = posicionCentral.y + sin(t) * 3.0f + (distanciaDelCentro / tamanoRecorrido) * 2.0f;

	// Calcular rotaci�n
	float dx = -tamanoRecorrido * sin(t) / denominador;
	float dz = tamanoRecorrido * (cos(t) * cos(t) - sin(t) * sin(t)) / denominador;
	rotacionY = glm::degrees(atan2(dx, dz));

	//Inclinacion del dirigible basada en la curvatura de la trayectoria para un efecto m�s realista
	float curvatura = dx * dz; // Producto cruzado simplificado
	inclinacion = curvatura * 0.05f; // Inclinaci�n proporcional
}

void animacionMoto(float deltaTime, glm::vec3& position, glm::vec3 centroPista,float& rotationY, float& anguloInclinacion, float& wheelRotation,float& timeAccum) {
	timeAccum += deltaTime / 32;

	// RECORRIDO CIRCULAR
	float radio = 30.0f;
	float velocidadAngular = 0.5f;
	float angulo = timeAccum * velocidadAngular;

	// Posici�n en c�rculo
	position.x = centroPista.x + radio * cos(angulo);
	position.z = centroPista.z + radio * sin(angulo);
	position.y = centroPista.y;

	// Rotaci�n tangente a la curva
	rotationY = glm::degrees(angulo) - 90.0f;

	// Rotaci�n de ruedas
	wheelRotation += velocidadAngular * radio * 10.0f;

	// Caballito en puntos espec�ficos (cada PI radianes)
	float moduloAngulo = fmod(angulo, 3.14159f);
	bool hacerCaballito = (moduloAngulo < 0.5f); // Durante el primer cuarto de cada media vuelta

	if (hacerCaballito) {
		float progreso = moduloAngulo / 0.5f; // 0 a 1

		// Salto parab�lico
		position.y = centroPista.y + sin(progreso * 3.14159f) * 2.0f;

		// Inclinaci�n hacia ATR�S (eje X negativo)
		anguloInclinacion = -45.0f * sin(progreso * 3.14159f);
	}
	else {
		// Volver suavemente a posici�n normal
		anguloInclinacion *= 0.9f;
		if (abs(anguloInclinacion) < 0.1f) anguloInclinacion = 0.0f;
	}
}

void animacionEstrella(float deltaTime, glm::vec3& posicionBase, float& rotacionY, float& timeAccum) {
	// Actualizar el tiempo general
	timeAccum += deltaTime;

	float radioMaximo = 2.0f;     // Distancia al centro cuando est� en la parte m�s baja
	float alturaMaxima = 6.0f;    // Cu�nto sube la estrella en el eje Y
	float velocidadVertical = 0.005f;// Velocidad de la subida y bajada
	float velocidadOrbita = 0.05f;  // Velocidad con la que da vueltas alrededor del centro
	float velocidadGiro = 5.0f;  // Velocidad a la que rota sobre su propio centro

	// Guardar la posici�n central
	static glm::vec3 posicionCentral = posicionBase;

	// Movimiento vertical
	float factorAltura = std::abs(cos(timeAccum * velocidadVertical));
	posicionBase.y = posicionCentral.y + (factorAltura * alturaMaxima);

	// Movimiento en espiral c�nica (Acerc�ndose/Alej�ndose del centro)
	float radioActual = radioMaximo * (1.0f - factorAltura);
	posicionBase.x = posicionCentral.x + radioActual * cos(timeAccum * velocidadOrbita);
	posicionBase.z = posicionCentral.z + radioActual * sin(timeAccum * velocidadOrbita);

	// Rotaci�n sobre su propio eje
	rotacionY += velocidadGiro * deltaTime;

	// Mantener los grados en un rango seguro
	if (rotacionY >= 360.0f) {rotacionY -= 360.0f;}
}

// Funci�n para guardar keyframes del tren (Tecla K)
void saveFrameTren(void)
{
	printf("Guardando keyframe del tren %d\n", FrameIndexTren);
	KeyFrameTren[FrameIndexTren].movTrenX = movTrenX;
	FrameIndexTren++;
}
//Tecla por definir para guardar B y H para habilitar el guardado
void saveFrameReloj(void)
{
	printf("Guardando keyframe del Reloj %d\n", FrameIndexReloj);
	KeyFrameReloj[FrameIndexReloj].rotEngranajeGrande = rotEngranajeGrande;
	KeyFrameReloj[FrameIndexReloj].rotEngranajeMediano = rotEngranajeMediano;
	KeyFrameReloj[FrameIndexReloj].rotEngranajePequeno = rotEngranajePequeno;
	KeyFrameReloj[FrameIndexReloj].rotEngranajeEnano = rotEngranajeEnano;
	KeyFrameReloj[FrameIndexReloj].rotManecillaHora = rotManecillaHora;
	KeyFrameReloj[FrameIndexReloj].rotManecillaMinuto = rotManecillaMinuto;
	FrameIndexReloj++;
}
// Funci�n para resetear el tren (Tecla 0)
void resetTren(void)
{
	movTrenX = KeyFrameTren[0].movTrenX;
	rotRuedasTren = 0.0f;
}
//Tecla R
void resetReloj(void)
{
	rotEngranajeGrande = KeyFrameReloj[0].rotEngranajeGrande;
	rotEngranajeMediano = KeyFrameReloj[0].rotEngranajeMediano;
	rotEngranajePequeno = KeyFrameReloj[0].rotEngranajePequeno;
	rotEngranajeEnano = KeyFrameReloj[0].rotEngranajeEnano;
	rotManecillaHora = 0.0f;
	rotManecillaMinuto = 0.0f;
}

// Interpolaci�n entre keyframes
void interpolationTren(void)
{
	KeyFrameTren[playIndexTren].movTrenXInc =
		(KeyFrameTren[playIndexTren + 1].movTrenX - KeyFrameTren[playIndexTren].movTrenX) / i_max_steps;
}
void interpolationReloj(void)
{
	KeyFrameReloj[playIndexReloj].incRotEngranajeGrande = (KeyFrameReloj[playIndexReloj + 1].rotEngranajeGrande - KeyFrameReloj[playIndexReloj].rotEngranajeGrande) / i_max_steps;
	KeyFrameReloj[playIndexReloj].incRotEngranajeMediano = (KeyFrameReloj[playIndexReloj + 1].rotEngranajeMediano - KeyFrameReloj[playIndexReloj].rotEngranajeMediano) / i_max_steps;
	KeyFrameReloj[playIndexReloj].incRotEngranajePequeno = (KeyFrameReloj[playIndexReloj + 1].rotEngranajePequeno - KeyFrameReloj[playIndexReloj].rotEngranajePequeno) / i_max_steps;
	KeyFrameReloj[playIndexReloj].incRotEngranajeEnano = (KeyFrameReloj[playIndexReloj + 1].rotEngranajeEnano - KeyFrameReloj[playIndexReloj].rotEngranajeEnano) / i_max_steps;
}
// Funci�n de animaci�n del tren
void animateTren(void)
{
	if (playTren)
	{
		if (i_curr_steps >= i_max_steps) // �Termin� la interpolaci�n entre frames?
		{
			playIndexTren++;

			if (playIndexTren > FrameIndexTren - 2) // �Termin� la animaci�n completa?
			{
				printf("Animaci�n del tren completada\n");
				playIndexTren = 0;
				playTren = false;
			}
			else // Siguiente frame
			{
				i_curr_steps = 0;
				interpolationTren();
			}
		}
		else
		{
			// Animar
			movTrenX += KeyFrameTren[playIndexTren].movTrenXInc;
			rotRuedasTren += 2.0f; // Rotar ruedas
			i_curr_steps++;
		}
	}
}
void animateReloj(void)
{
	if (playReloj)
	{
		if (i_curr_steps >= i_max_steps) // �Termin� la interpolaci�n entre frames?
		{
			playIndexReloj++;
			if (playIndexReloj > FrameIndexReloj - 2) // �Termin� la animaci�n completa?
			{
				printf("Animaci�n del reloj completada\n");
				playIndexReloj = 0;
				playReloj = false;
			}
			else // Siguiente frame
			{
				i_curr_steps = 0;
				interpolationReloj();
			}
		}
		else
		{
			// Animar
			rotEngranajeGrande += KeyFrameReloj[playIndexReloj].incRotEngranajeGrande;
			rotEngranajeMediano += KeyFrameReloj[playIndexReloj].incRotEngranajeMediano;
			rotEngranajePequeno += KeyFrameReloj[playIndexReloj].incRotEngranajePequeno;
			rotEngranajeEnano += KeyFrameReloj[playIndexReloj].incRotEngranajeEnano;
			rotManecillaMinuto -= 0.5f; // velocidad de los minutos
			rotManecillaHora -= 0.5f / 12.0f; // la hora gira 12 veces mas lento
			i_curr_steps++;
		}
	}
}
int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	//audio
	if (ma_engine_init(NULL, &engine) != MA_SUCCESS) {
		printf("Error al inicializar el motor de audio\n");
	}
	//Soundtrack de fondo
	ma_sound_init_from_file(&engine, "Audio/Musica/Soundtrack.wav", 0, NULL, NULL, &music);
	ma_sound_set_looping(&music, MA_TRUE); // Para que se repita
	ma_sound_start(&music); // Para empezar a reproducir

	plain = Texture("Textures/plain.png");
	plain.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	grassTexture = Texture("Textures/pasto.jpg");
	grassTexture.LoadTextureA();
	waterTexture = Texture("Textures/agua.jpg");
	waterTexture.LoadTextureA();

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

	//Bandera
	Flag = Model();
	Flag.LoadModel("Models/Flag.obj");
	Flagpole = Model();
	Flagpole.LoadModel("Models/Flagpole.obj");

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
	Castillo = Model();
	Castillo.LoadModel("Models/PeachCastle.obj");
	Iglesia = Model();
	Iglesia.LoadModel("Models/Iglesia.obj");
	EngranajesIglesia = Model();
	EngranajesIglesia.LoadModel("Models/EngranajesIglesia.obj");

	Reloj_M = Model();
	Reloj_M.LoadModel("Models/reloj.obj");
	EngranajeGrande_M = Model();
	EngranajeGrande_M.LoadModel("Models/engranajeGrande.obj");
	EngranajeMediano_M = Model();
	EngranajeMediano_M.LoadModel("Models/engranajeMediano.obj");
	EngranajePequeno_M = Model();
	EngranajePequeno_M.LoadModel("Models/engranajePequeno.obj");
	EngranajeEnano_M = Model();
	EngranajeEnano_M.LoadModel("Models/engranajeEnano.obj");
	ManecillaHora_M = Model();
	ManecillaHora_M.LoadModel("Models/manecillaHora.obj");
	ManecillaMinuto_M = Model();
	ManecillaMinuto_M.LoadModel("Models/manecillaMinuto.obj");

	KeyFrameReloj[0].rotEngranajeGrande = 35.0f;
	KeyFrameReloj[0].rotEngranajeMediano = -25.0f;
	KeyFrameReloj[0].rotEngranajePequeno = 35.0f;
	KeyFrameReloj[0].rotEngranajeEnano = -45.0f;

	KeyFrameReloj[1].rotEngranajeGrande = 55.0f;
	KeyFrameReloj[1].rotEngranajeMediano = 0.0f;
	KeyFrameReloj[1].rotEngranajePequeno = 70.0f;
	KeyFrameReloj[1].rotEngranajeEnano = 0.0f;

	KeyFrameReloj[2].rotEngranajeGrande = 90.0f;
	KeyFrameReloj[2].rotEngranajeMediano = 15.0f;
	KeyFrameReloj[2].rotEngranajePequeno = 120.0f;
	KeyFrameReloj[2].rotEngranajeEnano = 45.0f;

	FrameIndexReloj = 3; // 3 keyframes definidos

	TunelTren = Model();
	TunelTren.LoadModel("Models/TunelTren.obj");

	//decoraciones
	Farola = Model();
	Farola.LoadModel("Models/Farola.obj");
	Arbol1 = Model();
	Arbol1.LoadModel("Models/ArbolLowPoly.obj");
	Arbol2 = Model();
	Arbol2.LoadModel("Models/Abeto.obj");
	Bancos = Model();
	Bancos.LoadModel("Models/BancosSentar.obj");
	Estrella = Model();
	Estrella.LoadModel("Models/Star-Mario64.obj");
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

	Locomotora = Model();
	Locomotora.LoadModel("Models/Locomotora.obj");
	Vagon = Model();
	Vagon.LoadModel("Models/Vagon.obj");
	ViasTren = Model();
	ViasTren.LoadModel("Models/ViasTren.obj");

	// Keyframes inicializados del tren con valores por defecto
	// Keyframe 0: posici�n inicial
	KeyFrameTren[0].movTrenX = 240.0f;
	// Keyframe 1: se movi� a la izquierda
	KeyFrameTren[1].movTrenX = 200.0f;
	// Keyframe 2: regresa al centro
	KeyFrameTren[2].movTrenX = 240.0f;
	FrameIndexTren = 3; // 3 keyframes definidos


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


	//luz direccional, s�lo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, -1.0f, 0.0f);

	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaraci�n de primer luz puntual
	// Farola 1
	pointLights[pointLightCount] = PointLight(1.0f, 0.6f, 0.2f,  // Color naranja c�lido
		0.5f, 2.5f,  // Intensidad ambiental y difusa AUMENTADAS
		-80.0f, 7.0f, 156.0f,  // Posici�n
		1.0f, 0.022f, 0.0019f);  // Atenuaci�n REDUCIDA (mayor alcance)

	// Farola 2
	pointLights[pointLightCount + 1] = PointLight(1.0f, 0.6f, 0.2f,
		0.5f, 2.5f,
		-80.0f, 7.0f, 112.0f,
		1.0f, 0.022f, 0.0019f);

	// Farola 3
	pointLights[pointLightCount + 2] = PointLight(1.0f, 0.6f, 0.2f,
		0.5f, 2.5f,
		10.0f, 7.0f, -120.0f,
		1.0f, 0.022f, 0.0019f);

	// Farola 4
	pointLights[pointLightCount + 3] = PointLight(1.0f, 0.6f, 0.2f,
		0.5f, 2.5f,
		150.0f, 7.0f, 30.0f,
		1.0f, 0.022f, 0.0019f);

	unsigned int baseLightCount = pointLightCount;
	numFarolasActivas = 4; // N�mero total de farolas

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
	//luz del foco de la locomotora
	spotLights[2] = SpotLight(
		1.0f, 1.0f, 0.0f,      // Color: Amarillo puro 
		0.2f, 5.0f,            
		80.0f, 20.0f, 75.0f,  
		0.0f, -1.0f, 0.0f,     
		0.1f, 0.005f, 0.002f,  
		25.0f                  
	);
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
	glm::vec2 toffsetAgua = glm::vec2(0.0f, 0.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		angulovaria += 0.5f * deltaTime;
		// Actualizar ciclo d�a/noche
		mainLight.UpdateCycle(deltaTime);

		GLfloat sunHeight = mainLight.getDirection().y; // Obtener altura del sol

		if (sunHeight > 0.0f && !farolasEncendidas) // Es de noche y farolas apagadas
		{
			// Encender farolas
			farolasEncendidas = true;
			pointLightCount = baseLightCount + numFarolasActivas;
		}
		else if (sunHeight <= 0.0f && farolasEncendidas) // Es de d�a y farolas encendidas
		{
			// Apagar farolas
			farolasEncendidas = false;
			pointLightCount = baseLightCount;
		}


		// Animar el dirigible
		animacionDirigible(deltaTime, posicionDirigible, rotYDirigible, inclinacionDirigible, dirigibleTime);

		// Actualizar animaci�n de la motocicleta
		animacionMoto(deltaTime, motoPosition, centroRecorridoMoto, motoRotationY, anguloInclinacionMoto, rotLlantasMoto, motoTime);

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Cambiar modo de c�mara con teclas num�ricas
		if (mainWindow.getsKeys()[GLFW_KEY_1])
		{
			camera.setCameraMode(1); // Modo tercera persona
		}
		if (mainWindow.getsKeys()[GLFW_KEY_2])
		{
			camera.setCameraMode(2); // Modo a�reo
		}
		if (mainWindow.getsKeys()[GLFW_KEY_3])
		{
			camera.setCameraMode(3); // Modo c�mara fija 1
		}
		if (mainWindow.getsKeys()[GLFW_KEY_4])
		{
			camera.setCameraMode(4); // Modo c�mara fija 2
		}
		if (mainWindow.getsKeys()[GLFW_KEY_5])
		{
			camera.setCameraMode(5); // Modo c�mara fija 3
		}

		// Toggle luz sobre la espada (tecla F)
		if (mainWindow.getsKeys()[GLFW_KEY_F])
		{
			if (!luzEspadaKeyPressed)
			{
				luzEspadaActiva = !luzEspadaActiva;
				luzEspadaKeyPressed = true;
			}
		}
		else
		{
			luzEspadaKeyPressed = false;
		}

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		if (sunHeight > 0.0f) // Noche
		{
			skyboxNoche.DrawSkybox(camera.calculateViewMatrix(), projection);
		}
		else // D�a 
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

		//informaci�n en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la c�mara de tipo flash
		lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//informaci�n al shader de fuentes de iluminaci�n
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);

		// Si la luz de la espada est� activa, mandar el arreglo completo.
		// Si est� apagada, armar un arreglo temporal sin spotLights[1].
		if (luzEspadaActiva)
		{
			shaderList[0].SetSpotLights(spotLights, spotLightCount);
		}
		else
		{
			SpotLight spotLightsSinEspada[MAX_SPOT_LIGHTS];
			unsigned int countSinEspada = 0;
			for (unsigned int i = 0; i < spotLightCount; i++)
			{
				if (i != 1) // 1 es la luz sobre la espada
					spotLightsSinEspada[countSinEspada++] = spotLights[i];
			}
			shaderList[0].SetSpotLights(spotLightsSinEspada, countSinEspada);
		}


		//Reinicializando variables cada ciclo de reloj
		model = glm::mat4(1.0);
		modelaux = glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		toffsetAgua = glm::vec2(0.0f, 0.0f);
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//	glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		// -- - RENDERIZAR PASTO-- -
		//pasto derecho molinos iglesia
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-215.0f, -1.99f, -50.0f)); // Un poco arriba del suelo
		model = glm::scale(model, glm::vec3(85.0f, 1.0f, 250.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		grassTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[0]->RenderMesh();

		//pasto izquierdo espada en la piedra castillo mario
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(140.0f, -1.99f, 110.0f)); // Un poco arriba del suelo
		model = glm::scale(model, glm::vec3(160.0f, 1.0f, 90.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		grassTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[0]->RenderMesh();

		// --- RENDERIZAR AGUA ---
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.99f, 250.0f)); // Un poco abajo si quieres un estanque
		model = glm::scale(model, glm::vec3(300.0f, 1.0f, 50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		waterTexture.UseTexture();
		toffsetAgua = glm::vec2(angulovaria * 0.001f,0.0f);
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffsetAgua));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
	

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
		model = glm::translate(model, glm::vec3(10.0f, 1.0f, -80.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Sherlock.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(35.0f, 55.0f, 210.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Waver.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, 0.5f, 115.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reines.RenderModel();

		//Mario (NPC)
		//El cuerpo es el modelo padre del resto de las partes del cuerpo.
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-110.0f, -2.0f, 135.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
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
		model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux3 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Mario64BrazoIzq.RenderModel();
		model = modelaux3;
		model = glm::translate(model, glm::vec3(0.42f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Mario64AntebrazoIzq.RenderModel();
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-0.45f, 2.0f, 0.0f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux3 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Mario64BrazoDer.RenderModel();
		model = modelaux3;
		model = glm::translate(model, glm::vec3(-0.42f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
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


		// Activar/desactivar animaci�n de Excalibur con tecla E
		if (mainWindow.getsKeys()[GLFW_KEY_E])
		{
			if (!excaliburSacada)
			{
				excaliburSacada = true;
				excaliburTiempoAnimado = 0.0f; // Reiniciar animaci�n
			}
		}

		// Actualizar animaci�n de Excalibur si est� activa
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
				giroEspada = excaliburTiempoAnimado - 2.0f; // Tiempo desde que empez� a girar
				rotExcalibur = giroEspada * 180.0f; // 2 vueltas completas en 4 segundos (720 grados)
			}
			else if (excaliburTiempoAnimado < animDuration) // Fase 3: Bajar (2 segundos)
			{
				// Bajar suavemente desde 8.0 hasta 1.0
				float tiempoDescenso = excaliburTiempoAnimado - 6.0f; // 0 a 2
				progreso = tiempoDescenso / 2.0f; // 0 a 1
				excaliburY = 6.0f - (progreso * 7.0f); // De 8.0 a 1.0
				rotExcalibur = 720.0f; // Mantener rotaci�n final
			}
			else // Animaci�n completada
			{
				// Reiniciar animaci�n
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
		//roca detras de la piedra para dar sensaci�n de profundidad al punto de inter�s
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
		
		//Bandera SM64
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-200.0f, -2.0, -130.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Flagpole.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, -26.0f + mainWindow.getBanderaAltura(), 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Flag.RenderModel();

		//modelo de barco volador
		model = glm::mat4(1.0);
		model = glm::translate(model, posicionDirigible);//para cambiar la posici�n del dirigible en su recorrido
		model = glm::rotate(model, glm::radians(rotYDirigible+180), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotaci�n animada
		// A�adir inclinaci�n en los giros
		//inclinacionDirigible = sin(dirigibleTime * 0.6f) * 15.0f;
		model = glm::rotate(model, glm::radians(inclinacionDirigible), glm::vec3(0.0f, 0.0f, 1.0f));

		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Dirigible.RenderModel();

		// Aspa del dirigible DERECHA (con rotaci�n continua)
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.5f, -0.2f, 1.25f));
		model = glm::rotate(model, dirigibleTime * 10.0f, glm::vec3(0.0f, 0.0f, 1.0f)); // Gira r�pido
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		AspaDirigible.RenderModel();

		// Aspa del dirigible IZQUIERDA (con rotaci�n continua)
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.5f, -0.2f, 1.25f));
		model = glm::rotate(model, -dirigibleTime * 10.0f, glm::vec3(0.0f, 0.0f, 1.0f)); // Gira r�pido en sentido opuesto
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		AspaDirigible.RenderModel();

		// Cola del dirigible (movimiento como pez)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 3.0f));
		// Movimiento de cola tipo pez (oscilaci�n lateral)
		rotColaDirigible = sin(dirigibleTime * 3.0f) * 20.0f; // Oscila �20 grados
		model = glm::rotate(model, glm::radians(rotColaDirigible), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		ColaDirigible.RenderModel();

		//modelo de motocicleta
		model = glm::mat4(1.0f);
		model = glm::translate(model, motoPosition); // Usar posici�n animada (que ya tiene el offset)
		model = glm::rotate(model, glm::radians(motoRotationY), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(anguloInclinacionMoto), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		CuerpoMoto.RenderModel();

		// Llanta delantera izquierda con rotaci�n de avance
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.675f, -0.55f, -2.6f));
		model = glm::rotate(model, glm::radians(rotLlantasMoto), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotaci�n de la llanta
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		LlantaDelanteraMoto.RenderModel();

		// Llanta delantera derecha con rotaci�n de avance
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.675f, -0.55f, -2.6f));
		model = glm::rotate(model, glm::radians(rotLlantasMoto), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotaci�n de la llanta
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		LlantaDelanteraMoto.RenderModel();

		// Llanta trasera izquierda con rotaci�n de avance
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -0.6f, 1.1f));
		model = glm::rotate(model, glm::radians(rotLlantasMoto), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotaci�n de la llanta
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		LlantaTraseraMoto.RenderModel();

		// Llanta trasera derecha con rotaci�n de avance
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -0.6f, 2.55f));
		model = glm::rotate(model, glm::radians(rotLlantasMoto), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotaci�n de la llanta
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		LlantaTraseraMoto.RenderModel();


		// Controles del tren con keyframes
		if (mainWindow.getsKeys()[GLFW_KEY_K]) // Guardar keyframe
		{
			if (guardoFrame < 1)
			{
				saveFrameTren();
				printf("Frame guardado con exito. Presiona 'L' para habilitar el siguiente.\n");
				guardoFrame++;
				reinicioFrame = 0;
			}
		}
		if (mainWindow.getsKeys()[GLFW_KEY_L]) // Guardar keyframe
		{
			if (reinicioFrame < 1)
			{
				guardoFrame = 0;
				reinicioFrame++;
				printf("Habilitado: Ya puedes guardar otro frame con 'K'\n");
			}
		}
		if (mainWindow.getsKeys()[GLFW_KEY_P]) // Reproducir animaci�n
		{
			if (!playTren)
			{
				resetTren();
				playTren = true;
				playIndexTren = 0;
				i_curr_steps = 0;
				interpolationTren();
			}
		}
		if (mainWindow.getsKeys()[GLFW_KEY_0]) // Resetear
		{
			resetTren();
			playTren = false;
		}

		// Movimiento manual del tren 
		if (!playTren)
		{
			if (mainWindow.getsKeys()[GLFW_KEY_N]) 
			{
				if (ciclo < 1)
				{
					movTrenX -= 20.0f;
					printf("Tren movido hacia adelante: %f. Presiona 'M' para habilitar.\n", movTrenX);
					ciclo++;
					ciclo2 = 0;
				}
			}
			if (mainWindow.getsKeys()[GLFW_KEY_M]) 
			{
				if (ciclo2 < 1)
				{
					ciclo = 0;
					ciclo2++;
					printf("Tecla 'N' habilitada de nuevo.\n");
				}
			}
		}

		// Actualizar animaci�n del tren
		animateTren();

		//Modelo de tren
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movTrenX, 3.0f, -15.0f)); // Centro del escenario
		modelaux = model;
		glm::vec3 posTren = glm::vec3(model[3][0], model[3][1], model[3][2]);
		posTren.z += 2.0f; // Ajusta este valor seg�n el largo de tu modelo para que salga por el frente
		glm::vec3 dirTren = glm::vec3(-1.0f, 0.0f, 0.0f);
		// 3. Actualizar la Spotlight (asumiendo que es la posici�n 2 del arreglo de spotlights)
		// Parametros: Color, Ambiente, Difuso, Posicion, Direccion, Lineal, Cuadratico, Cutoff
		spotLights[2].SetFlash(posTren, dirTren);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Locomotora.RenderModel();
		//faltan ruedas, pero no se alcanz� a modelar a tiempo, se hizo un modelo de tren muy simple para poder incluirlo en el escenario y mostrar la animaci�n por keyframes
		// Vag�n 
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Vagon.RenderModel();


		//ESTRUCTURAS

		//Castillo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(150.0f, -1.5f, 105.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Castillo.RenderModel();

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
		for (const auto& fabricas : posicionesFabricas) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, fabricas.posicion);
			model = glm::rotate(model, glm::radians(fabricas.rotacionY), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
			fabrica.RenderModel();
		}

		//tunel del tren
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(260.0f, -3.0f, -15.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		TunelTren.RenderModel();

		//vias
		for (const auto& via : posicionesVias) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, via);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
			ViasTren.RenderModel();
		}

		//animacion por teclado para el movimiento de los brazo y la casa
		if (mainWindow.getsKeys()[GLFW_KEY_Q])
		{
			if (!presionoQ) // Solo cambia el estado una vez por pulsaci�n
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
				direccionCasa = -1; // Cambiamos direcci�n
				faseCasa = 1;       // Pasamos a la siguiente fase
			}
			// FASE 1: De la derecha a la izquierda (cruzando el centro)
			else if (direccionCasa == -1 && progresoCasa <= -1.0f)
			{
				progresoCasa = -1.0f;
				direccionCasa = 1; // Cambiamos direcci�n para volver
				faseCasa = 2;      // Pasamos a la fase final
			}
			// FASE 2: De la izquierda de regreso al centro EXACTO
			else if (faseCasa == 2 && direccionCasa == 1 && progresoCasa >= 0.0f)
			{
				progresoCasa = 0.0f; // La fijamos exactamente en el centro (posici�n inicial)
				animarCasa = false;
				faseCasa = 0;        // Lista para la pr�xima vez que presiones Q
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
			t = progresoCasa; // Ya est� de 0 a 1
			angBrazo1 = 45.0f + (135.0f - 45.0f) * t;
			angBrazo2 = -45.0f + (-90.0f - (-45.0f)) * t; // Asum� 0.0f para extremo derecho
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

		//molinos, renderizados en un loop para aplicar animaci�n a las aspas de cada molino
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

		//Iglesia
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

		animateReloj();
		//Reloj con jerarqu�a de modelos
		//Base del reloj
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(posXreloj, posYreloj, posZreloj));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reloj_M.RenderModel();

		// Engtanaje Grande
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.0f, 1.3f, -1.3f));
		model = glm::rotate(model, glm::radians(rotEngranajeGrande), glm::vec3(0.0f, 0.0f, 1.0f)); // eje Z
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeGrande_M.RenderModel();

		// Engranaje Mediano
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.3f, -8.5f, -1.3f));
		model = glm::rotate(model, glm::radians(rotEngranajeMediano), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeMediano_M.RenderModel();

		// Engranaje Peque�o
		model = modelaux;
		model = glm::translate(model, glm::vec3(9.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotEngranajePequeno), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajePequeno_M.RenderModel();

		// Engranaje Enano
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -2.6f, 0.0f));
		model = glm::rotate(model, glm::radians(rotEngranajeEnano), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeEnano_M.RenderModel();

		// Manecilla Hora
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.2f, -0.1f, -0.1f));
		model = glm::rotate(model, glm::radians(rotManecillaHora), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ManecillaHora_M.RenderModel();

		// Manecilla Minuto
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.2f, -0.1f, -0.1f));
		model = glm::rotate(model, glm::radians(rotManecillaMinuto), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ManecillaMinuto_M.RenderModel();

		// Controles del reloj con keyframes
		if (mainWindow.getsKeys()[GLFW_KEY_B]) // Guardar keyframe
		{
			if (guardoFrame < 1)
			{
				saveFrameReloj();
				printf("Frame guardado con exito. Presiona 'H' para habilitar el siguiente.\n");
				guardoFrame++;
				reinicioFrame = 0;
			}
		}
		if (mainWindow.getsKeys()[GLFW_KEY_H]) // Guardar keyframe
		{
			if (reinicioFrame < 1)
			{
				guardoFrame = 0;
				reinicioFrame++;
				printf("Habilitado: Ya puedes guardar otro frame con 'B'\n");
			}
		}
		if (mainWindow.getsKeys()[GLFW_KEY_SPACE]) // Reproducir animaci�n
		{
			if (!playReloj)
			{
				resetReloj();
				playReloj = true;
				playIndexReloj = 0;
				i_curr_steps = 0;
				interpolationReloj();
			}
		}
		if (mainWindow.getsKeys()[GLFW_KEY_R]) // Resetear
		{
			resetReloj();
			playReloj = false;
		}

		// Movimiento engranaje grande del reloj
		if (!playReloj)
		{
			if (mainWindow.getsKeys()[GLFW_KEY_Z])
			{
				if (ciclo < 1)
				{
					rotEngranajeGrande += 35.0f;
					printf("Tren movido hacia adelante: %f. Presiona 'M' para habilitar.\n", movTrenX);
					ciclo++;
					ciclo2 = 0;
				}
			}
			if (mainWindow.getsKeys()[GLFW_KEY_X])
			{
				if (ciclo2 < 1)
				{
					ciclo = 0;
					ciclo2++;
					printf("Tecla 'N' habilitada de nuevo.\n");
				}
			}
			if (mainWindow.getsKeys()[GLFW_KEY_C])
			{
				if (ciclo < 1)
				{
					rotEngranajeMediano -= 35.0f;
					printf("Tren movido hacia adelante: %f. Presiona 'V' para habilitar.\n", movTrenX);
					ciclo++;
					ciclo2 = 0;
				}
			}
			if (mainWindow.getsKeys()[GLFW_KEY_V])
			{
				if (ciclo2 < 1)
				{
					ciclo = 0;
					ciclo2++;
					printf("Tecla 'C' habilitada de nuevo.\n");
				}
			}
			if (mainWindow.getsKeys()[GLFW_KEY_6])
			{
				if (ciclo < 1)
				{
					rotEngranajePequeno += 35.0f;
					printf("Tren movido hacia adelante: %f. Presiona '7' para habilitar.\n", movTrenX);
					ciclo++;
					ciclo2 = 0;
				}
			}
			if (mainWindow.getsKeys()[GLFW_KEY_7])
			{
				if (ciclo2 < 1)
				{
					ciclo = 0;
					ciclo2++;
					printf("Tecla '6' habilitada de nuevo.\n");
				}
			}
			if (mainWindow.getsKeys()[GLFW_KEY_8])
			{
				if (ciclo < 1)
				{
					rotEngranajeEnano -= 35.0f;
					printf("Tren movido hacia adelante: %f. Presiona '9' para habilitar.\n", movTrenX);
					ciclo++;
					ciclo2 = 0;
				}
			}
			if (mainWindow.getsKeys()[GLFW_KEY_9])
			{
				if (ciclo2 < 1)
				{
					ciclo = 0;
					ciclo2++;
					printf("Tecla '8' habilitada de nuevo.\n");
				}
			}
		}

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

		//Estrella Mario 64
		animacionEstrella(deltaTime, posicionEstrella, rotacionEstrellaY, timeAccumEstrella);
		model = glm::mat4(1.0);
		model = glm::translate(model, posicionEstrella + glm::vec3(-110.0f, -1.0f, 135.0f)); //Posicion din�mica + base
		model = glm::rotate(model, glm::radians(90.0f + rotacionEstrellaY), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Estrella.RenderModel();



		
		
		glDisable(GL_BLEND);
		
		glUseProgram(0);

		mainWindow.swapBuffers();
	}
	// 1. Detener el sonido
	ma_sound_stop(&music);

	// 2. Liberar el recurso del sonido
	ma_sound_uninit(&music);

	// 3. Liberar el motor de audio
	ma_engine_uninit(&engine);
	return 0;
}
