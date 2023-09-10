#ifndef PORTAL_H
#define PORTAL_H

#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

#include "gameObject.h"

// A class for representing a portal in the game
class Portal: public GameObject{ 

protected:

    // A reference to the other portal object
    Portal * linkedPortal;

    // This portal's view (virtual camera view)
    mat4 portalview;
    
public:

    // Going to need a constructor
    Portal(const char * objFile, std::array<Texture *,NUM_TEX> textureFiles, GLuint shader);

    // returns a reference to the other portal
    Portal * getPortal();

    // set the portal that will be linked to this portal
    void setPortal(Portal * otherPortal);

};

#endif