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
#include "headers/player.h"

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
Player * player = new Player(200);

// Shaders
GLuint skyboxShaders, terrainShaders, objectsShaders, pickingShaders, portalShaders, heightShaders, billboardShaders;

// Intermediary to set the look-at function in the player camera to be used by openGL
void bindCamera(int x, int y) {player->calcLookAt(x, y);}

// Another intermediary for the mouse clicking function, used for picking
void bindMouseFunc(int button, int state, int x, int y) {player->mouseFunc(button, state, x, y);}

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
std::vector<std::array<Portal *, 2>> portals;

void addPortalPair(vec3 pos1, vec3 pos2, mat4 r1, mat4 r2) {
	Portal * port1 = new Portal(portalShaders, pos1, r1);
	Portal * port2 = new Portal(portalShaders, pos2, r2);

	portals.push_back({port1, port2});
}

int maxRecursion = 1;
int testCase = 0;

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
	int dim = terrain->getMapWidth() / 2;
	mat4 orthoMatrix = ortho(-dim + 1, dim, -dim + 1, dim, 0.2f, 250.0); // 128 because it's half of map dimension
	mat4 aboveViewMatrix = lookAtv(vec3(128, 128, 128), vec3(128, 0, 128), vec3(-1, 0, 0));

	terrain->setShader(heightShaders);
	terrain->Draw(aboveViewMatrix, orthoMatrix);
	terrain->setShader(terrainShaders);

	// loop through all objects
	for (uint i = 0; i < objects.size(); i++){
		objects.at(i)->setShader(heightShaders);
		objects.at(i)->Draw(aboveViewMatrix, orthoMatrix);
		objects.at(i)->setShader(terrainShaders);
	}

	// loop through all (clickable) objects
	for (uint i = 0; i < clickableObjects.size(); i++){
		clickableObjects.at(i)->setShader(heightShaders);
		clickableObjects.at(i)->Draw(aboveViewMatrix, orthoMatrix);
		clickableObjects.at(i)->setShader(terrainShaders);
	}

	SaveFramebufferToTGA("assets/tga/heightmap.tga", 0,0, 1024, 1024);

	heightmap = new Heightmap("assets/tga/heightmap.tga");
}

// render to screenbuffer for picking purposes (because fbo:s and glReadPixels don't mix)
void pickingRender() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// upload viewmatrix and the number of (clickable) objects to the picking shaders
	glUseProgram(pickingShaders);
	glUniform1f(glGetUniformLocation(pickingShaders, "numObjects"), clickableObjects.size()); // the secret sauce that made it work

	// loop through all (clickable) objects, upload their index in the list to the picking shaders and draw it
	for (uint i = 0; i < clickableObjects.size(); i++){
		clickableObjects.at(i)->setShader(pickingShaders);
		glUniform1f(glGetUniformLocation(pickingShaders, "objectIndex"), i + 1);
		clickableObjects.at(i)->Draw(player->getPlayerCam()->viewMatrix, player->getPlayerCam()->projectionMatrix);
		clickableObjects.at(i)->setShader(terrainShaders);
	}

	// read the red color channel (objectID) of the pixel where the player clicked, and set it as the pickedObject (it's float because that's what I got to work)
	PixelInfo pixel;
	glReadPixels(player->x(), WIN_H - player->y() - 1, 1, 1,  GL_RGB, GL_FLOAT, &pixel);
	int pickedObject = int(round(pixel.objectID*clickableObjects.size()) - 1);
	if (pickedObject != -1) {
		clickableObjects.at(pickedObject)->onClick();
	}

	// we have used up the click, reset
	player->resetPressed();
}

