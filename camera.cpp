#include "headers/camera.h"
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"
#include "headers/camera.h"

// collision detection will probably have to be done in here

void Camera::setProjMat(mat4 projMat)
{
    projectionMatrix = projMat;
}

void Camera::updateWorld2view()
{
    world2viewMatrix = lookAtv(cameraPos, lookAtPos, upVec); 
}

void Camera::calcLookAt(int x, int y)
{
    if (!pause) {
        vec3 forward = normalize(lookAtPos - cameraPos);
        vec3 rightVec = normalize(CrossProduct(forward, upVec));
        vec3 trueUp = normalize(CrossProduct(forward, rightVec));
        lookAtPos = cameraPos + normalize(forward + ((x - WIN_W/2) * rightVec + (y - WIN_H/2) * trueUp)*cameraSensitivity); 
        // find way to let screen dimensions be arbitrary
        // and why its not working on my windows/ubuntu setup - or thinlinc - got ubuntu now so no worries
        glutWarpPointer(WIN_W/2, WIN_H/2);
    }
}

void Camera::checkKeyboardInput(Terrain * terrain) {
    vec3 forward = normalize(lookAtPos - cameraPos);
    vec3 rightVec = normalize(CrossProduct(forward, upVec)); 

    moveDist = 1.0f;

    if(!flyingCamera) {
    moveDist = 0.1f;

    //place player on terrain
    if(!doneOncePerCamSwitch){
    cameraPos.y = 1.0 + calcHeight(terrain->getModel(),terrain->getMapWidth(), cameraPos.x, cameraPos.z);
    doneOncePerCamSwitch = true;
    }

    // kinda dirty ngl and only works on terrain - how to make work on placed meshes?
    forward = normalize(vec3(cameraPos.x + (forward * moveDist).x, calcHeight(terrain->getModel(), terrain->getMapWidth(), cameraPos.x + (forward * moveDist).x, cameraPos.z +(forward * moveDist).z), cameraPos.z + (forward * moveDist).z)
     - vec3(cameraPos.x, calcHeight(terrain->getModel(), terrain->getMapWidth(), cameraPos.x, cameraPos.z), cameraPos.z));
    
    rightVec = normalize(vec3(cameraPos.x + (rightVec * moveDist).x, calcHeight(terrain->getModel(), terrain->getMapWidth(), cameraPos.x + (rightVec * moveDist).x, cameraPos.z +(rightVec * moveDist).z), cameraPos.z + (rightVec * moveDist).z)
     - vec3(cameraPos.x, calcHeight(terrain->getModel(), terrain->getMapWidth(), cameraPos.x, cameraPos.z), cameraPos.z));
    }
    if (glutKeyIsDown('w')) { 
        cameraPos = cameraPos + forward * moveDist; 
        lookAtPos = lookAtPos + forward* moveDist;
    }
    if (glutKeyIsDown('a')) {
        cameraPos = cameraPos - rightVec * moveDist;
        lookAtPos = lookAtPos - rightVec * moveDist;
    }
    if (glutKeyIsDown('s')) {
        cameraPos = cameraPos - forward * moveDist;
        lookAtPos = lookAtPos - forward * moveDist;
    }
    if (glutKeyIsDown('d')) {
        cameraPos = cameraPos + rightVec * moveDist;
        lookAtPos = lookAtPos + rightVec * moveDist;
    }
    if (glutKeyIsDown('e')) {
        exit(0);
    }
    if (glutKeyIsDown('p')) {
        if (!pPressed) {
            pPressed = true;
            pause = !pause;
            if (pause) {glutShowCursor();}
            else {glutHideCursor();}
        }
    } 
    else {
        pPressed = false;
    }

    if (glutKeyIsDown('c')) {
        if (!cPressed) {
            cPressed = true;
            flyingCamera = !flyingCamera;
            doneOncePerCamSwitch = false;
        }
    } 
    else {
        cPressed = false;
    }
}

void Camera::mouseFunc(int button, int state, int x, int y) {
    // Getting these x,y in screen coordinates
    // Step 1: make into Norm Device coords
    float dev_x = (2.0f * x) / WIN_W - 1.0f;
    float dev_y = 1.0f - (2.0f * y) / WIN_H; // because 0 in screen coord is top of screen
    float dev_z = -1.0f; //forwards

    // Step 2: Homogeneous Clip Coordinates (w coord added)
    vec4 rayClip = vec4(dev_x, dev_y, dev_z, 1.0f);

    // Step 3: View Coordinates
    vec4 rayEye = inverse(projectionMatrix) * rayClip;
    rayEye = vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

    // Step 4: World Coordinates
    vec3 rayWorld =  normalize(vec3(inverse(world2viewMatrix) * rayEye));

    printVec3(rayWorld);

    // If I want to test against bounding spheres of objects, I need to:
    // 1: cull the list based on cell player is in. Movable objects need to change cell ref?
    // 2: get access to list of objects to check against. List would be in penumbra.cpp? 
    // Would I need player class with camera in it?
    // 3: test each relevant objects boundingsphere against rayWorld (O + Dt)
}

float Camera::calcHeight(Model *terrain, int terrainWidth, float x, float z)
{
	int x0 = x;
	int z0 = z;
	float xRemainder = x - x0;
	float zRemainder = z - z0;
	vec3 xVec;
	vec3 zVec;
	if ( zRemainder < xRemainder) {
		xVec = terrain->vertexArray[x0 + 1 + z0 * terrainWidth] - terrain->vertexArray[x0 + z0 * terrainWidth];
		zVec = terrain->vertexArray[x0 + 1 + (z0 + 1) * terrainWidth] - terrain->vertexArray[x0 + 1 + z0 * terrainWidth];
	}
	else {
		xVec = terrain->vertexArray[x0 + 1 + (z0 + 1) * terrainWidth] - terrain->vertexArray[x0 + (z0 + 1)* terrainWidth];
		zVec = terrain->vertexArray[x0 + (z0 + 1) * terrainWidth] - terrain->vertexArray[x0 + z0 * terrainWidth];
	}
	 return zRemainder * zVec.y + xRemainder * xVec.y + terrain->vertexArray[x0 + z0 * terrainWidth].y;

}
