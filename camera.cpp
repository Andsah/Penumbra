#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"
#include "headers/camera.h"

#include "headers/array2d.h"

// collision detection will probably have to be done in here - nope this is a simple cam now

void Camera::setProjMat(mat4 projMat)
{
    projectionMatrix = projMat;
}

void Camera::setViewMat(mat4 viewMat)
{
    viewMatrix = viewMat;  
}
