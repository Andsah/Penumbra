#include "common/Linux/MicroGlut.h"
#include "common/GL_utilities.h"
#include "common/VectorUtils4.h"
#include "common/LittleOBJLoader.h"
#include "common/LoadTGA.h"

#include "headers/heightmap.h"
#include "headers/array2d.h"
#include <string>

Heightmap::Heightmap(std::string heightMap) {
    TextureData ttex;
    LoadTGATextureData(heightMap.c_str(), &ttex);
    generateHeightmap(&ttex);
}

void Heightmap::generateHeightmap(TextureData * hMap) {
    unsigned int width = hMap->width;
    unsigned int height = hMap->height;

    mapWidth = width;
    mapHeight = height;

    //printf("mapwidth and height: %u and %u \n", width, height);

    float min = 100, max = 0;

    // vertexArray
     Array2D<vec3> tempArray = Array2D<vec3>(width, height);

    for (uint x = 0; x < width; x++) {
        for (uint z = 0; z < height; z++) {
            // Initialise the vertex array, using the height map for y values
            tempArray.Set(x, z, vec3(x, hMap->imageData[(x + z * width) * (hMap->bpp/8)] / 20.0, z));
            if (tempArray.Get(x + z * width).y < min) {
				min = tempArray.Get(x + z * width).y;
			}
			else if (tempArray.Get(x + z * width).y > max) {
				max = tempArray.Get(x + z * width).y;
			}
        }
    }

    //printf("min: %f, max: %f\n", min, max);

    tempArray.transpose();

    vArray = tempArray.GetBaseAddr();
}

float Heightmap::calcHeight(float x, float z)
{
    int x0 = x * 4; // times 4 because the current resolution of the walkingMap is 4 times bigger than the terrainMap
	int z0 = z * 4;
	float xRemainder = x * 4 - x0;
	float zRemainder = z * 4 - z0;
	vec3 xVec;
	vec3 zVec;
	if ( zRemainder < xRemainder) {
		xVec = vArray[x0 + 1 + z0 * mapWidth] - vArray[x0 + z0 * mapWidth];
		zVec = vArray[x0 + 1 + (z0 + 1) * mapWidth] - vArray[x0 + 1 + z0 * mapWidth];
	}
	else {
		xVec = vArray[x0 + 1 + (z0 + 1) * mapWidth] - vArray[x0 + (z0 + 1)* mapWidth];
		zVec = vArray[x0 + (z0 + 1) * mapWidth] - vArray[x0 + z0 * mapWidth];
	}
	 return zRemainder * zVec.y + xRemainder * xVec.y + vArray[x0 + z0 * mapWidth].y;
}

vec3 *Heightmap::getArray()
{
    return vArray;
}
int Heightmap::getMapWidth()
{
    return mapWidth;
}

int Heightmap::getMapHeight()
{
    return mapHeight;
}