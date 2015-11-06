#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

namespace nsfw {

    class Transform {
    public:
        glm::vec3 m_rotation; // Rotation in degrees
        glm::vec3 m_scale;
        glm::vec3 m_position;

        Transform() : m_rotation( 0.0f ), m_scale( 1.0f ), m_position( 0.0f ) {}

        virtual ~Transform() {}

        glm::mat4 GetWorldTransform() const {
            return glm::inverse( glm::lookAt( m_position, glm::vec3( 0, 0, 1 ), glm::vec3( 0, 1, 0 ) ) );
        }

    };

}
