#pragma once

#include <glm\glm.hpp>
#include <glm\ext.hpp>

#include "../nsfwgl/nsfw.h"

struct Camera {
    glm::mat4 transform;
    float near, far, aspect, fov;
    glm::vec3 worldPosition;

    Camera() : near( 0.1 ), far( 1000 ), aspect( nsfw::Window::instance().getWidth() / ( float )nsfw::Window::instance().getHeight() ), fov( 90 ), worldPosition( 0.f, 0.f, -5.f ) {}

    void update() {}

    void lookAt( glm::vec3 pos, glm::vec3 target, glm::vec3 up ) {
        transform = glm::inverse( glm::lookAt( pos, target, up ) );
    }

    glm::mat4 getView() const {
        return glm::inverse( transform );
    }

    glm::mat4 getProjection() const {
        return glm::perspective( fov, aspect, near, far );
    }
};
