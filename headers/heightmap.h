#ifndef HEIGHT_H
#define HEIGHT_H

#include "../common/Linux/MicroGlut.h"
#include "../common/GL_utilities.h"
#include "../common/VectorUtils4.h"
#include "../common/LittleOBJLoader.h"
#include "../common/LoadTGA.h"

#include "array2d.h"

#include <string>

// A class for representing the generated heightmap with terrain and objects on it
class Heightmap{ 
    
    vec3 * vArray;

    int mapWidth;

    int mapHeight;
public:

    Heightmap(std::string heightMap);

    // A helper function for generating vertices, normals, texCoords and indices from a heightmap
    void generateHeightmap(TextureData * hMap);

    vec3 * getArray();
    // Calculating the height y on the map at coords x,z
    float calcHeight(float x, float z);

    int getMapWidth();

    int getMapHeight();

};

#endif