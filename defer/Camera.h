#pragma once

#include "Transform.h"

namespace nsfw {

    class Camera : public Transform {
    private:
    protected:
        bool m_isOrtho;

        // In degrees
        float m_near, m_far, m_width, m_height, m_fov;

    public:

        Camera( glm::vec3 position, float fov = 0.78f, float width = 1600.f, float height = 900.f, float near = 0.1f, float far = 1000.0f ) :
            Transform(), m_isOrtho( false ), m_near( near ), m_far( far ), m_width( width ), m_height( height ), m_fov( fov ) {}

        virtual ~Camera() {}

        virtual void Update() {}

        virtual glm::mat4 GetViewTransform() const {
            return glm::inverse( glm::mat4(1) );
        }

        virtual glm::mat4 GetProjection() const {
            if ( m_isOrtho ) {
                float halfWidth = m_width * 0.5f;
                float halfHeight = m_height * 0.5f;
                return glm::ortho( -halfWidth, halfWidth, -halfHeight, halfWidth, m_near, m_far );
            }
            return glm::perspective( m_fov, m_width / m_height, m_near, m_far );
        }
    };

}
