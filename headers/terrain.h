#ifndef TERRAIN_H
#define TERRAIN_H

#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

#include "gameObject.h"

// A class for handling the terrain in the game. Inherits from GameObject and extends it
class Terrain: public GameObject{

    // Separate splatmap?

    // Initializes the terrain object
    Terrain(const char heightMap[], std::array<const char *,16> textureFiles, GLuint shader);

    // A helper function for generating vertices and normals from a heightmap
    Model * generateTerrain(TextureData * hMap);

};

#endif