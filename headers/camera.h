#ifndef CAMERA_H
#define CAMERA_H

// window width
#define WIN_W 1024
// window height
#define WIN_H 1024

#include "../common/Linux/MicroGlut.h"
#include "../common/GL_utilities.h"
#include "../common/VectorUtils4.h"
#include "../common/LittleOBJLoader.h"
#include "../common/LoadTGA.h"

#include "heightmap.h"

// maybe rename this? portals might need camera objects too that are more basic
// Struct for handling variables and functions for controlling the players movement and camera
struct Camera {

    // Matrix for projecting the view onto the screen(?)
    mat4 projectionMatrix;
    // Matrix for translating the worldspace into view-coordinates
    mat4 viewMatrix;
    // The sensitivity of the mouse-controlled camera rotation

    //camera position

    vec3 cameraPos;
    float cameraSensitivity {0.0005f};

    // rework camera to use pitch and yaw and be limited vertically. also the whole walk on the plane thing
    // maybe raycast straight down and pass object and x,y coords to calc height?

    // Set ProjectionMatrix
    void setProjMat(mat4 projMat);
    // Update the world2view matrix (done every tick)
    void setViewMat(mat4 viewMat);
};
#endif