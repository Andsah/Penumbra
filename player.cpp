#include "headers/player.h"

Player::Player(float health): Actor(health){
    playerCam = new Camera();
}

void Player::calcLookAt(int x, int y)
{
    if (!pause) {
        vec3 forward = normalize(lookAtPos - playerPos);
        vec3 rightVec = normalize(CrossProduct(forward, upVec));
        vec3 trueUp = normalize(CrossProduct(forward, rightVec));
        lookAtPos = playerPos + normalize(forward + ((x - WIN_W/2) * rightVec + (y - WIN_H/2) * trueUp)*playerCam->cameraSensitivity); 
        // find way to let screen dimensions be arbitrary
        // and why its not working on my windows/ubuntu setup - or thinlinc - got ubuntu now so no worries
        glutWarpPointer(WIN_W/2, WIN_H/2);
    }
}

void Player::checkKeyboardInput(Heightmap * hMap) {
    vec3 forward = normalize(lookAtPos - playerPos);
    vec3 rightVec = normalize(CrossProduct(forward, upVec)); 

    moveDist = 1.0f;

    if(!flyingCamera) {
    moveDist = 0.1f;

    //place player on terrain
    if(!doneOncePerCamSwitch){
    playerPos.y = 2.0 + hMap->calcHeight(playerPos.x, playerPos.z);
    doneOncePerCamSwitch = true;
    }

    vec3 forwardOld = forward, rightOld = rightVec;
    // kinda dirty ngl and only works on terrain - how to make work on placed meshes?
    forward = normalize(vec3(playerPos.x + (forward * moveDist).x, hMap->calcHeight(playerPos.x + (forward * moveDist).x, playerPos.z +(forward * moveDist).z), playerPos.z + (forward * moveDist).z)
     - vec3(playerPos.x, hMap->calcHeight(playerPos.x, playerPos.z), playerPos.z));
    
    rightVec = normalize(vec3(playerPos.x + (rightVec * moveDist).x, hMap->calcHeight(playerPos.x + (rightVec * moveDist).x, playerPos.z +(rightVec * moveDist).z), playerPos.z + (rightVec * moveDist).z)
     - vec3(playerPos.x, hMap->calcHeight(playerPos.x, playerPos.z), playerPos.z));

     if (abs(dot(forward, vec3(0,1,0))) > 0.8) {
        return;
        }
    }

    if (glutKeyIsDown('w')) { 
        playerPos = playerPos + forward * moveDist; 
        lookAtPos = lookAtPos + forward* moveDist;
    }
    if (glutKeyIsDown('a')) {
        playerPos = playerPos - rightVec * moveDist;
        lookAtPos = lookAtPos - rightVec * moveDist;
    }
    if (glutKeyIsDown('s')) {
        playerPos = playerPos - forward * moveDist;
        lookAtPos = lookAtPos - forward * moveDist;
    }
    if (glutKeyIsDown('d')) {
        playerPos = playerPos + rightVec * moveDist;
        lookAtPos = lookAtPos + rightVec * moveDist;
    }

    // update camera so it tags along
    playerCam->cameraPos = playerPos;

    // other controls

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

void Player::mouseFunc(int button, int state, int x, int y) {

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

Camera * Player::getPlayerCam() {
    return playerCam;
}

void Player::updatePlayerCam(){
    playerCam->setViewMat(lookAtv(playerPos, lookAtPos, upVec));
}

int Player::x()
{
    return leftButton.x;
}

int Player::y() {
    return leftButton.y;
}

bool Player::isPressed() {
    return leftButton.isPressed;
}

void Player::resetPressed() {
    leftButton.isPressed = false;
}
