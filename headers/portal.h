#ifndef PORTAL_H
#define PORTAL_H

#include "../common/Linux/MicroGlut.h"
#include "../common/GL_utilities.h"
#include "../common/VectorUtils4.h"
#include "../common/LittleOBJLoader.h"
#include "../common/LoadTGA.h"

#include <array>
#include "camera.h"

// A class for representing a portal in the game
class Portal { 

protected:


    GLuint shader;

    Portal * otherEnd;

    // The portal's translation (position), rotation, and scale combined (aka model matrix)
    mat4 transformMatrix;

    vec3 position;

    mat4 rotation;

    Model* portalModel;

    void setTransform(mat4 transform);
    
public:

    // Going to need a constructor
    Portal(GLuint shader, vec3 position = vec3(0, 0, 0), mat4 rotation = Ry(0));

    // calculates the portalView of this portal based on where player is in relation to the portal and where the other portal is
    mat4 makePortalView(mat4 playerViewMatrix, mat4 rot, mat4 otherEndMat);

    void setOtherEnd(Portal * otherEnd);

    Portal * getOtherEnd();

    mat4 getTransform();

    void setRotation(mat4 rot);

    mat4 getRotation();

    void setPosition(vec3 pos);

    vec3 getPosition();

    void draw(const mat4 & viewMat, const mat4 & projMat);

    const mat4 clipProjMat(const mat4 & viewMat, const mat4 & projMat);

};
#endif