// the render call to render to the screen
void finalRender(Camera * pov) {
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
	portalShaders = loadShaders("shaders/portal.vert", "shaders/portal.frag");
	heightShaders = loadShaders("shaders/heightmap.vert", "shaders/heightmap.frag");
	billboardShaders = loadShadersG("shaders/billboard.vert", "shaders/billboard.frag", "shaders/billboard.geom");

	// Set the projection to be perspective projection with a frustum
	player->getPlayerCam()->setProjMat(frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 250.0)); // probably make these defines so i can make planes for frustum culling

	// Initiate skybox - make into a class/objecttype?

	Texture * skyboxTex = new Texture(TGA + "SkyBoxFull.tga", "", "");

	skybox = new Skybox("assets/obj/skyboxfull.obj", skyboxTex, skyboxShaders);

    // Initiate terrain - will probably make a separate file for this

	Texture * terrainTex = new Texture(TGA + "brick02.tga", TGA + "brick02_normal.tga", TGA + "brick02_normal.tga");

	std::array<Texture *, NUM_TEX> texList {terrainTex};

	terrain = new Terrain("assets/tga/fft-terrain.tga", texList, terrainShaders); // 1204c so big it lags - frustum culling pls lol

	heightmap = new Heightmap("assets/tga/heightmap.tga");

    // Initiate game objects (models, textures, material properties etc.) - portals should fit in here somewhere

	Texture * bushtex = new Texture(TGA + "bush.tga", "", "");
	Billboard * bushes = new Billboard(bushtex, billboardShaders);
	bushes->setHeightmap(heightmap);

	if(testCase == 0) {
		bushes->generateVerts(1000000);
		maxRecursion = 1;
	}
	else if (testCase == 1) {
		bushes->generateVerts(1000);
		addPortalPair(vec3(4,5,23), vec3(20, 8, 10), Ry(M_PI_4f), Ry(M_PI_2f));
		addPortalPair(vec3(90, 2, 152), vec3(124, 2, 130), Ry(M_PI), Ry(0));
		addPortalPair(vec3(110, 2, 121), vec3(126, 2, 132), Ry(0), Ry(-M_PI_2));
		maxRecursion = 1;
	}
	else if (testCase == 2){
		addPortalPair(vec3(128, 2, 126), vec3(128, 2, 130), Ry(M_PI), Ry(0));
		maxRecursion = 4;
	}
	billboards.push_back(bushes);

	// just a test - ceiling has backside, rendering z-buffer from above to fbo to get heightmap for player walking not gonna work - maybe if split all ceils off to a draw after writing to fbo?
	//also this should be instancable

	GLuint tShaders = terrainShaders;
	std::array<Texture *, NUM_TEX> ttex = {new Texture(CASTLETGA + "Floor02.tga", "", "")};
	std::array<Texture *, NUM_TEX> doortex = {new Texture(CASTLETGA + "WoodDoor.tga", "", "")};
	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(10,4, 1)));
	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(6, 4, 1)));
	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(2, 4, 1)));
	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(10,4, 5)));
	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(6, 4, 5)));
	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(2, 4, 5)));
	objects.push_back(new GameObject(CASTLEOBJ + "Stairs01.obj",ttex, tShaders, T(6, 2.9, 8.6)));

	std::array<Texture *, NUM_TEX> ctex = {new Texture(CASTLETGA + "Cauldron01.tga", "", "")};
	std::array<Texture *, NUM_TEX> btex = {new Texture(CASTLETGA + "BookshelfLarge.tga", "", "")};

	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(128,0, 128)));

	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(114,0, 115)));
	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(110, 0, 115)));
	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(106, 0, 115)));

	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(114,0, 119)));
	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(110, 0, 119)));
	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(106, 0, 119)));

	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(114,0, 123)));
	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(110, 0, 123)));
	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(106, 0, 123)));

	objects.push_back(new GameObject(CASTLEOBJ + "Cauldron01.obj", ctex, tShaders, T(110, 1, 119)));

	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(94, 0, 150)));
	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(90, 0, 150)));
	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(86, 0, 150)));

	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(94, 0, 154)));
	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(90, 0, 154)));
	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(86, 0, 154)));

	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(94, 0, 158)));
	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(90, 0, 158)));
	objects.push_back(new GameObject(CASTLEOBJ + "Floor02.obj", ttex, tShaders, T(86, 0, 158)));

	objects.push_back(new GameObject(CASTLEOBJ + "BookshelfLarge.obj", btex, tShaders, T(90, 2, 154)));

	Clickable * click = new Clickable(CASTLEOBJ + "Door01.obj", doortex, tShaders, T(12, 3, 10));
	click->setOnCLick([click](){click->setTransform(click->getTransform()*Ry(M_PI/2.0));});
	clickableObjects.push_back(click);

	// render world from above and save to tga for height map with objects in it


    // Initiate lighting objects - should make files with placement data for lights and objects aka level info
	Light * tL1 = new Light(vec3(1.0f,0.0f,0.0f), vec3(0.1f,0.3f,0.3f), vec3(100, 10, 200), true);
	Light * tL2 = new Light(vec3(0.0f,1.0f,0.5f), vec3(0.0f,0.0f,0.0f), vec3(3, 6, 6), true);
	Light * tL4 = new Light(vec3(0.0f,1.0f,0.5f), vec3(0.0f,0.0f,0.0f), vec3(3, 6, 300), true);
	Light * tL5 = new Light(vec3(0.0f,1.0f,0.5f), vec3(0.0f,0.0f,0.0f), vec3(200, 14, 6), true);
	Light * tL6 = new Light(vec3(0.0f,1.0f,0.5f), vec3(0.0f,0.0f,0.0f), vec3(3, 50, 6), true);
	Light * tL7 = new Light(vec3(0.0f,1.0f,0.5f), vec3(0.0f,0.0f,0.0f), vec3(40, 40, 36), true);
	Light * tL8 = new Light(vec3(0.0f,1.0f,0.5f), vec3(0.0f,0.0f,0.0f), vec3(300, 16, 64), true);
	Light * tL9 = new Light(vec3(0.0f,1.0f,0.5f), vec3(0.0f,0.0f,0.0f), vec3(40, -13, 60), true);
	Light * tL3 = new Light(vec3(0.0f,1.0f,0.5f), vec3(0.0f,0.0f,0.0f), vec3(90, 10, 154), true);
	Light * tLa = new Light(vec3(0.0f,1.0f,0.5f), vec3(0.0f,0.0f,0.0f), vec3(110, 10, 109), true);

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

	glutRepeatingTimer(25);

}

