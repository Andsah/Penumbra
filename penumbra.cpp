/*
║===========================================================================║
║                                                                           ║
║   ██████  ███████ ███    ██ ██    ██ ███    ███ ██████  ██████   █████    ║
║   ██   ██ ██      ████   ██ ██    ██ ████  ████ ██   ██ ██   ██ ██   ██   ║
║   ██████  █████   ██ ██  ██ ██    ██ ██ ████ ██ ██████  ██████  ███████   ║
║   ██      ██      ██  ██ ██ ██    ██ ██  ██  ██ ██   ██ ██   ██ ██   ██   ║
║   ██      ███████ ██   ████  ██████  ██      ██ ██████  ██   ██ ██   ██   ║
║                                                                           ║
║===========================================================================║
*/
// The main game file
#define MAIN
#include "headers/camera.h"
#include "headers/gameObject.h"
#include "headers/light.h"
#include "headers/terrain.h"
#include "headers/skybox.h"

#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

#include <array>
#include <vector>
#include <string>

std::string CASTLETGA = "assets/interiorcastle/tga/";
std::string CASTLEOBJ = "assets/interiorcastle/obj/";
std::string TGA = "assets/tga/";
std::string OBJ = "assets/obj/";

// Object for handling player controls and camera movement
Camera* playerCamera = new Camera();
// Shaders
GLuint skyboxShaders, terrainShaders, objectsShaders;
// I plan on creating a game objects class (with polymorphism/inheritance?) that contains it's own total transformation matrix

// Intermediary to set the look-at function in the player camera to be used by openGL
void bindCamera(int x, int y) {playerCamera->calcLookAt(x, y);}

// Another intermediary for the mouse clicking function, used for picking
void bindMouseFunc(int button, int state, int x, int y) {playerCamera->mouseFunc(button, state, x, y);}

// TODO: make classes/structs for light source (check), game objects (check), portals etc.

Terrain * terrain;

std::vector<Light *> lights;

Skybox * skybox;

std::vector<GameObject *> objects;



/*      ___          _   _   
 *     |_ _|  _ _   |_| | |_ 
 *      | |  | ' \  | | |  _|
 *     |___| |_||_| |_|  \__|
 */
// Initiates a lot of values, loads models and shaders etc.
void init(void) {
    // GL inits
	glClearColor(0.2,0.2,0.7,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//printError("GL inits");

	// Load and compile shaders to use (One for skybox, one for terrain, one for game objects)
	skyboxShaders = loadShaders("shaders/skybox.vert", "shaders/skybox.frag");
	terrainShaders = loadShaders("shaders/terrain.vert", "shaders/terrain.frag");
	objectsShaders = loadShaders("shaders/objects.vert", "shaders/objects.frag");

	// Set the projection to be perspective projection with a frustum
	playerCamera->setProjMat(frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 250.0)); // probably make these defines so i can make planes for frustum culling
	
	glUseProgram(skyboxShaders);
	glUniformMatrix4fv(glGetUniformLocation(skyboxShaders, "projMatrix"), 1, GL_TRUE, playerCamera->projectionMatrix.m);

	glUseProgram(terrainShaders);
	glUniformMatrix4fv(glGetUniformLocation(terrainShaders, "projMatrix"), 1, GL_TRUE, playerCamera->projectionMatrix.m);

	glUseProgram(objectsShaders);
	glUniformMatrix4fv(glGetUniformLocation(objectsShaders, "projMatrix"), 1, GL_TRUE, playerCamera->projectionMatrix.m);

	// Initiate skybox - make into a class/objecttype?

	Texture * skyboxTex = new Texture(TGA + "SkyBoxFull.tga", "", "");

	skybox = new Skybox("assets/obj/skyboxfull.obj", skyboxTex, skyboxShaders);

    // Initiate terrain - will probably make a separate file for this

	Texture * terrainTex = new Texture(TGA + "brick02.tga", TGA + "brick02_normal.tga", TGA + "brick02_normal.tga");

	std::array<Texture *, NUM_TEX> texList {terrainTex};

	terrain = new Terrain("assets/tga/fft-terrain.tga", texList, terrainShaders); // 1204c so big it lags - frustum culling pls lol
	

    // Initiate game objects (models, textures, material properties etc.) - portals should fit in here somewhere

	// just a test - ceiling has backside, rendering z-buffer from above to fbo to get heightmap for player walking not gonna work - maybe if split all ceils off to a draw after writing to fbo?
	//also this should be instancable
	std::array<Texture *, NUM_TEX> ttex = {new Texture(CASTLETGA + "Floor02.tga", "", "")};
	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, terrainShaders, T(10,4, 1)));
	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, terrainShaders, T(6, 4, 1)));
	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, terrainShaders, T(2, 4, 1)));
	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, terrainShaders, T(10,4, 5)));
	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, terrainShaders, T(6, 4, 5)));
	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, terrainShaders, T(2, 4, 5)));
	objects.push_back(new GameObject(CASTLEOBJ + "Stairs01.obj", ttex, terrainShaders, T(6, 2.9, 8.6)));

    // Initiate lighting objects - should make files with placement data for lights and objects aka level info
	Light * tL1 = new Light(vec3(1.0f,0.0f,0.0f), vec3(0.1f,0.3f,0.3f), vec3(100, 10, 200), true);
	Light * tL2 = new Light(vec3(0.0f,1.0f,0.5f), vec3(0.0f,0.0f,0.0f), vec3(3, 6, 6), true);
	Light * tL3 = new Light(vec3(0.0f,1.0f,0.5f), vec3(0.0f,0.0f,0.0f), vec3(100, 10, 100), true);
	Light * tL4 = new Light(vec3(0.0f,1.0f,0.5f), vec3(0.0f,0.0f,0.0f), vec3(3, 6, 300), true);
	Light * tL5 = new Light(vec3(0.0f,1.0f,0.5f), vec3(0.0f,0.0f,0.0f), vec3(200, 14, 6), true);
	Light * tL6 = new Light(vec3(0.0f,1.0f,0.5f), vec3(0.0f,0.0f,0.0f), vec3(3, 50, 6), true);
	Light * tL7 = new Light(vec3(0.0f,1.0f,0.5f), vec3(0.0f,0.0f,0.0f), vec3(40, 40, 36), true);
	Light * tL8 = new Light(vec3(0.0f,1.0f,0.5f), vec3(0.0f,0.0f,0.0f), vec3(300, 16, 64), true);
	Light * tL9 = new Light(vec3(0.0f,1.0f,0.5f), vec3(0.0f,0.0f,0.0f), vec3(40, -13, 60), true);
	Light * tLa = new Light(vec3(0.0f,1.0f,0.5f), vec3(0.0f,0.0f,0.0f), vec3(200, 6, 200), true);

	lights.push_back(tL1);
	lights.push_back(tL2);
	lights.push_back(tL3);
	lights.push_back(tL4);
	lights.push_back(tL5);
	lights.push_back(tL6);
	lights.push_back(tL7);
	lights.push_back(tL8);
	lights.push_back(tL9);
	lights.push_back(tLa);


    // Perhaps more things to initiate

}


