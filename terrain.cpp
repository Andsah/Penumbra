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

    // vertexArray
    Array2D<vec3> vArray = Array2D<vec3>(width, height);
    // normalArray
    Array2D<vec3> nArray = Array2D<vec3>(width, height);
    // textureArray"
    Array2D<vec2> tArray = Array2D<vec2>(width, height);
    // make tanArray and BiArray for the tangent and bitangent
    Array2D<vec3> tanArray = Array2D<vec3>(width, height);
    Array2D<vec3> biArray = Array2D<vec3>(width, height);
    //indexArray
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
        for (z = 0; z < (height - 1); z++) {
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

    // Create the tangents and bitangents for each normal vector - maybe extract this to a function so it works for everything

    // texture coordinates - these are like always the ones right? might need to use tArray - these do seem to work though!
    vec2 uv1(0.0, 1.0);
    vec2 uv2(0.0, 0.0);
    vec2 uv3(1.0, 0.0);
    vec2 uv4(1.0, 1.0);

    // for each triangle in the model, get the delta_x and delta_y aka the edges from the first node to the two other nodes in the triangle
    for(int i = 0; i < iArray.GetSize() - 2; i += 3) {

        vec3 tangent = vec3(0,0,0);
        vec3 bitangent = vec3(0,0,0);

        // get the deltas in the triangle and the texture coordinates 
        vec3 edge1 = vArray.Get(iArray.Get(i + 1)) - vArray.Get(iArray.Get(i));
        vec3 edge2 = vArray.Get(iArray.Get(i + 2)) - vArray.Get(iArray.Get(i));
        vec2 deltaUV1 = uv2 - uv1;
        vec2 deltaUV2 = uv3 - uv1;

        // Tangent and bitangent calculations
        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

        // set the tangent for the 3 vertices
        tanArray.Set(iArray.Get(i), tangent);
        tanArray.Set(iArray.Get(i + 1), tangent);
        tanArray.Set(iArray.Get(i + 2), tangent);

        // set the bitangent for the 3 vertices
        biArray.Set(iArray.Get(i), bitangent);
        biArray.Set(iArray.Get(i + 1), bitangent);
        biArray.Set(iArray.Get(i + 2), bitangent);
    }


    // Need to edit the model to take tangent and bitangent


    Model* terrainModel = LoadDataToModel(
			vArray.GetBaseAddr(),
			nArray.GetBaseAddr(),
			tArray.GetBaseAddr(),
			NULL,
			iArray.GetBaseAddr(),
            tanArray.GetBaseAddr(),
            biArray.GetBaseAddr(),
			vertexCount,
			triangleCount*3);

	return terrainModel;


}