#include "nsfw.h"
#include <GLFW/glfw3.h>

void nsfw::Window::init( unsigned width, unsigned height ) {
//#pragma message ( __WARN__ "Should create and set an active windowing context. ONLY GLFW! No GL!" )
    //using namespace gl;

    this->width = width;
    this->height = height;

    // Init GLFW
    assert( glfwInit() );
    window = glfwCreateWindow( width, height, "NSFW", nullptr, nullptr );
    assert( window != nullptr );
    glfwMakeContextCurrent( window );
}

void nsfw::Window::step() {
#pragma message ( __WARN__ "GLFW poll events and swap buffers is all that should really be here! No GL!" )
    glfwPollEvents();
    glfwSwapBuffers( window );
}

void nsfw::Window::term() {
#pragma message ( __WARN__ )
    glfwDestroyWindow( window );
    window = nullptr;
}

float nsfw::Window::getTime() const {
//#pragma message ( __WARN__ )
    return (float)glfwGetTime();
}

bool nsfw::Window::getKey( unsigned k ) const {
//#pragma message ( __WARN__ )
    return glfwGetKey( window, k ) != GLFW_RELEASE;
}

bool nsfw::Window::getShouldClose() const {
//#pragma message ( __WARN__ )
    return glfwWindowShouldClose( window ) != 0;
}

unsigned nsfw::Window::getWidth() const {
//#pragma message ( __WARN__ )
    return width;
}

unsigned nsfw::Window::getHeight() const {
//#pragma message ( __WARN__ )
    return height;
}

glm::mat4 nsfw::Window::getTexelAdjustmentMatrix() const {
#pragma message ( __WARN__ "Implemented, not tested." )

    glm::vec3 texelSize = 1.0f / glm::vec3( width, height, 0 );
    glm::vec3 halfTexel = texelSize * 0.5f; // bottom left

    // scale up to the appropriate size, then shift it to line up w/center of texels
    return glm::translate( halfTexel ) * glm::scale( glm::vec3( 1, 1, 1 ) - texelSize );
}
