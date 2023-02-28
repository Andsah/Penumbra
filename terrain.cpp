#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

#include "terrain.h"

Terrain::Terrain(const char heightMap[], std::array<const char *,16> textureFiles, GLuint shader): 
GameObject(shader) {
    TextureData ttex;
    LoadTGATextureData(heightMap, &ttex);
    meshData = generateTerrain(&ttex);
    for(unsigned i = 0; i < textureFiles.size(); i++) {
        LoadTGATextureSimple(textureFiles[i], &textures[i]);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
    }
}