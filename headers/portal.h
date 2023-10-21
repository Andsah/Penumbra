#ifndef PORTAL_H
#define PORTAL_H

#include "../common/Linux/MicroGlut.h"
#include "../common/GL_utilities.h"
#include "../common/VectorUtils4.h"
#include "../common/LittleOBJLoader.h"
#include "../common/LoadTGA.h"

#include "gameObject.h"

// A class for representing a portal in the game
class Portal { 

protected:

    // A reference to the other portal object
    Portal * linkedPortal;

    // This portal's view (virtual camera view)
    mat4 portalCam;

    // The portal's translation, rotation, and scale combined, expressed in world coordinates
    mat4 transformMatrix;

    std::array<vec4, 4> portalVerts = {
        vec4(-1, -1, 0, 1),
        vec4( 1, -1, 0, 1),
        vec4(-1,  1, 0, 1),
        vec4( 1,  1, 0, 1),
        };

    std::array<vec3, 2> portalIndices = {
        vec3(0, 1, 2),
        vec3(2, 1, 3)
    };
    
public:

    // Going to need a constructor
    Portal(GLuint shader);

    // calculates the portalView of this portal based on where player is in relation to the portal and where the other portal is
    mat4 makePortalView(mat4 playerViewMatrix);

    Portal * getOtherEnd();

    void setOtherEnd(Portal * otherEnd);

    void setTransform(mat4 newTransform);

    mat4 getTransform();

};

#endif