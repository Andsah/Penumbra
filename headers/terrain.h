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
    // Should I make it completely separate from GameObject? Maybe inherit from common parent? Drawable interface?

    // Separate splatmap?

    // Initializes the terrain object

    int mapWidth;
    int mapHeight;
public:
    Terrain(std::string heightMap, std::array<Texture *,NUM_TEX> textureFiles, GLuint shader);

    // A helper function for generating vertices, normals, texCoords and indices from a heightmap
    Model * generateTerrain(TextureData * hMap);

    int getMapWidth();

    int getMapHeight();

};

#endif