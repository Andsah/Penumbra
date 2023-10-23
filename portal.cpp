#include "common/Linux/MicroGlut.h"
#include "common/GL_utilities.h"
#include "common/VectorUtils4.h"
#include "common/LittleOBJLoader.h"
#include "common/LoadTGA.h"

#include "headers/portal.h"

Portal::Portal(GLuint shader, vec3 position, mat4 rotation): 
shader(shader), position(position), rotation(rotation) {

    setTransform(T(position.x,position.y, position.z) * rotation);
    portalModel = LoadModel("assets/interiorcastle/obj/Wall01.obj");
}

mat4 Portal::makePortalView(mat4 playerViewMatrix, mat4 otherEndMat) {
        mat4 mat = playerViewMatrix * transformMatrix * Ry(M_PI) * rotation * inverse(otherEndMat);
        return mat;
    }

void Portal::setOtherEnd(Portal * otherEnd) {
    otherEnd = otherEnd;
}

Portal * Portal::getOtherEnd() {
    return otherEnd;
}

void Portal::setTransform(mat4 transform) { // change this back to just transform, make a get position func that extracts the position from the full mat
    transformMatrix = transform;
}

mat4 Portal::getTransform() {
    return transformMatrix;
}
void Portal::draw(const mat4 & viewMat, const mat4 & projMat) {
    glUseProgram(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelMatrix"), 1, GL_TRUE, transformMatrix.m);
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewMatrix"), 1, GL_TRUE, viewMat.m);
	glUniformMatrix4fv(glGetUniformLocation(shader, "projMatrix"), 1, GL_TRUE, projMat.m);

    DrawModel(portalModel, shader, "inPosition", NULL, NULL);
}

const mat4 Portal::clipProjMat(const mat4 &viewMat, const mat4 &projMat) {   

    float dist = Norm(position);
	vec4 clipPlane(rotation * vec3(0.0f, 0.0f, -1.0f), dist);
	clipPlane = inverse(transpose(viewMat)) * clipPlane;

	if (clipPlane.w > 0.0f)
		return projMat; 

	vec4 q = inverse(projMat) * vec4(sign(clipPlane.x), sign(clipPlane.y), 1.0f, 1.0f);

	vec4 c = clipPlane * (2.0f / (dot(clipPlane, q)));

	mat4 newProj = projMat;
	// third row = clip plane - fourth row
    newProj.m[8] = c.x - newProj.m[12];
    newProj.m[9] = c.y - newProj.m[13];
    newProj.m[10] = c.z - newProj.m[14];
    newProj.m[11] = c.w - newProj.m[15];

	return newProj;
}
