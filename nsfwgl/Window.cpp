#include "nsfw.h"

void nsfw::Window::init( unsigned width, unsigned height ) {
#pragma message ( __ERR__ "Should create and set an active windowing context. ONLY GLFW! No GL!")
}

void nsfw::Window::step() {
#pragma message ( __ERR__ "GLFW poll events and swap buffers is all that should really be here! No GL!")
}

void nsfw::Window::term() {
#pragma message ( __ERR__ )
}

float nsfw::Window::getTime() const {
#pragma message ( __ERR__ )
    return 0.0f;
}

bool nsfw::Window::getKey( unsigned k ) const {
#pragma message ( __ERR__ )
    return false;
}

bool nsfw::Window::getShouldClose() const {
#pragma message ( __ERR__ )
    return true;
}

unsigned nsfw::Window::getWidth() const {
#pragma message ( __ERR__ )
    return 0;
}

unsigned nsfw::Window::getHeight() const {
#pragma message ( __ERR__ )
    return 0;
}

glm::mat4 nsfw::Window::getTexelAdjustmentMatrix() const {
#pragma message ( __ERR__ "Implemented, not tested.")

    glm::vec3 texelSize = 1.0f / glm::vec3( width, height, 0 );
    glm::vec3 halfTexel = texelSize * 0.5f; // bottom left

    // scale up to the appropriate size, then shift it to line up w/center of texels
    return glm::translate( halfTexel ) * glm::scale( glm::vec3( 1, 1, 1 ) - texelSize );
}