void drawSkybox(const mat4 & viewMat, const mat4 & projMat) {
	// Draw skybox (remember to turn off depth buffer)
	mat4 t = getTranslationPart(inverse(viewMat));
	vec3 pos = vec3(t.m[3], t.m[7], t.m[11]);
	skybox->setTransform(T(pos.x, pos.y, pos.z));
	skybox->Draw(viewMat, projMat);
}

void drawNonPortals(const mat4 & viewMat, const mat4 & projMat)
{
	// Draw all objects that aren't portals
	//fill this in later probably take all from final render and place in here

	//Upload lights to objects and terrain shaders
	for(size_t i = 0; i < lights.size(); i++) {
		glUseProgram(terrainShaders);
		lights[i]->uploadLight(terrainShaders, i);
		//glUseProgram(objectsShaders);
		//lights[i]->uploadLight(objectsShaders, i);
	}

	mat4 t = getTranslationPart(inverse(viewMat));
	vec3 pos = vec3(t.m[3], t.m[7], t.m[11]);

	if(testCase == 0) {drawSkybox(viewMat, projMat);}

	// Draw terrain
	terrain->Draw(viewMat, projMat);

	// Draw objects - need to cull away some objects (cell based)
	for (size_t i = 0; i < objects.size(); i++){
		objects.at(i)->Draw(viewMat, projMat);
	}

	// Draw clickable objects - need to cull away some objects (cell based)
	for (size_t i = 0; i < clickableObjects.size(); i++){
		clickableObjects.at(i)->Draw(viewMat, projMat);
	}

	glUseProgram(billboardShaders);
	glUniform3fv(glGetUniformLocation(billboardShaders, "cameraPos"), 1, &(pos.x));

	for (size_t i = 0; i < billboards.size(); i++){
		billboards.at(i)->Draw(viewMat, projMat);
	}

}