/*      ___    _               _               
 *     |   \  |_|  ___  _ __  | |  __ _   _  _ 
 *     | |) | | | (_-< | '_ \ | | / _` | | || |
 *     |___/  |_| /__/ | .__/ |_| \__,_|  \_, |
 *                     |_|                |__/ 
 */
// Draws the image every tick
void display(void) {
	playerCamera->checkKeyboardInput(terrain);

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Build the world-to-view matrix every tick
	playerCamera->updateWorld2view();// precalc inverse view*model mat instead of doing expensively in shader and save to models.
	
	glUseProgram(skyboxShaders);
	glUniformMatrix4fv(glGetUniformLocation(skyboxShaders, "viewMatrix"), 1, GL_TRUE, playerCamera->world2viewMatrix.m);

	glUseProgram(terrainShaders);
	glUniformMatrix4fv(glGetUniformLocation(terrainShaders, "viewMatrix"), 1, GL_TRUE, playerCamera->world2viewMatrix.m);

	glUseProgram(objectsShaders);
	glUniformMatrix4fv(glGetUniformLocation(objectsShaders, "viewMatrix"), 1, GL_TRUE, playerCamera->world2viewMatrix.m);

	//Upload lights to objects and terrain shaders
	for(int i = 0; i < lights.size(); i++) {
		glUseProgram(terrainShaders);
		lights[i]->uploadLight(terrainShaders, i);
		//glUseProgram(objectsShaders);
		//lights[i]->uploadLight(objectsShaders, i);
	}
	
	// Draw skybox (remember to turn off depth buffer)
	skybox->setTransform(T(playerCamera->cameraPos.x, playerCamera->cameraPos.y, playerCamera->cameraPos.z));
	skybox->Draw();

	// Draw terrain
	terrain->Draw();

	// Draw objects - need to cull away some objects (cell based)
	for (size_t i = 0; i < objects.size(); i++){objects.at(i)->Draw();}

	glutSwapBuffers();
}


/*      __  __          _        
 *     |  \/  |  __ _  |_|  _ _  
 *     | |\/| | / _` | | | | ' \ 
 *     |_|  |_| \__,_| |_| |_||_|
 */
int main(int argc, char **argv) {
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize(WIN_W, WIN_H);

	//printf("%d vs %d", sizeof(GLfloat), sizeof(vec3));

	glutCreateWindow("Penumbra™ Definitive Edition GOTY™ Edition Collector's Edition");
	glutDisplayFunc(display);
	init();

	glutRepeatingTimer(20);
    glutPassiveMotionFunc(&bindCamera);
	glutMouseFunc(&bindMouseFunc);

	glutMainLoop();
	exit(0);
}