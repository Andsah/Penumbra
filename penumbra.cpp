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
#include "camera.h"
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

// Object for handling player controls and camera movement
Camera* playerCamera = new Camera();
// Shaders
GLuint skyboxShaders, terrainShaders, objectShaders;

// Matrix for projecting the view onto the screen(?)
mat4 projectionMatrix;
// Matrix for translating the worldspace into view-coordinates
mat4 world2viewMatrix;
// I plan on creating a game objects class (with polymorphism/inheritance?) that contains it's own total transformation matrix

// Intermediary to set the look-at function in the player camera to be used by openGL
void bindCamera(int x, int y) {playerCamera->calcLookAt(x, y);}

// TODO: make classes/structs for light source, game objects, portals etc.


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
	glDisable(GL_CULL_FACE);
	//printError("GL inits");

	// Set the projection to be perspective projection with a frustum
	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 200.0);
	
    // Load and compile shaders to use (One for skybox, one for terrain, one for game objects)
	skyboxShaders = loadShaders("shaders/skybox.vert", "shaders/skybox.frag");
	terrainShaders = loadShaders("shaders/terrain.vert", "shaders/terrain.frag");
	objectShaders = loadShaders("shaders/objects.vert", "shaders/objects.frag");

    // Initiate terrain - will probably make a separate file for this


    // Initiate game objects (models, textures, material properties etc.) - portals should fit in here somewhere
	

    // Initiate lighting objects


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

	// Build the world-to-view matrix every tick
	world2viewMatrix = lookAtv(playerCamera->cameraPos, playerCamera->lookAtPos, playerCamera->upVec);

	// Draw skybox (remember to turn off depth buffer)


	// Draw terrain


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

	glutCreateWindow("Penumbra™ Definitive Edition GOTY™ Edition Collector's Edition");
	glutDisplayFunc(display);
	init();

	glutRepeatingTimer(20);
    glutPassiveMotionFunc(&bindCamera);

	glutMainLoop();
	exit(0);
}