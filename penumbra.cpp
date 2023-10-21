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
#include "headers/clickable.h"
#include "headers/billboard.h"
#include "headers/portal.h"

#include "common/Linux/MicroGlut.h"
#include "common/GL_utilities.h"
#include "common/VectorUtils4.h"
#include "common/LittleOBJLoader.h"
#include "common/LoadTGA.h"

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
GLuint skyboxShaders, terrainShaders, objectsShaders, pickingShaders, plainShaders, heightShaders, billboardShaders;

// Intermediary to set the look-at function in the player camera to be used by openGL
void bindCamera(int x, int y) {playerCamera->calcLookAt(x, y);}

// Another intermediary for the mouse clicking function, used for picking
void bindMouseFunc(int button, int state, int x, int y) {playerCamera->mouseFunc(button, state, x, y);}

// TODO: make classes/structs for light source (check), game objects (check), portals etc.

Terrain * terrain;

Heightmap * heightmap;

std::vector<Light*> lights;

Skybox * skybox;

/* separating out clickable objects into a separate list will be needed and practical since most objects 
   will not be clickable like walls and floor tiles and the floating point precision isn't endless*/
std::vector<GameObject *> objects; 
std::vector<Clickable *> clickableObjects;
std::vector<Billboard *> billboards;
std::vector<Portal * > portals;

// Picking stuff
struct PixelInfo
    {
        float objectID = 0.0; 
        float junk1 = 0.0; 
        float junk2 = 0.0; // these two might be useful for something in the future

        void Print()
        {
            printf("Object %f\n", objectID);
        }
    };


// check
bool once = false;
// render to screenbuffer and save as TGA to use as heightMap
void heightRender() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, 1024, 1024);

	glUseProgram(heightShaders);
	// place the camera in the center of the terrain looking down
	glUniformMatrix4fv(glGetUniformLocation(pickingShaders, "viewMatrix"), 1, GL_TRUE, lookAtv(vec3(128, 128, 128), vec3(128, 0, 128), vec3(-1, 0, 0)).m);

	terrain->setShader(heightShaders);
	terrain->Draw();
	terrain->setShader(terrainShaders);

	// loop through all objects
	for (uint i = 0; i < objects.size(); i++){
		objects.at(i)->setShader(heightShaders);
		objects.at(i)->Draw();
		objects.at(i)->setShader(terrainShaders);
	}

	// loop through all (clickable) objects
	for (uint i = 0; i < clickableObjects.size(); i++){
		clickableObjects.at(i)->setShader(heightShaders);
		clickableObjects.at(i)->Draw();
		clickableObjects.at(i)->setShader(terrainShaders);
	}

	SaveFramebufferToTGA("assets/tga/heightmap.tga", 0,0, 1024, 1024);
	glViewport(0, 0, 800, 800); 

	heightmap = new Heightmap("assets/tga/heightmap.tga");
}

// render to screenbuffer for picking purposes (because fbo:s and glReadPixels don't mix)
void pickingRender() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// upload viewmatrix and the number of (clickable) objects to the picking shaders
	glUseProgram(pickingShaders);
	glUniformMatrix4fv(glGetUniformLocation(pickingShaders, "viewMatrix"), 1, GL_TRUE, playerCamera->world2viewMatrix.m);
	glUniform1f(glGetUniformLocation(pickingShaders, "numObjects"), clickableObjects.size()); // the secret sauce that made it work

	// loop through all (clickable) objects, upload their index in the list to the picking shaders and draw it
	for (uint i = 0; i < clickableObjects.size(); i++){
		clickableObjects.at(i)->setShader(pickingShaders);
		glUniform1f(glGetUniformLocation(pickingShaders, "objectIndex"), i + 1);
		clickableObjects.at(i)->Draw();
		clickableObjects.at(i)->setShader(terrainShaders);
	}

	// read the red color channel (objectID) of the pixel where the player clicked, and set it as the pickedObject (it's float because that's what I got to work)
	PixelInfo pixel;
	glReadPixels(playerCamera->leftButton.x, WIN_H - playerCamera->leftButton.y - 1, 1, 1,  GL_RGB, GL_FLOAT, &pixel);
	int pickedObject = int(round(pixel.objectID*clickableObjects.size()) - 1);
	if (pickedObject != -1) {
		clickableObjects.at(pickedObject)->onClick();
	}

	// we have used up the click, reset
	playerCamera->leftButton.isPressed = false;
}

