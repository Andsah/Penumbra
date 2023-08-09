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

#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

#include <array>
#include <vector>
#include <string>

// Object for handling player controls and camera movement
Camera* playerCamera = new Camera();
// Shaders
GLuint skyboxShaders, terrainShaders, objectsShaders;

// Matrix for projecting the view onto the screen(?)
mat4 projectionMatrix;
// Matrix for translating the worldspace into view-coordinates
mat4 world2viewMatrix;
// I plan on creating a game objects class (with polymorphism/inheritance?) that contains it's own total transformation matrix

// Intermediary to set the look-at function in the player camera to be used by openGL
void bindCamera(int x, int y) {playerCamera->calcLookAt(x, y);}

// TODO: make classes/structs for light source, game objects, portals etc.

Terrain * terrain;

std::vector<Light *> lights;


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
	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 250.0);
	
	glUseProgram(skyboxShaders);
	glUniformMatrix4fv(glGetUniformLocation(skyboxShaders, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

	glUseProgram(terrainShaders);
	glUniformMatrix4fv(glGetUniformLocation(terrainShaders, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

	glUseProgram(objectsShaders);
	glUniformMatrix4fv(glGetUniformLocation(objectsShaders, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

    // Initiate terrain - will probably make a separate file for this

	Texture * grass = new Texture("assets/tga_files/brick02.tga", "assets/tga_files/brick02_normal.tga", "assets/tga_files/grass_normal.tga");

	std::array<Texture *, NUM_TEX> texList {nullptr};

	texList[0] = grass;

	terrain = new Terrain("assets/tga_files/fft-terrain.tga", texList, terrainShaders); // 1204c so big it lags - frustum culling pls lol
	

    // Initiate game objects (models, textures, material properties etc.) - portals should fit in here somewhere

    // Initiate lighting objects

	Light * testLight = new Light(vec3(0.5f,0.5f,0.5f), vec3(0.3f,0.1f,0.1f), vec3(10, 5, 10), true);
	lights.push_back(testLight);


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
	playerCamera->checkKeyboardInput();
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//printf("(%f, %f, %f)\n", playerCamera->cameraPos.x, playerCamera->cameraPos.y, playerCamera->cameraPos.z);

	// Build the world-to-view matrix every tick
	world2viewMatrix = lookAtv(playerCamera->cameraPos, playerCamera->lookAtPos, playerCamera->upVec);
	glUseProgram(skyboxShaders);
	glUniformMatrix4fv(glGetUniformLocation(skyboxShaders, "viewMatrix"), 1, GL_TRUE, world2viewMatrix.m);

	glUseProgram(terrainShaders);
	glUniformMatrix4fv(glGetUniformLocation(terrainShaders, "viewMatrix"), 1, GL_TRUE, world2viewMatrix.m);

	glUseProgram(objectsShaders);
	glUniformMatrix4fv(glGetUniformLocation(objectsShaders, "viewMatrix"), 1, GL_TRUE, world2viewMatrix.m);

	//Upload lights to objects and terrain shaders

	for(int i = 0; i < lights.size(); i++) {
		glUseProgram(terrainShaders);
		lights[i]->uploadLight(terrainShaders, i);
		//glUseProgram(objectsShaders);
		//lights[i]->uploadLight(objectsShaders, i);
	}
	
	// Draw skybox (remember to turn off depth buffer)


	// Draw terrain

	terrain->Draw();

	// Draw objects

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

	glutMainLoop();
	exit(0);
}