// uses framework Cocoa
// uses framework OpenGL
#define MAIN
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"
#include "controller.h"

mat4 projectionMatrix;

// Default values - tweak in future
vec3 cameraPos = {20*sin(M_PI/2), 12.0f, 20*cos(M_PI/2)};
vec3 lookAtPos = {0.0f, 10.0f, 0.0f};
vec3 upVec = {0.0f, 1.0f, 0.0f};

Controller player_controlls = new Controller(&cameraPos, &lookAtPos, &upVec);

// make classes/structs for light source, game objects etc.