// the render call to render to the screen
void finalRender() {
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(skyboxShaders);
	glUniformMatrix4fv(glGetUniformLocation(skyboxShaders, "viewMatrix"), 1, GL_TRUE, playerCamera->world2viewMatrix.m);

	glUseProgram(terrainShaders);
	glUniformMatrix4fv(glGetUniformLocation(terrainShaders, "viewMatrix"), 1, GL_TRUE, playerCamera->world2viewMatrix.m);

	glUseProgram(objectsShaders);
	glUniformMatrix4fv(glGetUniformLocation(objectsShaders, "viewMatrix"), 1, GL_TRUE, playerCamera->world2viewMatrix.m);

	glUseProgram(heightShaders);
	glUniformMatrix4fv(glGetUniformLocation(heightShaders, "viewMatrix"), 1, GL_TRUE, playerCamera->world2viewMatrix.m);

	glUseProgram(billboardShaders);
	glUniformMatrix4fv(glGetUniformLocation(billboardShaders, "viewMatrix"), 1, GL_TRUE, playerCamera->world2viewMatrix.m);
	glUniform3fv(glGetUniformLocation(billboardShaders, "cameraPos"), 1, &(playerCamera->cameraPos.x));

	//Upload lights to objects and terrain shaders
	for(size_t i = 0; i < lights.size(); i++) {
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
	for (size_t i = 0; i < objects.size(); i++){
		objects.at(i)->Draw();
	}

	// Draw clickable objects - need to cull away some objects (cell based)
	for (size_t i = 0; i < clickableObjects.size(); i++){
		clickableObjects.at(i)->Draw();
	}

	for (size_t i = 0; i < billboards.size(); i++){
		billboards.at(i)->Draw();
	}
}

/*      ___          _   _   
 *     |_ _|  _ _   |_| | |_ 
 *      | |  | ' \  | | |  _|
 *     |___| |_||_| |_|  \__|
 */
// Initiates a lot of values, loads models and shaders etc.
void init(void) {
    // GL inits
	glClearColor(0.0,0.2,0.75,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//printError("GL inits");

	// Load and compile shaders to use (One for skybox, one for terrain, one for game objects, one for picking)
	skyboxShaders  = loadShaders("shaders/skybox.vert",  "shaders/skybox.frag" );
	terrainShaders = loadShaders("shaders/terrain.vert", "shaders/terrain.frag");
	objectsShaders = loadShaders("shaders/objects.vert", "shaders/objects.frag");
	pickingShaders = loadShaders("shaders/picking.vert", "shaders/picking.frag");
	plainShaders = loadShaders("shaders/plain.vert", "shaders/plain.frag");
	heightShaders = loadShaders("shaders/heightmap.vert", "shaders/heightmap.frag");
	billboardShaders = loadShadersG("shaders/billboard.vert", "shaders/billboard.frag", "shaders/billboard.geom");

	// Set the projection to be perspective projection with a frustum
	playerCamera->setProjMat(frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 250.0)); // probably make these defines so i can make planes for frustum culling
	
	
	glUseProgram(skyboxShaders);
	glUniformMatrix4fv(glGetUniformLocation(skyboxShaders, "projMatrix"), 1, GL_TRUE, playerCamera->projectionMatrix.m);

	glUseProgram(terrainShaders);
	glUniformMatrix4fv(glGetUniformLocation(terrainShaders, "projMatrix"), 1, GL_TRUE, playerCamera->projectionMatrix.m);

	glUseProgram(objectsShaders);
	glUniformMatrix4fv(glGetUniformLocation(objectsShaders, "projMatrix"), 1, GL_TRUE, playerCamera->projectionMatrix.m);

	glUseProgram(pickingShaders);
	glUniformMatrix4fv(glGetUniformLocation(pickingShaders, "projMatrix"), 1, GL_TRUE, playerCamera->projectionMatrix.m);

	glUseProgram(billboardShaders);
	glUniformMatrix4fv(glGetUniformLocation(billboardShaders, "projMatrix"), 1, GL_TRUE, playerCamera->projectionMatrix.m);

	// Initiate skybox - make into a class/objecttype?

	Texture * skyboxTex = new Texture(TGA + "SkyBoxFull.tga", "", "");

	skybox = new Skybox("assets/obj/skyboxfull.obj", skyboxTex, skyboxShaders);

    // Initiate terrain - will probably make a separate file for this

	Texture * terrainTex = new Texture(TGA + "brick02.tga", TGA + "brick02_normal.tga", TGA + "brick02_normal.tga");

	std::array<Texture *, NUM_TEX> texList {terrainTex};

	terrain = new Terrain("assets/tga/fft-terrain.tga", texList, terrainShaders); // 1204c so big it lags - frustum culling pls lol

	heightmap = new Heightmap("assets/tga/heightmap.tga");


	// creating navigationMap
	int dim = terrain->getMapWidth() / 2;
	mat4 orthoMatrix = ortho(-dim + 1, dim, -dim + 1, dim, 0.2f, 250.0); // 128 because it's half of map dimension
	glUseProgram(heightShaders);
	glUniformMatrix4fv(glGetUniformLocation(heightShaders, "projMatrix"), 1, GL_TRUE, orthoMatrix.m);

    // Initiate game objects (models, textures, material properties etc.) - portals should fit in here somewhere

	// just a test - ceiling has backside, rendering z-buffer from above to fbo to get heightmap for player walking not gonna work - maybe if split all ceils off to a draw after writing to fbo?
	//also this should be instancable

	GLuint tShaders = terrainShaders;
	std::array<Texture *, NUM_TEX> ttex = {new Texture(CASTLETGA + "Floor02.tga", "", "")};
	std::array<Texture *, 1> doortex = {new Texture(CASTLETGA + "WoodDoor.tga", "", "")};
	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(10,4, 1)));
	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(6, 4, 1)));
	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(2, 4, 1)));
	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(10,4, 5)));
	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(6, 4, 5)));
	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(2, 4, 5)));
	objects.push_back(new GameObject(CASTLEOBJ + "Stairs01.obj",ttex, tShaders, T(6, 2.9, 8.6)));
	Texture * bushtex = new Texture(TGA + "bush.tga", "", "");
	Billboard * bushes = new Billboard(bushtex, billboardShaders);
	bushes->setHeightmap(heightmap);
	bushes->generateVerts(100000);
	billboards.push_back(bushes);
	// objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(10,5, 1)));
	// objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(6, 5, 1)));
	// objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(2, 5, 1)));
	// objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(10,5, 5)));
	// objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(6, 5, 5)));
	// objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(2, 5, 5)));
	// objects.push_back(new GameObject(CASTLEOBJ + "Stairs01.obj",ttex, tShaders, T(6, 3.9, 8.6)));
	// objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(10,6, 1)));
	// objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(6, 6, 1)));
	// objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(2, 6, 1)));
	// objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(10,6, 5)));
	// objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(6, 6, 5)));
	// objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(2, 6, 5)));
	// objects.push_back(new GameObject(CASTLEOBJ + "Stairs01.obj",ttex, tShaders, T(6, 4.9, 8.6)));
	// objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(10,7, 1)));
	// objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(6, 7, 1)));
	// objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(2, 7, 1)));
	// objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(10,7, 5)));
	// objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(6, 7, 5)));
	// objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(2, 7, 5)));
	// objects.push_back(new GameObject(CASTLEOBJ + "Stairs01.obj",ttex, tShaders, T(6, 5.9, 8.6)));

	// Clickable * click = new Clickable(CASTLEOBJ + "Door01.obj", doortex, tShaders, T(12,3,10));
	// click->setOnCLick([click](){click->setTransform(click->getTransform()*Ry(M_PI/2.0));});
	// clickableObjects.push_back(click);

	// render world from above and save to tga for height map with objects in it


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

	if(!once) {
		//heightRender();
		once = true;
	}

	// handle player input and update camera
	playerCamera->checkKeyboardInput(heightmap);
	playerCamera->updateWorld2view();// precalc inverse view*model mat instead of doing expensively in shader and save to models.

	// ---- PICKING PHASE ----
	if (playerCamera->leftButton.isPressed) {
		// Optimise out the picking phase if player isn't trying to click something
		pickingRender();
	}

	// ---- RENDER PHASE ----
	finalRender(); 

	//printf("-----\n cam coords: %f:%f:%f\n-------\n", playerCamera->cameraPos.x, playerCamera->cameraPos.y, playerCamera->cameraPos.z);

	//printf("-----\n lookAt pos: %f:%f:%f\n-------\n", playerCamera->lookAtPos.x, playerCamera->lookAtPos.y, playerCamera->lookAtPos.z);

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

	glutCreateWindow("Penumbra Definitive Edition GOTY Edition Collector's Edition");
	glutDisplayFunc(display);
	init();

	glutRepeatingTimer(20);
    glutPassiveMotionFunc(&bindCamera);
	glutMouseFunc(&bindMouseFunc);

	glutMainLoop();
	exit(0);
}