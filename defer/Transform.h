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

        virtual glm::mat4 GetRoationMatrix() const {
            return glm::rotate( m_rotation.z, glm::vec3( 0, 0, 1 ) ) *
                    glm::rotate( m_rotation.y, glm::vec3( 0, 1, 0 ) ) *
                    glm::rotate( m_rotation.x, glm::vec3( 1, 0, 0 ) );
        }

        virtual glm::mat4 GetWorldTransform() const {
            return glm::translate( m_position ) *
                    GetRoationMatrix() *
                    glm::scale( m_scale );
        }
    };

}
