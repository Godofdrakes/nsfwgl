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
    glfwPollEvents();
    glfwSwapBuffers( window );
}

void nsfw::Window::term() {
    // CLEAN UP YOUR SHIT
    glfwDestroyWindow( window );
    window = nullptr;
}

float nsfw::Window::getTime() const {
    return (float)glfwGetTime();
}

bool nsfw::Window::getKey( unsigned k ) const {
    return glfwGetKey( window, k ) != GLFW_RELEASE;
}

bool nsfw::Window::getShouldClose() const {
    return glfwWindowShouldClose( window ) != 0;
}

unsigned nsfw::Window::getWidth() const {
    return width;
}

unsigned nsfw::Window::getHeight() const {
    return height;
}

glm::mat4 nsfw::Window::getTexelAdjustmentMatrix() const {
#pragma message ( __WARN__ "Implemented, not tested." )
    glm::vec3 texelSize = 1.0f / glm::vec3( width, height, 0 );
    glm::vec3 halfTexel = texelSize * 0.5f; // bottom left

    // scale up to the appropriate size, then shift it to line up w/center of texels
    return glm::translate( halfTexel ) * glm::scale( glm::vec3( 1, 1, 1 ) - texelSize );
}
