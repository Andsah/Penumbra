#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"
#include "headers/camera.h"

#include "headers/array2d.h"

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

void Camera::checkKeyboardInput(Heightmap * hMap) {
    vec3 forward = normalize(lookAtPos - cameraPos);
    vec3 rightVec = normalize(CrossProduct(forward, upVec)); 

    moveDist = 1.0f;

    if(!flyingCamera) {
    moveDist = 0.1f;

    //place player on terrain
    if(!doneOncePerCamSwitch){
    cameraPos.y = 2.0 + hMap->calcHeight(cameraPos.x, cameraPos.z);
    doneOncePerCamSwitch = true;
    }

    vec3 forwardOld = forward, rightOld = rightVec;
    // kinda dirty ngl and only works on terrain - how to make work on placed meshes?
    forward = normalize(vec3(cameraPos.x + (forward * moveDist).x, hMap->calcHeight(cameraPos.x + (forward * moveDist).x, cameraPos.z +(forward * moveDist).z), cameraPos.z + (forward * moveDist).z)
     - vec3(cameraPos.x, hMap->calcHeight(cameraPos.x, cameraPos.z), cameraPos.z));
    
    rightVec = normalize(vec3(cameraPos.x + (rightVec * moveDist).x, hMap->calcHeight( cameraPos.x + (rightVec * moveDist).x, cameraPos.z +(rightVec * moveDist).z), cameraPos.z + (rightVec * moveDist).z)
     - vec3(cameraPos.x, hMap->calcHeight(cameraPos.x, cameraPos.z), cameraPos.z));

     if (abs(dot(forward, vec3(0,1,0))) > 0.8) {
        return;
        }
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

    // other controls

    if (glutKeyIsDown('o') && false) {
        cameraPos = vec3(128, 128, 128);
        lookAtPos = vec3(128, 0, 128);
        upVec = vec3(-1, 0, 0);
        glutReshapeWindow(256, 256);

    }
    else {
        glutReshapeWindow(WIN_W, WIN_H);
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

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (!leftButton.pressCheck) {
            leftButton.pressCheck = true;
            leftButton.isPressed = true;
            leftButton.x = x;
            leftButton.y = y;
        }
    } 
    else {
        leftButton.pressCheck = false;
    }
}
