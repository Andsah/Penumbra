#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

#include "headers/terrain.h"
#include "headers/array2d.h"

Terrain::Terrain(const char * heightMap, std::array<Texture *,NUM_TEX> textures, GLuint shader): 
GameObject(textures, shader) {
    TextureData ttex;
    LoadTGATextureData(heightMap, &ttex);
    model = generateTerrain(&ttex);
    //printf("%f, %f, %f\n", model->vertexArray[899].x, model->vertexArray[899].y, model->vertexArray[899].z);
}

Model * Terrain::generateTerrain(TextureData * hMap) {
    unsigned int width = hMap->width;
    unsigned int height = hMap->height;

    // The number of vertices in the map is the height * width as one pixel represents one vertex
    int vertexCount = width * height;

    /*                                                                                                          | /       | /
       The triangle count is equal the product (height-1) * (width-1) * 2 for each axis,                        o---------o---
       you can think of a vertex as contributing to the triangle above (z-axis) or left of (x-axis) it.         |  1    / |
       Except for the topmost row and leftmost column, which dont contribute to a new triangle.                 |    /    |
       The multiplication by 2 comes from there being 2 triangles in each quad created.                         | /    2  | /
                                                                                                                o---------o---
    */
	int triangleCount = (width-1) * (height-1) * 2;

    // The coordinates of the terrain grid, also used for texture coordinates, normals etc.
	unsigned int x, z;

    Array2D<vec3> vArray = Array2D<vec3>(width, height);
    Array2D<vec3> nArray = Array2D<vec3>(width, height);
    Array2D<vec2> tArray = Array2D<vec2>(width, height);
    Array2D<GLuint> iArray = Array2D<GLuint>(width - 1, (height - 1) * 6); // product = triangleCount * 3

    for (x = 0; x < width; x++) {
        for (z = 0; z < height; z++) {
            // Initialise the vertex array, using the height map for y values
            vArray.Set(x, z, vec3(x, hMap->imageData[(x + z * width) * (hMap->bpp/8)] / 20.0, z)); // dividing by float seems to be important for interpolating
            // Initialise the texture array, standard coordinates
            tArray.Set(x, z, vec2(x, z));
        }
    }
    // Tangents and the total normalised sum of normals, and p
	vec3 up, down, left, right, upRight, downleft, nTot, p;

    // A second traverse through the height map to calculated normal vectors for every vertex
    for (x = 0; x < width; x++) {
        for (z = 0; z < height; z++) {
            // Think crossing with (1, 0, 0) in lab4 is wrong, should be (x+-1, z) - p or (x, z+-1) - p depending

            // Current vertex to calculate the normal for
            p = vArray.Get(x, z);

			up = vec3(0,0,0);
            down = vec3(0,0,0);
            left = vec3(0,0,0);
            right = vec3(0,0,0);
            upRight = vec3(0,0,0); 
            downleft = vec3(0,0,0); 
            nTot = vec3(0,0,0);

            // Check what faces can contribute to calculation of the normal vector at x, z
            // Think they are calculated in a way so they can never point down TODO: needs to be tested though
            // up and up right
            if (vArray.isValid(x, z + 1) && vArray.isValid(x + 1, z + 1)) {                     
                up = vArray.Get(x, z + 1) - p;
                upRight = vArray.Get(x + 1, z + 1) - p;
                nTot += normalize(cross(up, upRight));
            }
            // up right and right
            if (vArray.isValid(x + 1, z + 1) && vArray.isValid(x + 1, z)) {     
                upRight = vArray.Get(x + 1, z + 1) - p;
                right = vArray.Get(x + 1, z) - p;
                nTot += normalize(cross(upRight, right));                                                
            }
            // right and down
            if (vArray.isValid(x + 1, z) && vArray.isValid(x, z - 1)) {                         
                right = vArray.Get(x + 1, z) - p;
                down = vArray.Get(x, z - 1) - p;
                nTot += normalize(cross(right, down));
            }
            // down and down left
            if (vArray.isValid(x, z - 1) && vArray.isValid(x - 1, z - 1)) {
                down = vArray.Get(x, z - 1) - p;
                downleft = vArray.Get(x - 1, z - 1) - p;
                nTot += normalize(cross(down, downleft));
            }
            // down left and left                                                                        
            if (vArray.isValid(x - 1, z - 1) && vArray.isValid(x - 1, z)) {                     
                downleft = vArray.Get(x - 1, z - 1) - p;   
                left = vArray.Get(x - 1, z) - p;
                nTot += normalize(cross(downleft, left));
            }
            // left and up
            if (vArray.isValid(x - 1, z) && vArray.isValid(x, z + 1)) {      
                left = vArray.Get(x - 1, z) - p;
                up = vArray.Get(x, z + 1) - p;
                nTot += normalize(cross(left, up));
            }

            // Normalise the summed normals and set the value in the normal array
            nArray.Set(x, z, normalize(nTot));
        }
    }

    // Create the indices needed to describe the triangles that make up the model
    for (x = 0; x < width - 1; x++) {
        for (z = 0; z < (height - 1); z++) { // are these triangle-diagonals leaning to the left instead? TODO: find out
            // Triangle 1
			iArray.Set((x + z * (width-1)) * 6 + 0, x + z * width);
            iArray.Set((x + z * (width-1)) * 6 + 1, x + (z + 1) * width);
            iArray.Set((x + z * (width-1)) * 6 + 2, x + 1 + z * width);
		    // Triangle 2
            iArray.Set((x + z * (width-1)) * 6 + 3, x + 1 + z * width);
            iArray.Set((x + z * (width-1)) * 6 + 4, x + (z + 1) * width);
            iArray.Set((x + z * (width-1)) * 6 + 5, x + 1 + (z + 1) * width);
        }
    }

    //iArray.PrintFloat();


    Model* terrainModel = LoadDataToModel(
			vArray.GetBaseAddr(),
			nArray.GetBaseAddr(),
			tArray.GetBaseAddr(),
			NULL,
			iArray.GetBaseAddr(),
			vertexCount,
			triangleCount*3);

	return terrainModel;


}