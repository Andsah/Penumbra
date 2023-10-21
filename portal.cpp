#include "headers/portal.h"

Portal::Portal(GLuint shader) {
    
}

mat4 Portal::makePortalView(mat4 playerViewMatrix) {
        mat4 mv = playerViewMatrix * transformMatrix;
        return mv * Ry(M_1_PI) * inverse(linkedPortal->getTransform());
    }

Portal * Portal::getOtherEnd() {

    return linkedPortal;
}

void Portal::setOtherEnd(Portal * otherEnd) {
    linkedPortal = otherEnd;
}

void Portal::setTransform(mat4 newTransform) {
    transformMatrix = newTransform;
}

mat4 Portal::getTransform() {
    return transformMatrix;
}