void drawRecursivePortals(const mat4 & viewMat, const mat4 & projMat, size_t maxRecursionLevel, size_t recursionLevel) {

	for (uint i = 0; i < portals.size(); i++) {
		for (uint j = 0; j < 2; j++) {
			Portal * portal = portals.at(i)[j];
			Portal * otherEnd = portals.at(i)[1 - j];

			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
			glDepthMask(GL_FALSE);
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_NOTEQUAL, recursionLevel, 0xFF);
			glStencilOp(GL_INCR, GL_KEEP, GL_KEEP);
			glStencilMask(0xFF);

			portal->draw(viewMat, projMat);

			mat4 otherEndView = portal->makePortalView(viewMat, otherEnd->getRotation(), otherEnd->getTransform());

			// Base case, render inside of inner portal
			if (recursionLevel == maxRecursionLevel)
			{
				glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
				glDepthMask(GL_TRUE);
				glClear(GL_DEPTH_BUFFER_BIT);
				glEnable(GL_DEPTH_TEST);
				glEnable(GL_STENCIL_TEST);
				glStencilMask(0x00);
				glStencilFunc(GL_EQUAL, recursionLevel + 1, 0xFF);
				//drawSkybox(otherEndView, portal->clipProjMat(otherEndView, projMat));
				drawNonPortals(otherEndView, portal->clipProjMat(otherEndView, projMat));
			}
			else
			{
				// Recursion case
				drawRecursivePortals(otherEndView, portal->clipProjMat(otherEndView, projMat), maxRecursionLevel, recursionLevel + 1);
			}

			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
			glDepthMask(GL_FALSE);
			glEnable(GL_STENCIL_TEST);
			glStencilMask(0xFF);
			glStencilFunc(GL_NOTEQUAL, recursionLevel + 1, 0xFF);
			glStencilOp(GL_DECR, GL_KEEP, GL_KEEP);

			// Draw portal into stencil buffer
			portal->draw(viewMat, projMat);
		}
	}

	glDisable(GL_STENCIL_TEST);
	glStencilMask(0x00);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_ALWAYS);
	glClear(GL_DEPTH_BUFFER_BIT);

	// Draw portals into depth buffer
	for (uint i = 0; i < portals.size(); i++) {
		portals.at(i)[0]->draw(viewMat, projMat);
		portals.at(i)[1]->draw(viewMat, projMat);
	}

	glDepthFunc(GL_LESS);
	glEnable(GL_STENCIL_TEST);
	glStencilMask(0x00);
	glStencilFunc(GL_EQUAL, recursionLevel, 0xFF);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	drawNonPortals(viewMat, projMat);

}


/*      ___    _               _               
 *     |   \  |_|  ___  _ __  | |  __ _   _  _ 
 *     | |) | | | (_-< | '_ \ | | / _` | | || |
 *     |___/  |_| /__/ | .__/ |_| \__,_|  \_, |
 *                     |_|                |__/ 
 */
// Draws the image every tick
void display(void) {
	// clear the screen
	glStencilMask(0xFF);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glStencilMask(0x00);

	glEnable(GL_CULL_FACE);


	// bit dirty but this can't be done in init so...
	if(!once) {
		heightRender();
		once = true;
	}

	// handle player input and update camera
	player->checkKeyboardInput(heightmap);
	player->updatePlayerCam();// precalc inverse view*model mat instead of doing expensively in shader and save to models.

	// ---- PICKING PHASE ----
	if (player->isPressed()) {
		// Optimise out the picking phase if player isn't trying to click something
		pickingRender();
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ---- RENDER PHASE ----

	//rendering skybox here makes it not cover the portals but it doesn't move with the portals either

	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME)/ 1000;

	mat4 rot = Ry(sin(t * 0.2 * sqrt(2)));

	if(testCase == 1) {portals[0][0]->setRotation(rot);}

	drawRecursivePortals(player->getPlayerCam()->viewMatrix, player->getPlayerCam()->projectionMatrix, maxRecursion, 0);

	glutSwapBuffers();
}


/*      __  __          _        
 *     |  \/  |  __ _  |_|  _ _  
 *     | |\/| | / _` | | | | ' \ 
 *     |_|  |_| \__,_| |_| |_||_|
 */
int main(int argc, char **argv) {
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
	glutInitContextVersion(3, 2);
	glutInitWindowSize(WIN_W, WIN_H);

	glutCreateWindow("Penumbra Definitive Edition GOTY Edition Collector's Edition");
	glutDisplayFunc(display);
	init();

	glutRepeatingTimer(20);
    glutPassiveMotionFunc(&bindCamera);
	glutMouseFunc(&bindMouseFunc);

	glutMainLoop();
	exit(0);
}