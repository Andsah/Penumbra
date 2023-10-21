#include "common/Linux/MicroGlut.h"
#include "common/GL_utilities.h"
#include "common/VectorUtils4.h"
#include "common/LittleOBJLoader.h"
#include "common/LoadTGA.h"

#include "headers/billboard.h"

#include <GLES3/gl3.h> // omg this gave me glGenVertexArrays and the others!

Billboard::Billboard(Texture *texture, GLuint shader):
shader(shader),
texture(texture){}

void Billboard::generateVerts(uint num) {

    for(uint i = 0; i < num; i++) {
        float x = (float)(rand()) / (float)(RAND_MAX) * 256;
        float z = (float)(rand()) / (float)(RAND_MAX) * 256;

        float y = hMap->calcHeight(x, z); // times 4 because you know what - no i do that in the calcHeight func already you stupid bih

        startVerts.push_back(vec3(x, y, z));
    }

    // VBO for vertex data
    glGenVertexArrays(1, &vertexArrayObjID); // ok finish the drawing call and add all buffers and arrays
    glBindVertexArray(vertexArrayObjID);

    glGenBuffers(1, &vertexBufferObjID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, startVerts.size()*sizeof(startVerts[0]), &startVerts[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(glGetAttribLocation(shader, "inPosition"));
    glVertexAttribPointer(glGetAttribLocation(shader, "inPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0); 

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Billboard::setShader(GLuint newShader) {
    shader = newShader;

    glVertexAttribPointer(glGetAttribLocation(shader, "inPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexAttribArray(glGetAttribLocation(shader, "inPosition"));
}

void Billboard::setHeightmap(Heightmap * newMap) {
    hMap = newMap;
}
void Billboard::Draw() {

    texture->loadTextureToShader(shader);

    glBindVertexArray(vertexArrayObjID);

    glDrawArrays(GL_POINTS, 0, (int)startVerts.size());

    glBindVertexArray(0);
}
